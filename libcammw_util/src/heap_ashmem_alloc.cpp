// cammw_util_heap_alloc（@0x13f90，95% 信心）與 cammw_util_ashmem_alloc
// （@0x13360，96% 信心）——這兩個函式的格式對照表**看起來很像但不是
// 完全一樣**（heap_alloc 對 0x100000 只精確比對單一值就回
// unsupported、其餘 0x100001~3 回 invalid；ashmem_alloc 是整個
// [0x100000,0x100004) 範圍都回 unsupported）——已經對照過一次踩到這個
// 差異，所以兩個函式各自照自己的反編譯獨立轉譯，不共用一份表。

#define LOG_TAG "cammw_util"

#include "cammw_buf.h"
#include "cammw_util_internal.h"

#include <cutils/ashmem.h>
#include <log/log.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

extern "C" int cammw_util_mmap(int fd, uint32_t size, uint8_t **out_addr);

// 對照 decompiled.c @ 0x13f90，逐行轉譯。
extern "C" int cammw_util_heap_alloc(uint32_t width, uint32_t height, int32_t format,
                                     uint32_t color_space, uint8_t populate_metadata,
                                     cammw_buf_t *out) {
  if (color_space >= 4 || height == 0) {
    ALOGE("E: %s: Invalid Arg", __FUNCTION__);
    return 0xffffff99;
  }
  if (width == 0 || out == nullptr) {
    ALOGE("E: %s: Invalid Arg", __FUNCTION__);
    return 0xffffff99;
  }

  out->w[0] = 0;
  out->w[1] = 0;
  out->w[2] = -1;
  out->w[3] = 0;
  out->w[4] = 4;
  reinterpret_cast<uint8_t *>(&out->w[5])[0] = 1;

  uint32_t size;
  uint32_t stride = width;
  uint32_t vstride = 1;

  if (format < 0x800000) {
    if (format < 0x40000) {
      if (format < 0x20000) {
        size = width;
        stride = width;
        if (format != 0) {
          if (format == 0x10000) return 0xffffff96;
          if (format != 0x10001) return 0xffffff99;
          stride = (width + 0xf) & ~0xfu;
          vstride = (height + 1) & ~1u;
          size = (((stride >> 1) + 0xf) & ~0xfu) + stride;
          size *= vstride;
        }
      } else {
        if (format - 0x20000u > 1) return 0xffffff99;
        stride = (width + 1) & ~1u;
        vstride = (height + 1) & ~1u;
        uint32_t p = vstride * stride;
        size = p + (p >> 1);
      }
    } else if (format - 0x40000u < 2) {
      vstride = (height + 1) & ~1u;
      stride = (width + 1) & ~1u;
      size = stride * vstride * 2;
    } else {
      if (format != 0x80000) {
        if (format != 0x100000) return 0xffffff99;
        return 0xffffff96;
      }
      stride = (width + 1) & ~1u;
      vstride = (height + 1) & ~1u;
      size = vstride * stride;
    }
  } else {
    if (format < 0x2000000) {
      if (format - 0x1000001u < 2) {
        size = height * width * 2 + 0x10000;
      } else {
        if (format == 0x800000) return 0xffffff96;
        if (format != 0x1000000) return 0xffffff99;
        size = height * width * 2;
      }
    } else {
      if (format - 0x4000001u < 3) return 0xffffff96;
      if (format != 0x2000000) {
        if (format != 0x4000000) return 0xffffff99;
        vstride = (height + 1) & ~1u;
        stride = (width + 1) & ~1u;
        size = stride * vstride * 3;
        goto do_malloc;
      }
      size = 0x5001c;
    }
    vstride = 1;
    stride = size;
  }

do_malloc:
  void *mem = malloc(size);
  out->w[0] = reinterpret_cast<intptr_t>(mem);
  if (mem == nullptr) {
    return 0xffffff9a;
  }
  out->w[1] = static_cast<int32_t>(size);
  out->w[2] = -1;
  out->w[3] = -1;
  out->w[4] = 3;
  if (populate_metadata == 0) {
    return 0;
  }

  out->w[kCammwBufPixelFormat] = format;
  out->w[kCammwBufWidth] = static_cast<int32_t>(width);
  out->w[kCammwBufHeight] = static_cast<int32_t>(height);
  out->w[kCammwBufOffset] = 0;
  out->w[kCammwBufPlaneOffset1] = 0;
  out->w[kCammwBufPlaneOffset2] = 0;
  out->w[kCammwBufStride] = static_cast<int32_t>(stride);
  out->w[kCammwBufVStride] = static_cast<int32_t>(vstride);
  out->w[kCammwBufUsageOrColorSpace] = static_cast<int32_t>(color_space);
  out->w[0xf] = 0;
  out->w[0x10] = 0;
  out->w[kCammwBufWidth2] = static_cast<int32_t>(width);
  out->w[kCammwBufHeight2] = static_cast<int32_t>(height);
  out->w[kCammwBufImageSize] = static_cast<int32_t>(size);

  int32_t plane_calc;
  if (format < 0x40000) {
    if (format - 0x20000u < 2) {
      uint32_t p = vstride * stride;
      out->w[kCammwBufPlaneOffset1] = static_cast<int32_t>(p);
      plane_calc = static_cast<int32_t>(p + (p >> 1));
    } else {
      if (format != 0x10001) return 0;
      const uint32_t offset = static_cast<uint32_t>(out->w[kCammwBufOffset]);
      const uint32_t p1 = vstride * stride + offset;
      const uint32_t p2 = vstride * (((stride >> 1) + 0xf) & ~0xfu);
      plane_calc = static_cast<int32_t>(p1 + (p2 >> 1));
      out->w[kCammwBufPlaneOffset1] = plane_calc;
      out->w[kCammwBufPlaneOffset2] = static_cast<int32_t>(p1);
      plane_calc = static_cast<int32_t>((p2 >> 1) - offset) + plane_calc;
    }
  } else if (format - 0x40000u < 2) {
    out->w[kCammwBufPlaneOffset1] = static_cast<int32_t>(vstride * stride);
    plane_calc = static_cast<int32_t>(vstride * stride * 2);
  } else {
    if (format != 0x80000) return 0;
    plane_calc = static_cast<int32_t>(vstride * stride);
  }
  out->w[kCammwBufImageSize] = plane_calc;
  return 0;
}

// 對照 decompiled.c @ 0x13360，逐行轉譯（跟 heap_alloc 結構很像但
// unsupported/invalid 的邊界不完全一樣，見檔案開頭註解）。
extern "C" int cammw_util_ashmem_alloc(uint32_t width, uint32_t height, int32_t format,
                                       uint32_t color_space, uint8_t populate_metadata,
                                       cammw_buf_t *out) {
  if (color_space >= 4 || height == 0 || width == 0 || out == nullptr) {
    ALOGE("E: %s: Invalid Arg", __FUNCTION__);
    return 0xffffff99;
  }

  out->w[0] = 0;
  out->w[1] = 0;
  out->w[2] = -1;
  out->w[3] = 0;
  out->w[4] = 4;
  reinterpret_cast<uint8_t *>(&out->w[5])[0] = 1;

  uint32_t size = width;
  uint32_t stride = width;
  uint32_t vstride = 1;
  bool unsupported_no_alloc = false;

  if (format < 0x800000) {
    if (format < 0x40000) {
      if (format < 0x20000) {
        if (format != 0) {
          if (format == 0x10000) {
            unsupported_no_alloc = true;
          } else if (format != 0x10001) {
            return 0xffffff99;
          } else {
            stride = (width + 0xf) & ~0xfu;
            vstride = (height + 1) & ~1u;
            size = (((stride >> 1) + 0xf) & ~0xfu) + stride;
            size *= vstride;
          }
        }
      } else {
        if (format - 0x20000u > 1) return 0xffffff99;
        stride = (width + 1) & ~1u;
        vstride = (height + 1) & ~1u;
        uint32_t p = vstride * stride;
        size = p + (p >> 1);
      }
    } else if (format - 0x100000u < 4) {
      unsupported_no_alloc = true;
    } else if (format - 0x40000u < 2) {
      vstride = (height + 1) & ~1u;
      stride = (width + 1) & ~1u;
      size = stride * vstride * 2;
    } else {
      if (format != 0x80000) return 0xffffff99;
      stride = (width + 1) & ~1u;
      vstride = (height + 1) & ~1u;
      size = vstride * stride;
    }
  } else {
    if (format < 0x2000000) {
      if (format - 0x1000001u < 2) {
        size = height * width * 2 + 0x10000;
      } else if (format == 0x800000) {
        unsupported_no_alloc = true;
      } else {
        if (format != 0x1000000) return 0xffffff99;
        size = height * width * 2;
      }
    } else {
      if (format - 0x4000001u < 3) {
        unsupported_no_alloc = true;
      } else if (format != 0x2000000) {
        if (format != 0x4000000) return 0xffffff99;
        vstride = (height + 1) & ~1u;
        stride = (width + 1) & ~1u;
        size = stride * vstride * 3;
        goto do_alloc;
      } else {
        size = 0x5001c;
      }
      if (!unsupported_no_alloc) {
        vstride = 1;
        stride = size;
      }
    }
  }

do_alloc:
  if (unsupported_no_alloc) {
    // 原版：不配置任何東西，直接以「呼叫成功但沒有 buffer」收尾——
    // 呼叫端要另外用 -0x6a（UNSUPPORTED）判斷這種情況。這裡忠實照抄
    // 原版行為：iVar5 一路帶著 -0x6a 走到最後的 return。
    return 0xffffff96;
  }

  {
    const size_t page = getpagesize();
    const uint32_t region_size = static_cast<uint32_t>((page + size - 1) & ~(page - 1));
    int fd = ashmem_create_region("cammw:ashmem", region_size);
    if (fd < 0) {
      ALOGE("E: %s: Failed to create region %u", __FUNCTION__, region_size);
      return 0xffffff97;
    }
    if (ashmem_set_prot_region(fd, PROT_READ | PROT_WRITE) < 0) {
      ALOGE("E: %s: Failed ashmem_set_prot_region", __FUNCTION__);
      close(fd);
      return 0xffffff97;
    }
    uint8_t *mapped = nullptr;
    if (cammw_util_mmap(fd, region_size, &mapped) != 0) {
      close(fd);
      return 0xffffff91;
    }
    out->w[0] = reinterpret_cast<intptr_t>(mapped);
    out->w[1] = static_cast<int32_t>(region_size);
    out->w[2] = fd;
    out->w[3] = -1;
    out->w[4] = 0;
    if (populate_metadata == 0) {
      return 0;
    }

    out->w[kCammwBufPixelFormat] = format;
    out->w[kCammwBufWidth] = static_cast<int32_t>(width);
    out->w[kCammwBufHeight] = static_cast<int32_t>(height);
    out->w[9] = 0;
    out->w[10] = 0;
    out->w[kCammwBufPlaneOffset2] = 0;
    out->w[kCammwBufStride] = static_cast<int32_t>(stride);
    out->w[kCammwBufVStride] = static_cast<int32_t>(vstride);
    out->w[kCammwBufUsageOrColorSpace] = static_cast<int32_t>(color_space);
    out->w[0xf] = 0;
    out->w[0x10] = 0;
    out->w[kCammwBufWidth2] = static_cast<int32_t>(width);
    out->w[kCammwBufHeight2] = static_cast<int32_t>(height);
    out->w[kCammwBufImageSize] = static_cast<int32_t>(region_size);

    if (format < 0x40000) {
      if (format - 0x20000u < 2) {
        uint32_t p = vstride * stride;
        out->w[kCammwBufPlaneOffset1] = static_cast<int32_t>(p);
        out->w[kCammwBufImageSize] = static_cast<int32_t>(p + (p >> 1));
      } else {
        if (format != 0x10001) return 0;
        // decompiled.c @ ~0x133cc：p1 已經把 offset(這裡固定是 0) 算進去，
        // plane_offset_1 = p1 + p2/2，plane_offset_2 = p1，
        // 總大小 = (p2>>1 - offset) + plane_offset_1。
        const uint32_t offset = static_cast<uint32_t>(out->w[kCammwBufOffset]);
        const uint32_t p1 = vstride * stride + offset;
        const uint32_t p2 = vstride * (((stride >> 1) + 0xf) & ~0xfu);
        const int32_t plane1 = static_cast<int32_t>(p1 + (p2 >> 1));
        out->w[kCammwBufPlaneOffset1] = plane1;
        out->w[kCammwBufPlaneOffset2] = static_cast<int32_t>(p1);
        out->w[kCammwBufImageSize] = static_cast<int32_t>((p2 >> 1) - offset) + plane1;
      }
    } else if (format - 0x40000u < 2) {
      out->w[kCammwBufPlaneOffset1] = static_cast<int32_t>(vstride * stride);
      out->w[kCammwBufImageSize] = static_cast<int32_t>(vstride * stride * 2);
    } else {
      if (format != 0x80000) return 0;
      out->w[kCammwBufImageSize] = static_cast<int32_t>(vstride * stride);
    }
    return 0;
  }
}
