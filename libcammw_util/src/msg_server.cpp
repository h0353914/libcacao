// cammw_util 訊息層重建（server 側）
//
// 逐行對照 kong 這輪的 ../kong-output/decompiled.c：
//   cammw_util_msg_srv_open           @ 0x12b18
//   cammw_util_msg_server_thread      @ 0x12c68
//   cammw_util_msg_srv_create_session @ 0x12dc4
//   cammw_util_msg_srv_close          @ 0x12d60
//   cammw_util_msg_srv_delete_session @ 0x12f18

#define LOG_TAG "cammw_util"

#include "cammw_msg.h"
#include "cammw_util_internal.h"

#include <log/log.h>

#include <cutils/sockets.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

extern "C" cammw_util_msg_session_t *cammw_util_msg_create_session(
    int fd, const char *thread_name, int priority, void *user_ctx,
    uint32_t (*message_callback)(void *, cammw_util_msg_t *, void *),
    void (*disconnect_callback)(void *, void *), uint32_t private_data_size, void *private_data,
    int aux1, int aux2);
extern "C" void cammw_util_msg_delete_session(cammw_util_msg_session_t *session);
extern "C" int cammw_util_msg_server_thread(void *arg);

// 開一個伺服端 socket（Android control socket，非 abstract 命名空間）、
// listen()、啟動 accept 執行緒。connection_callback(accepted_fd, context)
// 會在每次 accept() 成功後被叫。
extern "C" cammw_msg_server_context *cammw_util_msg_srv_open(const char *name, int priority,
                                                              void *connection_context,
                                                              void (*connection_callback)(int,
                                                                                          void *)) {
  if (name == nullptr || connection_callback == nullptr) {
    return nullptr;
  }

  auto *server = static_cast<cammw_msg_server_context *>(calloc(1, sizeof(cammw_msg_server_context)));
  if (server == nullptr) {
    ALOGE("E: %s: no memory", __FUNCTION__);
    return nullptr;
  }
  server->fd = -1;
  server->connection_callback_context = connection_context;
  server->connection_callback = connection_callback;
  pthread_mutex_init(&server->session_table_mutex, nullptr);

  char sock_name[108];
  snprintf(sock_name, sizeof(sock_name), "cammw_%s", name);
  int fd = android_get_control_socket(sock_name);
  bool ok = false;
  if (fd < 0) {
    ALOGE("E: %s: socket connect err fd=%d %s (%d %s)", __FUNCTION__, fd, sock_name, errno,
          strerror(errno));
  } else if (listen(fd, 4) < 0) {
    ALOGE("E: %s: listen err %d", __FUNCTION__, errno);
    close(fd);
  } else {
    server->fd = fd;
    strlcpy(server->socket_path, sock_name, sizeof(server->socket_path));
    char thread_name[64];
    snprintf(thread_name, sizeof(thread_name), "cammw_srv:%s", name);
    if (cammw_util_thread_create(0, thread_name, priority, cammw_util_msg_server_thread, server,
                                  &server->accept_thread) == 0) {
      ok = true;
    } else {
      close(fd);
      server->fd = -1;
    }
  }

  if (!ok) {
    pthread_mutex_destroy(&server->session_table_mutex);
    free(server);
    return nullptr;
  }
  return server;
}

// accept loop：select({shutdown_fd, listen_fd})，收到 shutdown 的 'y'
// 就結束，否則 accept() 一個連線就呼叫 connection_callback。
extern "C" int cammw_util_msg_server_thread(void *arg) {
  auto *server = static_cast<cammw_msg_server_context *>(arg);
  if (server == nullptr || server->fd < 0) {
    return 0xffffff99;
  }

  const int shutdown_fd = server->accept_thread.control_read_fd;
  const int listen_fd = server->fd;
  const int nfds = (shutdown_fd > listen_fd ? shutdown_fd : listen_fd) + 1;

  bool done = false;
  while (!done) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(shutdown_fd, &fds);
    FD_SET(listen_fd, &fds);
    if (select(nfds, &fds, nullptr, nullptr, nullptr) < 0) {
      break;
    }
    if (FD_ISSET(shutdown_fd, &fds)) {
      char c = 0;
      read(shutdown_fd, &c, 1);
      done = (c == 'y');
    }
    if (!FD_ISSET(listen_fd, &fds)) {
      continue;
    }
    int client_fd = accept(listen_fd, nullptr, nullptr);
    if (client_fd >= 0) {
      server->connection_callback(client_fd, server->connection_callback_context);
    }
  }
  return 0;
}

// 收到新連線後，替它在 4 個 session 槽位裡找一個空位，建立 session，
// 成功就標成 StartupDone（result_a=0，喚醒任何在等待的人）。
// ABI 注意：**回傳 session 指標**（失敗為 nullptr），不是狀態碼。原版結尾是
// `return iVar4`，iVar4 就是 cammw_util_msg_create_session() 的結果，失敗路徑
// 一律設成 0。之前這裡回傳「0 = 成功」的狀態碼，語意跟呼叫端完全相反，
// 導致 libcammw.so 的 cammw_link_tintless_srv_done_cb 判定失敗並印出
// "server session create error"，相機開不起來。
//
// 守衛條件也照原版：檢查 disconnect_callback / message_callback / name /
// **server->fd**（不是傳進來的 fd），而且原版不檢查 user_ctx。
extern "C" cammw_util_msg_session_t *cammw_util_msg_srv_create_session(
    cammw_msg_server_context *server, int fd, const char *name, int priority, void *user_ctx,
    uint32_t (*message_callback)(void *, cammw_util_msg_t *, void *),
    void (*disconnect_callback)(void *, void *), uint32_t private_data_size, void *private_data,
    int aux1, int aux2) {
  if (server == nullptr) {
    return nullptr;
  }
  if (disconnect_callback == nullptr || message_callback == nullptr || name == nullptr ||
      server->fd < 0) {
    if (fd >= 0) {
      close(fd);
    }
    return nullptr;
  }

  int status = -0x6b;
  cammw_util_msg_session_t *session = nullptr;
  pthread_mutex_lock(&server->session_table_mutex);
  for (int i = 0; i < 4; ++i) {
    if (server->session_table[i] != nullptr) {
      continue;
    }
    char session_name[64];
    snprintf(session_name, sizeof(session_name), "%s_session.%d", name, i);
    session = cammw_util_msg_create_session(fd, session_name, priority, user_ctx, message_callback,
                                             disconnect_callback, private_data_size, private_data,
                                             aux1, aux2);
    if (session == nullptr) {
      status = -0x66;
    } else {
      server->session_table[i] = session;
      status = 0;
      server->session_count++;
    }
    break;
  }
  pthread_mutex_unlock(&server->session_table_mutex);

  if (status != 0) {
    if (fd >= 0) {
      close(fd);
    }
    return nullptr;
  }

  // 原版是對 +0x74 做一次 2-byte 寫入 0x100，也就是同時把
  // disconnected(+0x74)=0、has_result(+0x75)=1——不是寫 result_a(+0x78)。
  pthread_mutex_lock(&session->wait_mutex);
  session->disconnected = 0;
  session->has_result = 1;
  pthread_cond_signal(&session->wait_cond);
  pthread_mutex_unlock(&session->wait_mutex);
  return session;
}

extern "C" void cammw_util_msg_srv_close(cammw_msg_server_context *server) {
  if (server == nullptr || server->fd < 0) {
    return;
  }
  pthread_mutex_lock(&server->session_table_mutex);
  for (int i = 0; i < 4; ++i) {
    if (server->session_table[i] != nullptr) {
      cammw_util_msg_delete_session(server->session_table[i]);
      server->session_table[i] = nullptr;
    }
  }
  pthread_mutex_unlock(&server->session_table_mutex);
  cammw_util_thread_destroy(&server->accept_thread);
  close(server->fd);
  unlink(server->socket_path);
  pthread_mutex_destroy(&server->session_table_mutex);
  free(server);
}

extern "C" void cammw_util_msg_srv_delete_session(cammw_msg_server_context *server,
                                                  cammw_util_msg_session_t *session) {
  if (server == nullptr || session == nullptr || server->fd < 0 || session->fd < 0) {
    return;
  }
  pthread_mutex_lock(&server->session_table_mutex);
  for (int i = 0; i < 4; ++i) {
    if (server->session_table[i] == session) {
      cammw_util_msg_delete_session(session);
      server->session_table[i] = nullptr;
      server->session_count--;
      break;
    }
  }
  pthread_mutex_unlock(&server->session_table_mutex);
}
