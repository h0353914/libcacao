// cammw_thread_context 的共用定義 —— thread.cpp 實作它，msg.cpp /
// msg_server.cpp 把它內嵌進 session/server context，所以獨立成頭檔。
//
// 欄位/offset 逐行對照 kong 這輪的 decompiled.c 核對過，見 src/thread.cpp
// 開頭的說明。

#pragma once

#include <pthread.h>
#include <stddef.h>
#include <stdint.h>

constexpr size_t kCammwThreadNameSize = 0x40;

struct cammw_thread_context {
  uint8_t external_shutdown_enabled;  // +0x00
  uint8_t reserved_01[3];             // +0x01
  pthread_t thread_handle;            // +0x04
  uint8_t thread_started;             // +0x08
  char thread_name[kCammwThreadNameSize];  // +0x09  "cammw:%s"
  uint8_t reserved_49[0x4c - 0x49];   // +0x49
  int priority;                       // +0x4c  clamp(requested, 100, 139)
  int control_read_fd;                // +0x50
  int control_write_fd;               // +0x54
  int thread_id;                      // +0x58  gettid()
  int (*thread_routine)(void *);      // +0x5c
  void *thread_argument;              // +0x60
};

static_assert(sizeof(cammw_thread_context) == 0x64, "跟 msg_server_context/msg_session_t 內嵌大小對得上");
static_assert(offsetof(cammw_thread_context, thread_handle) == 0x04, "pthread_create 第一個參數");
static_assert(offsetof(cammw_thread_context, thread_started) == 0x08, "param_6[8] 的存在旗標");
static_assert(offsetof(cammw_thread_context, thread_name) == 0x09, "snprintf(param_6+9, ...)");
static_assert(offsetof(cammw_thread_context, priority) == 0x4c, "*(int*)(param_6+0x4c)");
static_assert(offsetof(cammw_thread_context, control_read_fd) == 0x50, "pipe((int*)(param_6+0x50))");
static_assert(offsetof(cammw_thread_context, control_write_fd) == 0x54, "param_6+0x54");
static_assert(offsetof(cammw_thread_context, thread_id) == 0x58, "gettid() 存放處");
static_assert(offsetof(cammw_thread_context, thread_routine) == 0x5c, "*(int*)(param_6+0x5c)=param_4");
static_assert(offsetof(cammw_thread_context, thread_argument) == 0x60, "*(undefined4*)(param_6+0x60)=param_5");

extern "C" int cammw_util_thread_create(int external_shutdown_enabled, const char *name,
                                        int priority, int (*routine)(void *), void *argument,
                                        cammw_thread_context *ctx);
extern "C" void cammw_util_thread_destroy(cammw_thread_context *ctx);
