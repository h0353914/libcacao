// cammw_util 的執行緒管理重建
//
// 這次是直接對照 kong 這輪重新產生的 decompiled.c 逐行核對過的（不是憑
// 記憶抄），涵蓋：
//   cammw_util_thread_create  @ 0x125cc
//   cammw_util_thread_run     @ 0x12720
//   cammw_util_thread_destroy @ 0x12764
//
// 結構定義搬到 include/cammw_thread.h（msg 層要內嵌它），這裡只留實作。

#define LOG_TAG "cammw_util"

#include "cammw_thread.h"
#include "cammw_util_internal.h"

#include <errno.h>
#include <log/log.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <unistd.h>

namespace {

// pthread_create 實際指定的內部進入點；真正的使用者 routine 存在
// thread_routine 欄位，由這裡轉呼叫。
void *thread_run_trampoline(void *arg) {
  auto *ctx = static_cast<cammw_thread_context *>(arg);
  if (ctx == nullptr || ctx->thread_routine == nullptr) {
    return nullptr;
  }
  ctx->thread_id = gettid();
  prctl(PR_SET_NAME, ctx->thread_name, 0, 0, 0);
  // 原版：setpriority(PRIO_PROCESS, 0, priority - 0x78)。
  setpriority(PRIO_PROCESS, 0, ctx->priority - 0x78);
  int rc = ctx->thread_routine(ctx->thread_argument);
  return reinterpret_cast<void *>(static_cast<intptr_t>(rc));
}

}  // namespace

// 建立一個受管理的執行緒。
//
//   external_shutdown_enabled == 0 -> 內部建立一組 pipe（control_read_fd/
//     control_write_fd），destroy 時會往 write 端送一個 'y' 通知執行緒
//     結束；由呼叫端自行透過 select() 監控 control_read_fd。
//   external_shutdown_enabled != 0 -> 不建立 pipe，destroy 純粹
//     join/detach，關閉時機由呼叫端另外的機制負責。
//
// priority 會 clamp 到 [100, 139]（原版：min(param_3,0x8b) 再 max(.,100)）。
extern "C" int cammw_util_thread_create(int external_shutdown_enabled, const char *name,
                                        int priority, int (*routine)(void *), void *argument,
                                        cammw_thread_context *ctx) {
  if (name == nullptr || routine == nullptr || ctx == nullptr) {
    return CAMMW_ERR_INVALID_ARG;
  }
  if (strnlen(name, kCammwThreadNameSize) == 0) {
    return CAMMW_ERR_INVALID_ARG;
  }
  if (ctx->thread_started != 0) {
    // 原版回 -0x6b（跟 INVALID_ARG 的 -0x67/-0x99 不同碼，專指「已經在跑」）。
    return -0x6b;
  }

  int clamped = priority < 0x8b ? priority : 0x8b;
  clamped = clamped > 100 ? clamped : 100;
  ctx->priority = clamped;
  snprintf(ctx->thread_name, kCammwThreadNameSize, "cammw:%s", name);

  if (external_shutdown_enabled == 0) {
    int fds[2];
    if (pipe(fds) < 0) {
      ALOGE("E: %s: pipe err %d", __FUNCTION__, errno);
      return -0x91;
    }
    ctx->control_read_fd = fds[0];
    ctx->control_write_fd = fds[1];
  }

  ctx->thread_routine = routine;
  ctx->thread_argument = argument;

  int rc = pthread_create(&ctx->thread_handle, nullptr, thread_run_trampoline, ctx);
  if (rc == 0) {
    ctx->external_shutdown_enabled = static_cast<uint8_t>(external_shutdown_enabled);
    ctx->thread_started = 1;
    return 0;
  }

  ALOGE("E: %s: pthread_create fail %d", __FUNCTION__, rc);
  if (external_shutdown_enabled == 0) {
    if (ctx->control_read_fd >= 0) {
      close(ctx->control_read_fd);
      ctx->control_read_fd = -1;
    }
    if (ctx->control_write_fd >= 0) {
      close(ctx->control_write_fd);
      ctx->control_write_fd = -1;
    }
  }
  return -0x91;
}

// 停止並回收一個受管理的執行緒。
//   - 若是內部管理的 pipe（external_shutdown_enabled==0）且尚未關閉，
//     先送 'y' 通知它結束。
//   - 呼叫端若剛好就是該執行緒本身，用 detach；否則 join 等它結束。
//   - 內部管理的話把兩個 pipe fd 關掉、清成 -1。
//   - 清空 thread_started，允許之後重新 create。
extern "C" void cammw_util_thread_destroy(cammw_thread_context *ctx) {
  if (ctx == nullptr) {
    return;
  }
  const bool external = ctx->external_shutdown_enabled != 0;
  if (!external && ctx->control_write_fd < 0) {
    return;
  }
  if (ctx->thread_started == 0) {
    return;
  }

  pthread_t self = pthread_self();
  if (!external) {
    uint8_t byte = 'y';
    write(ctx->control_write_fd, &byte, 1);
  }

  if (pthread_equal(self, ctx->thread_handle)) {
    pthread_detach(ctx->thread_handle);
  } else {
    pthread_join(ctx->thread_handle, nullptr);
  }

  if (!external) {
    if (ctx->control_read_fd >= 0) {
      close(ctx->control_read_fd);
      ctx->control_read_fd = -1;
    }
    if (ctx->control_write_fd >= 0) {
      close(ctx->control_write_fd);
      ctx->control_write_fd = -1;
    }
  }
  ctx->thread_started = 0;
}
