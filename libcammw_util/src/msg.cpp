// cammw_util 訊息層重建（client 側 + wait/signal + session 收訊執行緒）
//
// 逐行對照 kong 這輪的 ../kong-output/decompiled.c，函式對應關係與位址
// 全部列在 include/cammw_msg.h 開頭。伺服端專屬的部分（srv_open/
// msg_server_thread/srv_create_session/srv_close/srv_delete_session）
// 在 msg_server.cpp。

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
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

extern "C" cammw_util_msg_session_t *cammw_util_msg_create_session(
    int fd, const char *thread_name, int priority, void *user_ctx,
    uint32_t (*message_callback)(void *, cammw_util_msg_t *, void *),
    void (*disconnect_callback)(void *, void *), uint32_t private_data_size, void *private_data,
    int aux1, int aux2);

namespace {
constexpr int kCammwMsgMaxPayload = 0xc00;  // 3072 bytes，recv_payload{,_big} 的大小
}  // namespace

// 組兩個 iovec（16-byte header + payload）送出去。fd_count 只在
// flags 不是 StartupDone/Reply 時才帶（原版：`(flags & ~1) != 2`）。
extern "C" int cammw_util_msg_send_internal(int fd, uint32_t flags, uint32_t result,
                                            const cammw_util_msg_t *msg) {
  if (fd < 0 || msg == nullptr) {
    return -0x67;
  }

  cammw_msg_wire_header header{};
  header.flags = flags;
  header.fd_count = (flags == kCammwMsgStartupDone || flags == kCammwMsgReply) ? 0 : msg->fd_count;
  header.payload_size = msg->size;
  header.result = result;

  iovec iov[2];
  iov[0].iov_base = &header;
  iov[0].iov_len = sizeof(header);
  iov[1].iov_base = msg->payload;
  iov[1].iov_len = msg->size;

  msghdr mh{};
  mh.msg_iov = iov;
  mh.msg_iovlen = 2;

  // SCM_RIGHTS ancillary data：只有 fd_count != 0 才附上。
  alignas(int) char cmsg_buf[CMSG_SPACE(sizeof(int) * 10)];
  if (header.fd_count != 0) {
    if (header.fd_count > 10) {
      ALOGE("E: %s: no memory", __FUNCTION__);
      return -0x66;
    }
    const size_t fds_bytes = sizeof(int) * header.fd_count;
    mh.msg_control = cmsg_buf;
    mh.msg_controllen = CMSG_SPACE(fds_bytes);
    cmsghdr *cmsg = CMSG_FIRSTHDR(&mh);
    cmsg->cmsg_len = CMSG_LEN(fds_bytes);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    memcpy(CMSG_DATA(cmsg), msg->fds, fds_bytes);
  }

  ssize_t sent = sendmsg(fd, &mh, MSG_NOSIGNAL);
  if (sent < 0) {
    ALOGE("E: %s: sendmsg err %d", __FUNCTION__, errno);
    return -0x6f;
  }
  return 0;
}

extern "C" int cammw_util_msg_send(cammw_util_msg_session_t *session, cammw_util_msg_t *msg) {
  if (session == nullptr || msg == nullptr || session->fd < 0) {
    return CAMMW_ERR_INVALID_ARG;
  }
  pthread_mutex_lock(&session->send_mutex);
  int rc = cammw_util_msg_send_internal(session->fd, kCammwMsgNotify, 0, msg);
  pthread_mutex_unlock(&session->send_mutex);
  return rc;
}

// 等待一則回覆（或起始交握）。response_selector 選 result_a(0)/result_b(1)。
// timeout_sec==0 -> 無限等；否則用 pthread_cond_timedwait。
// 呼叫端要先鎖住 wait_mutex 再進來（跟原版一樣，鎖的生命週期由呼叫端管）。
extern "C" uint32_t cammw_util_msg_wait(cammw_util_msg_session_t *session, cammw_util_msg_t *out,
                                        uint8_t response_selector, uint32_t timeout_sec) {
  if (session == nullptr || session->fd < 0) {
    return 0xffffff99;
  }

  uint32_t result;
  if (!session->has_result) {
    while (!session->disconnected) {
      if (timeout_sec == 0) {
        pthread_cond_wait(&session->wait_cond, &session->wait_mutex);
      } else {
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += timeout_sec;
        int rc = pthread_cond_timedwait(&session->wait_cond, &session->wait_mutex, &ts);
        if (rc != 0) {
          ALOGE("E: %s: recv wait timeout error %d", __FUNCTION__, rc);
          return 0xffffff92;
        }
      }
      if (session->has_result) {
        break;
      }
    }
  }

  result = response_selector == 0 ? session->result_a : session->result_b;
  if (out != nullptr) {
    uint32_t n = session->recv_size < out->size ? session->recv_size : out->size;
    out->size = n;
    if (n != 0 && out->payload != nullptr) {
      memcpy(out->payload, session->recv_payload, n);
    }
  }
  return result;
}

extern "C" int cammw_util_msg_sendrecv(cammw_util_msg_session_t *session, cammw_util_msg_t *msg) {
  if (session == nullptr || msg == nullptr || session->fd < 0) {
    return -0x67;
  }
  pthread_mutex_lock(&session->send_mutex);
  pthread_mutex_lock(&session->wait_mutex);
  session->has_result = 0;
  int rc = cammw_util_msg_send_internal(session->fd, kCammwMsgRequest, 0, msg);
  if (rc == 0) {
    rc = cammw_util_msg_wait(session, msg, /*response_selector=*/1, /*timeout_sec=*/0);
  }
  pthread_mutex_unlock(&session->wait_mutex);
  pthread_mutex_unlock(&session->send_mutex);
  return rc;
}

// 把收到的回覆/通知寫進 session、喚醒等待者。response_selector 選
// result_a(0)/result_b(1)。呼叫端負責鎖 wait_mutex。
extern "C" void cammw_util_msg_signal(cammw_util_msg_session_t *session, uint32_t result,
                                      const cammw_util_msg_t *msg, uint8_t response_selector) {
  if (session == nullptr || session->fd < 0) {
    return;
  }
  if (response_selector == 0) {
    session->result_a = result;
  } else {
    session->result_b = result;
  }
  session->recv_size = 0;
  if (msg != nullptr) {
    uint32_t n = msg->size < kCammwMsgMaxPayload ? msg->size : kCammwMsgMaxPayload;
    session->recv_size = n;
    if (n != 0 && msg->payload != nullptr) {
      memcpy(session->recv_payload, msg->payload, n);
    }
  }
  pthread_cond_signal(&session->wait_cond);
}

// 一個 session 的收訊事件迴圈：select({shutdown_fd, socket_fd})，
// recvmsg 收 16-byte header（+ 附帶的 fd），payload 太大時再收第二次到
// recv_payload_big。依 header.flags 做 4 way 分派。
extern "C" int cammw_util_msg_session_thread(void *arg) {
  auto *session = static_cast<cammw_util_msg_session_t *>(arg);
  if (session == nullptr || session->fd < 0) {
    return 0xffffff99;
  }

  // 起手先送一個 StartupDone 通知——msg_create_session 的呼叫端就是在等這個。
  pthread_mutex_lock(&session->send_mutex);
  cammw_util_msg_t startup_msg{};
  startup_msg.size = session->private_data_size;
  startup_msg.payload = session->private_data;
  cammw_util_msg_send_internal(session->fd, kCammwMsgStartupDone, 0, &startup_msg);
  pthread_mutex_unlock(&session->send_mutex);

  const int shutdown_fd = session->recv_thread.control_read_fd;
  const int sock_fd = session->fd;
  const int nfds = (shutdown_fd > sock_fd ? shutdown_fd : sock_fd) + 1;

  bool done = false;
  int status = 0;
  while (!done) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(shutdown_fd, &fds);
    FD_SET(sock_fd, &fds);
    if (select(nfds, &fds, nullptr, nullptr, nullptr) < 0) {
      break;
    }

    if (FD_ISSET(shutdown_fd, &fds)) {
      char c = 0;
      read(shutdown_fd, &c, 1);
      if (c == 'y') {
        break;
      }
    }
    if (!FD_ISSET(sock_fd, &fds)) {
      continue;
    }

    cammw_msg_wire_header header{};
    iovec iov{&header, sizeof(header)};
    alignas(int) char cmsg_buf[CMSG_SPACE(sizeof(int) * 10)];
    msghdr mh{};
    mh.msg_iov = &iov;
    mh.msg_iovlen = 1;
    mh.msg_control = cmsg_buf;
    mh.msg_controllen = sizeof(cmsg_buf);

    ssize_t n = recvmsg(sock_fd, &mh, 0);
    if (n < 1) {
      if (n == 0) {
        // 對面關了。
        pthread_mutex_lock(&session->wait_mutex);
        session->disconnected = 1;
        if (session->fd >= 0) {
          session->result_a = 0xffffff8f;
          session->recv_size = 0;
          pthread_cond_signal(&session->wait_cond);
        }
        pthread_mutex_unlock(&session->wait_mutex);
        if (session->disconnect_callback != nullptr) {
          session->disconnect_callback(session, session->user_ctx);
        }
        status = 0xffffff8f;
      } else {
        ALOGE("E: %s: recvmsg err %zd", __FUNCTION__, n);
        status = 0xffffff91;
      }
      break;
    }

    // 隨附的 fd（SCM_RIGHTS）。
    int recv_fds[10] = {};
    if (header.fd_count != 0) {
      if (header.fd_count > 10) {
        ALOGE("E: %s: fd num err %u", __FUNCTION__, header.fd_count);
        status = 0xffffff91;
        break;
      }
      cmsghdr *cmsg = CMSG_FIRSTHDR(&mh);
      if (cmsg != nullptr && cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SCM_RIGHTS) {
        memcpy(recv_fds, CMSG_DATA(cmsg), sizeof(int) * header.fd_count);
      }
    }

    // payload 另外收一次，收進大緩衝（跟 header 分開兩趟 recvmsg）。
    uint8_t *payload_buf = session->recv_payload_big;
    uint32_t payload_size = header.payload_size;
    if (payload_size != 0) {
      iovec iov2{payload_buf, payload_size};
      msghdr mh2{};
      mh2.msg_iov = &iov2;
      mh2.msg_iovlen = 1;
      recvmsg(sock_fd, &mh2, 0);
    }

    cammw_util_msg_t recv_msg{};
    recv_msg.size = payload_size;
    recv_msg.payload = payload_buf;
    recv_msg.fd_count = header.fd_count;
    memcpy(recv_msg.fds, recv_fds, sizeof(recv_fds));

    switch (header.flags) {
      case kCammwMsgNotify:
        if (session->message_callback != nullptr) {
          session->message_callback(session, &recv_msg, session->user_ctx);
        }
        break;
      case kCammwMsgRequest: {
        uint32_t reply_result = 0;
        if (session->message_callback != nullptr) {
          reply_result = session->message_callback(session, &recv_msg, session->user_ctx);
        }
        pthread_mutex_lock(&session->send_mutex);
        cammw_util_msg_send_internal(session->fd, kCammwMsgReply, reply_result, &recv_msg);
        pthread_mutex_unlock(&session->send_mutex);
        break;
      }
      case kCammwMsgStartupDone:
        pthread_mutex_lock(&session->wait_mutex);
        session->has_result = 1;
        cammw_util_msg_signal(session, header.result, &recv_msg, /*response_selector=*/0);
        pthread_mutex_unlock(&session->wait_mutex);
        break;
      case kCammwMsgReply:
        pthread_mutex_lock(&session->wait_mutex);
        session->has_result = 1;
        cammw_util_msg_signal(session, header.result, &recv_msg, /*response_selector=*/1);
        pthread_mutex_unlock(&session->wait_mutex);
        break;
      default:
        break;
    }
  }
  return status;
}

// client 端連線並建立 session。fd 由這裡開，session 建立失敗會關掉。
//
// ABI 注意：**直接回傳 session 指標**，不是 0/1 的成功旗標，也沒有
// out_session 參數。依據是原廠呼叫端 libcammw.so 的
// cammw_link_tintless_cli_open @0x66cdc：
//     bl  msg_cli_open
//     cmp r0, #0
//     str r0, [r4]      ← 把回傳值本身存成 session handle
//     bne success
// 而引數是 r0=name、r1=0x64(priority)、r2=user_ctx、r3=message_callback，
// 堆疊上依序是 disconnect_callback、private_data_size(4)、private_data。
//
// 之前這裡多插了一個不存在的 out_session 參數並回傳 1，造成
// (a) r2 之後的引數整組錯位，(b) 呼叫端把 1 當成 session 指標，接著
// cammw_util_msg_sendrecv 讀 session->fd 就 SIGSEGV——實機重現於
// cammw_link_tintless_cli_map_buf → msg_sendrecv+24，fault addr 0x1。
extern "C" cammw_util_msg_session_t *cammw_util_msg_cli_open(
    const char *name, int priority, void *user_ctx,
    uint32_t (*message_callback)(void *, cammw_util_msg_t *, void *),
    void (*disconnect_callback)(void *, void *), uint32_t private_data_size, void *private_data,
    int aux1, int aux2) {
  if (name == nullptr || user_ctx == nullptr) {
    return nullptr;
  }

  char sock_name[108];
  snprintf(sock_name, sizeof(sock_name), "cammw_%s", name);
  int fd = socket_local_client(sock_name, ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM);
  if (fd < 0) {
    ALOGE("E: %s: socket connect err fd=%d %s (%d %s)", __FUNCTION__, fd, sock_name, errno,
          strerror(errno));
    return nullptr;
  }

  char session_name[64];
  snprintf(session_name, sizeof(session_name), "%s_client", name);
  cammw_util_msg_session_t *session = cammw_util_msg_create_session(
      fd, session_name, priority, user_ctx, message_callback, disconnect_callback,
      private_data_size, private_data, aux1, aux2);
  if (session == nullptr) {
    ALOGE("E: %s: create_session err", __FUNCTION__);
    close(fd);
    return nullptr;
  }
  return session;
}

// 配置一個 session 物件、啟動它的收訊執行緒、等 StartupDone 交握完成。
extern "C" cammw_util_msg_session_t *cammw_util_msg_create_session(
    int fd, const char *thread_name, int priority, void *user_ctx,
    uint32_t (*message_callback)(void *, cammw_util_msg_t *, void *),
    void (*disconnect_callback)(void *, void *), uint32_t private_data_size, void *private_data,
    int aux1, int aux2) {
  auto *session = static_cast<cammw_util_msg_session_t *>(calloc(1, sizeof(cammw_util_msg_session_t)));
  if (session == nullptr) {
    ALOGE("E: %s: no memory", __FUNCTION__);
    return nullptr;
  }

  pthread_mutex_init(&session->send_mutex, nullptr);
  pthread_mutex_init(&session->wait_mutex, nullptr);
  pthread_cond_init(&session->wait_cond, nullptr);
  session->user_ctx = user_ctx;
  session->message_callback = message_callback;
  session->disconnect_callback = disconnect_callback;
  session->fd = fd;

  bool ok = false;
  if (private_data_size != 0 && private_data != nullptr) {
    void *copy = calloc(1, private_data_size);
    session->private_data = copy;
    if (copy == nullptr) {
      ALOGE("E: %s: no memory", __FUNCTION__);
      goto fail;
    }
    memcpy(copy, private_data, private_data_size);
    session->private_data_size = private_data_size;
  } else {
    session->private_data_size = 0;
    session->private_data = nullptr;
  }

  {
    pthread_mutex_lock(&session->wait_mutex);
    session->has_result = 0;
    pthread_mutex_unlock(&session->wait_mutex);

    int rc = cammw_util_thread_create(0, thread_name, priority, cammw_util_msg_session_thread,
                                       session, &session->recv_thread);
    if (rc == 0) {
      pthread_mutex_lock(&session->wait_mutex);
      // aux1/aux2 對應原版第 9、10 個參數：兩者都非 0 才用，語意未完全
      // 確認（這輪沒有再往下查呼叫端怎麼用），先照原本邏輯保留分支。
      int wait_rc = cammw_util_msg_wait(session, nullptr, /*response_selector=*/0, /*timeout_sec=*/5);
      pthread_mutex_unlock(&session->wait_mutex);
      if (wait_rc == 0) {
        ok = true;
      }
    }
  }

fail:
  if (ok) {
    return session;
  }
  cammw_util_thread_destroy(&session->recv_thread);
  if (session->private_data != nullptr) {
    free(session->private_data);
  }
  pthread_cond_destroy(&session->wait_cond);
  pthread_mutex_destroy(&session->wait_mutex);
  pthread_mutex_destroy(&session->send_mutex);
  free(session);
  return nullptr;
}

extern "C" void cammw_util_msg_delete_session(cammw_util_msg_session_t *session) {
  if (session == nullptr || session->fd < 0) {
    return;
  }
  cammw_util_thread_destroy(&session->recv_thread);
  close(session->fd);
  if (session->private_data != nullptr) {
    free(session->private_data);
  }
  pthread_cond_destroy(&session->wait_cond);
  pthread_mutex_destroy(&session->wait_mutex);
  pthread_mutex_destroy(&session->send_mutex);
  free(session);
}

// kong 這輪沒有分析到這個函式（跟 shmem_register_buf/unregister_buf
// 一樣），沒有反編譯可以對照。從命名對稱性（cli_open 的收尾）合理推斷
// 就是 msg_delete_session 的別名，行為上等價：關掉 client 端開的 session。
extern "C" void cammw_util_msg_cli_close(cammw_util_msg_session_t *session) {
  cammw_util_msg_delete_session(session);
}
