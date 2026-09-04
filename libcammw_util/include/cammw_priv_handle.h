// gralloc private_handle_t 的最小相容視圖。
//
// 不直接 #include display HAL 的 gr_priv_handle.h（那邊有一堆跟這裡無關
// 的相依），自己開一個「只到我們用得到的欄位」的版本，靠 static_assert
// 鎖住跟真正 private_handle_t（hardware/qcom-caf/msm8998/display/
// gralloc/gr_priv_handle.h）逐欄位比對過的 offset。
//
// native_handle_t 開頭 12 bytes（version, numFds, numInts），private_handle_t
// 自己的欄位接在後面。cammw_util_gralloc_make_buf_from_private_handle
// 用到的幾個 offset 都驗證過：
//   +0x0c fd, +0x1c width, +0x20 height, +0x2c format, +0x34 size,
//   +0x38 offset, +0x40 base（uint64_t，這裡只取低 32 位，32-bit target）

#pragma once

#include <stdint.h>

struct CammwGrallocHandleFields {
  int32_t version;         // +0x00
  int32_t numFds;          // +0x04
  int32_t numInts;         // +0x08
  int32_t fd;               // +0x0c
  int32_t fd_metadata;      // +0x10
  int32_t magic;             // +0x14
  int32_t flags;             // +0x18
  int32_t width;              // +0x1c
  int32_t height;             // +0x20
  int32_t unaligned_width;    // +0x24
  int32_t unaligned_height;   // +0x28
  int32_t format;              // +0x2c
  int32_t buffer_type;         // +0x30
  uint32_t size;                // +0x34
  uint32_t offset;               // +0x38
  uint32_t offset_metadata;      // +0x3c
  uint64_t base;                  // +0x40 (8-byte aligned)
};

static_assert(offsetof(CammwGrallocHandleFields, fd) == 0x0c, "fd");
static_assert(offsetof(CammwGrallocHandleFields, width) == 0x1c, "width");
static_assert(offsetof(CammwGrallocHandleFields, height) == 0x20, "height");
static_assert(offsetof(CammwGrallocHandleFields, format) == 0x2c, "format");
static_assert(offsetof(CammwGrallocHandleFields, size) == 0x34, "size");
static_assert(offsetof(CammwGrallocHandleFields, offset) == 0x38, "offset");
static_assert(offsetof(CammwGrallocHandleFields, base) == 0x40, "base");

// gralloc1.h 需要的 kMagic/kNumFds 常數，跟真正 private_handle_t 一致。
constexpr int32_t kCammwGrallocMagic = 'gmsm';
constexpr int kCammwGrallocNumFds = 2;

// 本機 gralloc 的 private_handle_t::NumInts()。
//   Sony A9（gr_priv_handle.h 無 pack）：sizeof=120 → 25
//   LineageOS / CAF 2019（有 #pragma pack(push,4)）：sizeof=116 → 24
// 我們建的 handle 要填這個值，retain 才會被 gralloc 接受。
constexpr int kGrallocNumInts = 24;
