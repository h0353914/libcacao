// cammw_util_gralloc_alloc 重建 —— 對照 kong 這輪 decompiled.c @ 0x13708
// （97% 信心度）。這個函式密度很高（格式對照表 + gralloc1 usage flag
// 組裝 + 完整 allocate pipeline），這輪採取「盡量貼著反編譯逐行轉譯，
// 不強行取語意名字」的策略，之後靠裝置端 test_camera.py + bisection
// 去抓殘餘的轉譯誤差（這本來就是這個專案從頭到尾驗證正確性的方式）。

#define LOG_TAG "cammw_util"

#include "cammw_buf.h"
#include "cammw_util_internal.h"

#include <hardware/gralloc1.h>
#include <log/log.h>
#include <stdint.h>

extern "C" int cammw_util_get_gralloc1_dev(gralloc1_device_t **out_device);
extern "C" int cammw_util_gralloc_make_buf_from_private_handle(
    const void *gralloc_device, const int *attribute_values, int width, int height,
    int pixel_format, uint32_t usage_or_color_space, const void *handle, uint8_t populate_metadata,
    cammw_buf_t *out);
// 這兩個是 C++ linkage（不是 extern "C"）—— 為了 symbol interposition
// 要 mangle 成跟 libcammw.so 一樣的名字，見 gralloc_descriptor.cpp 開頭
// 的說明。這裡宣告也不能加 extern "C"，否則連結器找的是沒 mangle 過的
// 名字，會找不到符號。
int cammw_util_gralloc1_add_descriptor_list(int fd, unsigned long long descriptor);

extern "C" void *g_perform_func;  // GRALLOC1_FUNCTION_PERFORM(0x1000)，可變參數
extern "C" GRALLOC1_PFN_ALLOCATE g_allocate_func;
extern "C" GRALLOC1_PFN_CREATE_DESCRIPTOR g_create_descriptor_func;
extern "C" GRALLOC1_PFN_DESTROY_DESCRIPTOR g_destroy_descriptor_func;
extern "C" GRALLOC1_PFN_RELEASE g_release_func;
extern "C" GRALLOC1_PFN_SET_CONSUMER_USAGE g_set_consumer_usage_func;
extern "C" GRALLOC1_PFN_SET_DIMENSIONS g_set_dimensions_func;
extern "C" GRALLOC1_PFN_SET_FORMAT g_set_format_func;
extern "C" GRALLOC1_PFN_SET_PRODUCER_USAGE g_set_producer_usage_func;

extern "C" int cammw_util_gralloc_alloc(int buf_type, uint32_t usage, uint32_t width, int height,
                                        uint32_t pixel_format, uint32_t color_space,
                                        uint8_t populate_metadata, cammw_buf_t *out) {
  buffer_handle_t handle = nullptr;
  gralloc1_buffer_descriptor_t descriptor = static_cast<gralloc1_buffer_descriptor_t>(-1);
  gralloc1_device_t *device = nullptr;
  int rc;

  if (color_space >= 4 || height == 0 || width == 0 || out == nullptr) {
    ALOGE("E: %s: Invalid Arg", __FUNCTION__);
    return -0x67;
  }

  out->w[kCammwBufBase] = 0;
  out->w[kCammwBufSize] = 0;
  out->w[kCammwBufFd] = -1;
  out->w[3] = 0;
  out->w[4] = 4;
  reinterpret_cast<uint8_t *>(&out->w[5])[0] = 1;

  // -------- 格式對照表：cammw 的 pixel_format(param_5) -> gralloc HAL 格式 --------
  int32_t gralloc_format;
  uint32_t alloc_width = 0;
  uint32_t alloc_height = 0;
  bool unsupported = false;

  if (pixel_format < 0x800000) {
    if (pixel_format < 0x40001) {
      if (pixel_format < 0x20000) {
        if (pixel_format == 0) {
          gralloc_format = 0x21;
          alloc_height = 1;
          alloc_width = width;
          goto usage_flags;
        }
        if (pixel_format == 0x10000) {
          unsupported = true;
          goto cleanup;
        }
        if (pixel_format != 0x10001) goto invalid;
        gralloc_format = 0x32315659;  // 'YV12'
      } else if (pixel_format == 0x20000) {
        gralloc_format = 0x109;
      } else if (pixel_format == 0x20001) {
        gralloc_format = 0x11;
      } else if (pixel_format == 0x40000) {
        gralloc_format = 0x10;
      } else {
        goto invalid;
      }
    } else if (pixel_format - 0x100000 < 4) {
      gralloc_format = 0x25;
    } else if (pixel_format == 0x40001) {
      gralloc_format = 0x10b;
    } else if (pixel_format == 0x80000) {
      alloc_width = ((height + 1) & ~1u) * ((width + 0x1f) & ~0x1fu);
      alloc_height = 1;
      gralloc_format = 0x21;
      goto usage_flags;
    } else {
      goto invalid;
    }
    alloc_width = (width + 1) & ~1u;
    alloc_height = (height + 1) & ~1u;
  } else {
    if (pixel_format < 0x2000000) {
      if (pixel_format - 0x1000001 < 2) {
        alloc_width = height * width * 2 + 0x10000;
      } else if (pixel_format == 0x800000) {
        gralloc_format = 0x20;
        alloc_width = (width + 1) & ~1u;
        alloc_height = (height + 1) & ~1u;
        goto usage_flags;
      } else if (pixel_format == 0x1000000) {
        alloc_width = height * width * 2;
      } else {
        goto invalid;
      }
    } else {
      if (pixel_format - 0x4000001 < 3) {
        unsupported = true;
        goto cleanup;
      }
      if (pixel_format != 0x2000000) {
        if (pixel_format != 0x4000000) goto invalid;
        gralloc_format = 3;
        alloc_width = (width + 1) & ~1u;
        alloc_height = (height + 1) & ~1u;
        goto usage_flags;
      }
      alloc_width = 0x5001c;
    }
    alloc_height = 1;
    gralloc_format = 0x21;
  }

usage_flags: {
  // -------- gralloc1 producer/consumer usage flags --------
  // 原版（decompiled.c @LAB_0001392c）：
  //   uVar2 = 0; uVar4 = 0x80000000;
  //   if (param_1 == 2) { uVar4 = 0; uVar2 = 0x40000000; }
  //   uVar2 |= (param_2 & 0x1000) << 0x11;
  //   uVar7 = uVar2 | 0x66;                    // (param_2 & 1) 時改成 |0x20066
  // 之後 perform / set_producer_usage 拿到的是 64-bit：低位 = uVar7、
  // **高位 = uVar4（0x80000000）**。之前這裡只傳低位，等於把高位那個
  // vendor 私有旗標整個丟掉，gralloc 會配出不同版面的 buffer。
  uint32_t consumer_usage = 0;
  uint32_t producer_usage_hi = 0x80000000u;
  if (buf_type == 2) {
    producer_usage_hi = 0;
    consumer_usage = 0x40000000u;
  }
  consumer_usage |= (usage & 0x1000) << 0x11;
  uint32_t producer_usage_lo = consumer_usage | 0x66u;

  if ((usage & 1) != 0 && pixel_format != 0) {
    if ((pixel_format & 0xf0000) == 0) {
      unsupported = true;
      goto cleanup;
    }
    producer_usage_lo = consumer_usage | 0x20066u;
  }

  bool bad_bit_usage = ((pixel_format & 0xf0000) == 0) && (usage & 0x40) != 0 && pixel_format != 0;
  bool bad_bit_usage2 = ((pixel_format & 0xf0000) == 0) && (usage & 2) != 0 && pixel_format != 0;
  if (bad_bit_usage || bad_bit_usage2) {
    unsupported = true;
    goto cleanup;
  }

  if (static_cast<int32_t>(usage << 0x1b) < 0 && pixel_format != 0) {
    if ((pixel_format & 0x70000) == 0) {
      if ((pixel_format & 0x80000) == 0) {
        unsupported = true;
        goto cleanup;
      }
      alloc_width = ((height + 0xf) & ~0xfu) * ((width + 0x1f) & ~0x1fu);
    } else {
      alloc_height = (alloc_height + 0xf) & ~0xfu;
      alloc_width = (alloc_width + 0xf) & ~0xfu;
    }
  }
  if (static_cast<int32_t>(usage << 0x1a) < 0 && pixel_format != 0) {
    if ((pixel_format & 0x70000) == 0) {
      if ((pixel_format & 0x80000) == 0) {
        unsupported = true;
        goto cleanup;
      }
      alloc_width = ((height + 0xf) & ~0xfu) * ((width + 0x1f) & ~0x1fu);
    } else {
      alloc_height = (alloc_height + 0xf) & ~0xfu;
      alloc_width = (alloc_width + 0xf) & ~0xfu;
    }
  }
  if (static_cast<int32_t>(usage << 0x1d) < 0 && pixel_format != 0) {
    if (pixel_format != 0x20000) {
      unsupported = true;
      goto cleanup;
    }
    gralloc_format = 0x102;
  }

  rc = cammw_util_get_gralloc1_dev(&device);
  if (rc != 0) {
    ALOGE("E: %s: failed to get gralloc1 device", __FUNCTION__);
    goto cleanup;
  }

  // 原版先用 PERFORM opcode 5（qcom 的 GRALLOC_MODULE_PERFORM_GET_ATTRIBUTES）
  // 去問 gralloc「這組 w/h/format/usage 對齊後應該是多少」，再把回填的
  // aligned_width / aligned_height 拿去 set_dimensions：
  //
  //   puVar13 = &local_34;  piVar14 = &local_2c;  puVar12 = &local_30;
  //   perform(dev, 5, local_30, local_34, uVar9,
  //           <producer_usage 64: uVar7,uVar4>, <consumer_usage 64: 0,0>,
  //           puVar12, puVar13, piVar14);
  //   ... set_dimensions(dev, descriptor, local_30, local_34)
  //
  // （Ghidra 把可變參數的堆疊引數誤掛到後面那個 set_dimensions 上，實際上
  //  那串是 perform 的。）之前這裡整段跳過、直接拿未對齊的寬高去配置，
  // 造成 chroma plane 位置算錯——拍出來整張全綠。
  {
    int32_t aligned_w = static_cast<int32_t>(alloc_width);
    int32_t aligned_h = static_cast<int32_t>(alloc_height);
    int32_t tile_enabled = 0;
    using PerformFn = int32_t (*)(gralloc1_device_t *, int32_t, ...);
    auto perform = reinterpret_cast<PerformFn>(g_perform_func);
    if (perform == nullptr) {
      ALOGE("E: %s: no perform func", __FUNCTION__);
      rc = -0x6f;
      goto cleanup;
    }
    rc = perform(device, 5, static_cast<int32_t>(alloc_width), static_cast<int32_t>(alloc_height),
                 gralloc_format,
                 (static_cast<uint64_t>(producer_usage_hi) << 32) | producer_usage_lo,
                 static_cast<uint64_t>(0), &aligned_w, &aligned_h, &tile_enabled);
    if (rc != 0) {
      ALOGE("E: %s: perform(GET_ATTRIBUTES) failed, err %d", __FUNCTION__, rc);
      goto cleanup;
    }
    alloc_width = static_cast<uint32_t>(aligned_w);
    alloc_height = static_cast<uint32_t>(aligned_h);
  }

  if (rc == 0) {
    rc = g_create_descriptor_func(device, &descriptor);
  }
  if (rc == 0) {
    rc = g_set_format_func(device, descriptor, gralloc_format);
  }
  if (rc == 0) {
    rc = g_set_dimensions_func(device, descriptor, alloc_width, alloc_height);
  }
  if (rc == 0) {
    rc = g_set_consumer_usage_func(device, descriptor, consumer_usage);
  }
  if (rc == 0) {
    // 64-bit：高位是 uVar4（0x80000000 的 vendor 私有旗標），不能只傳低位。
    rc = g_set_producer_usage_func(
        device, descriptor, (static_cast<uint64_t>(producer_usage_hi) << 32) | producer_usage_lo);
  }
  if (rc != 0) {
    ALOGE("E: %s: descriptor setup failed, err %d", __FUNCTION__, rc);
    rc = -0x6f;
    goto cleanup;
  }

  rc = g_allocate_func(device, 1, &descriptor, &handle);
  if (rc != 0) {
    ALOGE("E: %s: allocate failed", __FUNCTION__);
    rc = -0x6f;
    goto cleanup;
  }

  rc = cammw_util_gralloc_make_buf_from_private_handle(device, reinterpret_cast<const int *>(buf_type),
                                                        width, height, pixel_format, color_space,
                                                        handle, populate_metadata, out);
  if (rc != 0) {
    goto cleanup;
  }
  rc = cammw_util_gralloc1_add_descriptor_list(out->w[kCammwBufFd],
                                               static_cast<unsigned long long>(descriptor));
  if (rc == 0) {
    // 成功路徑「不」走 cleanup：handle 已經存進 out->w[kCammwBufPrivHandle]，
    // descriptor 也已登記到 descriptor list，兩者的所有權都轉移給呼叫端了，
    // 之後由 cammw_util_shmem_free_buf 負責 del_descriptor_list /
    // g_destroy_descriptor_func / g_release_func。
    //
    // 原版就是這樣：成功時 goto LAB_000137f8（只有 stack check + return），
    // 完全不碰 release/destroy。之前這裡直接落入下面的 cleanup，等於把剛
    // 配好的 buffer 立刻釋放掉，out->w[3] 變成 dangling pointer——實機在
    // free_buf → g_unlock_func → private_handle_t::validate 以 SIGSEGV
    // （fault addr 0xdb22e000，典型 use-after-free）收場。
    return 0;
  }
}

cleanup:
  if (handle != nullptr) {
    g_release_func(device, handle);
  }
  if (descriptor != static_cast<gralloc1_buffer_descriptor_t>(-1)) {
    g_destroy_descriptor_func(device, descriptor);
  }
  if (unsupported) {
    return -0x6a;
  }
  return rc;

invalid:
  ALOGE("E: %s: Invalid Arg", __FUNCTION__);
  return -0x67;
}
