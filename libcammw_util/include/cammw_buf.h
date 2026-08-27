// cammw_buf_t —— gralloc/ashmem/heap 配置路徑共用的「豐富」影像描述子。
//
// 跟 bufmgr.cpp 用的 cammw_util_image_buffer_t（24 bytes，6 words）是
// **不同的結構**：這個是 104 bytes（kong 這輪合成的 'cammw_buf_t' 也是
// 104 bytes/21 fields），gralloc_alloc/ashmem_alloc/heap_alloc/
// gralloc_make_buf_from_private_handle 都在填這個。
//
// 每個 word 的語意是從 cammw_util_gralloc_make_buf_from_private_handle
// （decompiled.c @ 0x14b50）逐行推回來的：
//   param_9[0]  = (int)private_handle->base   （只留低 32 位，32-bit target）
//   param_9[1]  = private_handle->size
//   param_9[2]  = private_handle->fd
//   param_9[3]  = private_handle 指標本身
//   param_9[4]  = 呼叫端傳進來的 attribute_values 指標
//   param_9[5]  = （沒被這個函式碰過，保留）
//   param_9[6]  = pixel_format（呼叫端傳的，不是 private_handle 裡的）
//   param_9[7]  = 呼叫時的 width 參數
//   param_9[8]  = 呼叫時的 height 參數
//   param_9[9]  = private_handle->offset
//   param_9[0xa]= 依格式而定的第一個 plane 偏移（各分支自己算）
//   param_9[0xb]= 依格式而定的第二個 plane 偏移
//   param_9[0xc]= stride（初值 = private_handle->width，部分格式改算）
//   param_9[0xd]= v_stride/plane 高度（初值 = private_handle->height，部分格式改算）
//   param_9[0xe]= 呼叫端傳的 usage/color_space 參數
//   param_9[0xf], [0x10] = 0（保留）
//   param_9[0x11] = width 參數（跟 [7] 重複存一次）
//   param_9[0x12] = height 參數（跟 [8] 重複存一次）
//   param_9[0x13] = 依格式而定的總影像大小
//   param_9[0x14..0x19] = 0（三個 undefined8 clear，24 bytes 尾端保留）
//
// ashmem_alloc/heap_alloc 用同一顆結構但从 0 開始配置（不是包 private
// handle），欄位語意大致對得上（size/fd/mmap base/format/width/height/
// stride/plane offsets），個別函式裡再註記差異。
//
// 用 word 陣列 + 具名索引常數表示，不硬取一堆可能猜錯的欄位名字——
// 這是刻意的：這顆結構牽涉的 format-specific 分支非常多，寧可讓存取方式
// 保持跟反編譯 1:1 對應，之後再視情況個別包裝成具名 helper。

#pragma once

#include <stdint.h>

struct cammw_buf_t {
  int32_t w[26];  // 26 * 4 = 104 bytes
};
static_assert(sizeof(cammw_buf_t) == 104, "kong 合成的 cammw_buf_t 大小");

enum cammw_buf_word : int {
  kCammwBufBase = 0,
  kCammwBufSize = 1,
  kCammwBufFd = 2,
  kCammwBufPrivHandle = 3,
  kCammwBufAttrPtr = 4,
  kCammwBufPixelFormat = 6,
  kCammwBufWidth = 7,
  kCammwBufHeight = 8,
  kCammwBufOffset = 9,
  kCammwBufPlaneOffset1 = 0xa,
  kCammwBufPlaneOffset2 = 0xb,
  kCammwBufStride = 0xc,
  kCammwBufVStride = 0xd,
  kCammwBufUsageOrColorSpace = 0xe,
  kCammwBufWidth2 = 0x11,
  kCammwBufHeight2 = 0x12,
  kCammwBufImageSize = 0x13,
};
