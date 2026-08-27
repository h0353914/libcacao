// cammw_util 的 buffer manager 重建
//
// 參考：../kong-output/libcammw_util_v12/decompiled.c
//   cammw_util_bufmgr_init      @ 0x14fe4
//   cammw_util_bufmgr_open      @ 0x1500e
//   cammw_util_bufmgr_lock_buf  @ 0x1507c
//   cammw_util_bufmgr_unlock_buf@ 0x1512c
//   cammw_util_bufmgr_deinit    @ 0x15162
//
// 又一次：反編譯對這個結構的推斷是錯的（"ignoring multiple overlapping
// fields"、`buffer_manager[7].field_0x14` 這種存取）。跟 PtrList 一樣，
// 改讀機器碼。bufmgr_unlock_buf @ 0x1512c 最短最清楚：
//
//   ldr    r0,[r6,#0x0]      ; cmp r0,r5(index)  -> +0 是 buffer_count
//   add.w  r4,r6,#0xf4       ; 傳給 pthread_mutex_lock -> +0xf4 是 mutex
//   adds   r0,r6,r5
//   strb.w r5,[r0,#0xfc]     ; -> +0xfc 起是每個 buffer 一個 byte 的鎖旗標
//
// 配合 bufmgr_open 的「每筆前進 6 個 word」（24 bytes）與上限 10 筆：
//   count@0 + info[10]@4..0xf4 + mutex@0xf4 + lock_flags[10]@0xfc
// 4 + 240 = 244 = 0xf4，剛好接上 mutex，佈局自洽。

#define LOG_TAG "cammw_util"

#include "cammw_util_internal.h"

#include <log/log.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

// 上限來自 bufmgr_open 的 `buffer_count < 0xb` 檢查。
#define CAMMW_BUFMGR_MAX_BUFFERS 10

struct cammw_util_buffer_manager_t {
  uint32_t buffer_count;                                          // +0x000
  cammw_util_image_buffer_t buffer_info[CAMMW_BUFMGR_MAX_BUFFERS];// +0x004
  pthread_mutex_t buffer_mutex;                                   // +0x0f4
  // bionic 的 pthread_mutex_t 在 32-bit 只有 4 bytes，但機器碼顯示鎖旗標
  // 在 +0xfc，中間空著 4 bytes。bufmgr_init 有一次「清 8 bytes」的動作
  // （`*(undefined8 *)((int)&mutex + 4) = 0`）正好蓋住這一格加旗標開頭，
  // 所以它是結構的一部分而不是編譯器 padding。用途未確認。
  // **不是保留欄位**：bufmgr_lock_buf 拿它當 round-robin 配置游標
  // （0x1509e ldr.w r5,[r6,#0xf8] / 0x150b0 str.w r2,[r6,#0xf8]），
  // bufmgr_open 會把它歸零（0x15038 str.w r9,[r6,#0xf8]）。
  uint32_t alloc_cursor;                                          // +0x0f8
  uint8_t lock_flags[CAMMW_BUFMGR_MAX_BUFFERS];                   // +0x0fc
};

// 這幾個偏移全部來自機器碼，不是推測 —— 見檔案開頭的反組譯。
// 第一次寫的時候 image_buffer_t 少算一個欄位（20 而非 24 bytes），
// mutex 就落到 0xcc 去了，是被這幾行擋下來的。結構定義一動就會編譯失敗，
// 比事後靠測試發現可靠得多。
static_assert(sizeof(cammw_util_image_buffer_t) == 24,
              "bufmgr_open 每筆前進 6 個 word");
static_assert(offsetof(cammw_util_buffer_manager_t, buffer_count) == 0x00,
              "cmp 的對象是 [r6,#0]");
static_assert(offsetof(cammw_util_buffer_manager_t, buffer_mutex) == 0xf4,
              "add.w r4,r6,#0xf4 傳給 pthread_mutex_lock");
static_assert(offsetof(cammw_util_buffer_manager_t, lock_flags) == 0xfc,
              "strb.w r5,[r0,#0xfc] 是每 buffer 一 byte 的鎖旗標");

// 由 shmem.cpp 提供（尚未重建，先宣告）。
extern "C" int cammw_util_shmem_import_buf(int import_context, uint32_t import_arg, int fd,
                                           cammw_util_image_buffer_t *out);
extern "C" int cammw_util_shmem_free_buf(cammw_util_image_buffer_t *buffer);

extern "C" int cammw_util_bufmgr_init(cammw_util_buffer_manager_t *mgr) {
  if (mgr == nullptr) {
    return CAMMW_ERR_INVALID_ARG;
  }
  mgr->buffer_count = 0;
  mgr->alloc_cursor = 0;
  memset(mgr->buffer_info, 0, sizeof(mgr->buffer_info));
  memset(mgr->lock_flags, 0, sizeof(mgr->lock_flags));
  pthread_mutex_init(&mgr->buffer_mutex, nullptr);
  return 0;
}

extern "C" int cammw_util_bufmgr_open(cammw_util_buffer_manager_t *mgr, uint32_t buffer_count,
                                      uint32_t import_arg, int import_context,
                                      const int *input_fds) {
  if (mgr == nullptr || buffer_count > CAMMW_BUFMGR_MAX_BUFFERS) {
    return CAMMW_ERR_INVALID_ARG;
  }

  pthread_mutex_lock(&mgr->buffer_mutex);
  mgr->buffer_count = buffer_count;
  mgr->alloc_cursor = 0;  // 0x15038 str.w r9,[r6,#0xf8]

  for (uint32_t i = 0; i < buffer_count; ++i) {
    cammw_util_shmem_import_buf(import_context, import_arg, input_fds[i], &mgr->buffer_info[i]);
    mgr->lock_flags[i] = 0;
    // 原版在 import 之後就把傳進來的 fd 關掉 —— import 內部已經 dup 過。
    close(input_fds[i]);
  }

  pthread_mutex_unlock(&mgr->buffer_mutex);
  return 0;
}

// 取用一塊 buffer。**這個函式先前整個做錯了，是錄影失敗的原因。**
//
// 原版有**兩種模式**，由第 3 個參數是不是 null 決定（0x15096
// `cmp.w r10,#0x0 / beq`）：
//
//   out_index != null -> **自動配置**：從 +0xf8 的 round-robin 游標開始
//                        找第一個 lock_flags 為 0 的槽，把槽號寫進
//                        *out_index，游標前進（到 count 就繞回 0）。
//                        找不到空位就回 -0x68。
//   out_index == null -> **指定索引**：直接用第 2 個參數當索引，
//                        超出 count 或該槽已被鎖就回 -0x68。
//
// 兩條路最後都做同一件事（0x150ea / 0x1510a）：把該槽的
// cammw_util_image_buffer_t（24 bytes）整包複製到第 4 個參數指向的地方
// —— 機器碼用 `vld1.32 {d16,d17}` + `vldr.64 d18` 搬 16+8 = 24 bytes，
// 正好一整個描述子，不是我先前寫的「只回傳 size 跟 address 兩個欄位」。
// 然後把該槽的 lock_flags 設成 1。
//
// 槽位址的算法也在機器碼裡：`add.w r0,r5,r5,lsl #0x1` (= idx*3)
// 再 `add.w r0,r6,r0,lsl #0x3` (= mgr + idx*24)，最後 `adds r0,#0x4`
// 跳過開頭的 count —— 跟 buffer_info[idx] 完全一致。
//
// 先前我把第 2 個參數當索引、第 3/4 當兩個獨立輸出，於是呼叫端拿到的
// 「索引」是垃圾值。裝置上看到的
//   E cammw: cammw_link_tintless_srv_send_evt_with_buf: invalid arg
//            handle = ..., index = 8192
// 就是這樣來的。
extern "C" int cammw_util_bufmgr_lock_buf(cammw_util_buffer_manager_t *mgr, uint32_t index,
                                          uint32_t *out_index,
                                          cammw_util_image_buffer_t *out_buffer) {
  if (mgr == nullptr) {
    return CAMMW_ERR_INVALID_ARG;
  }

  pthread_mutex_lock(&mgr->buffer_mutex);

  const uint32_t count = mgr->buffer_count;
  int status = -0x68;
  uint32_t chosen = 0;
  bool found = false;

  if (out_index != nullptr) {
    // 自動配置：從游標開始繞一圈找空位。
    if (count != 0) {
      uint32_t cursor = mgr->alloc_cursor;
      for (uint32_t tried = 0; tried < count; ++tried) {
        uint32_t next = cursor + 1;
        if (next >= count) {
          next = 0;
        }
        mgr->alloc_cursor = next;
        if (mgr->lock_flags[cursor] == 0) {
          chosen = cursor;
          found = true;
          *out_index = cursor;
          break;
        }
        cursor = next;
      }
    }
  } else {
    // 指定索引：必須在範圍內且尚未被鎖。
    if (index < count && mgr->lock_flags[index] == 0) {
      chosen = index;
      found = true;
    }
  }

  if (found) {
    if (out_buffer != nullptr) {
      *out_buffer = mgr->buffer_info[chosen];  // 整個 24-byte 描述子
    }
    mgr->lock_flags[chosen] = 1;
    status = 0;
  }

  pthread_mutex_unlock(&mgr->buffer_mutex);
  return status;
}

extern "C" int cammw_util_bufmgr_unlock_buf(cammw_util_buffer_manager_t *mgr, uint32_t index) {
  if (mgr == nullptr) {
    return CAMMW_ERR_INVALID_ARG;
  }

  pthread_mutex_lock(&mgr->buffer_mutex);
  int status;
  if (index < mgr->buffer_count) {
    mgr->lock_flags[index] = 0;
    status = 0;
  } else {
    status = CAMMW_ERR_INVALID_ARG;
  }
  pthread_mutex_unlock(&mgr->buffer_mutex);
  return status;
}

extern "C" int cammw_util_bufmgr_deinit(cammw_util_buffer_manager_t *mgr) {
  if (mgr == nullptr) {
    return CAMMW_ERR_INVALID_ARG;
  }

  pthread_mutex_lock(&mgr->buffer_mutex);
  for (uint32_t i = 0; i < mgr->buffer_count; ++i) {
    cammw_util_shmem_free_buf(&mgr->buffer_info[i]);
  }
  mgr->buffer_count = 0;
  pthread_mutex_unlock(&mgr->buffer_mutex);
  pthread_mutex_destroy(&mgr->buffer_mutex);
  return 0;
}
