// cammw_util 的 gralloc descriptor 註冊表重建
//
// 參考：../kong-output/libcammw_util_v12/decompiled.c
//   cammw_util_gralloc1_add_descriptor_list @ 0x12428
//   cammw_util_gralloc1_del_descriptor_list @ 0x124f4
//
// **這兩個函式的名字與連結方式極為關鍵，先前弄錯導致相機打不開。**
//
// 它們在原廠是 **C++ linkage**（不是 extern "C"），mangled name 分別是
//   _Z39cammw_util_gralloc1_add_descriptor_listiy    (int, unsigned long long)
//   _Z39cammw_util_gralloc1_del_descriptor_listiPy   (int, unsigned long long *)
//
// 而且 —— 這是關鍵 —— **libcammw.so 自己也定義了同名的這兩個符號**：
//   libcammw.so       0003a2e4 T _Z39cammw_util_gralloc1_add_descriptor_listiy
//   libcammw_util.so  00002428 T _Z39cammw_util_gralloc1_add_descriptor_listiy
// 這是刻意的 **symbol interposition**：動態連結器讓兩邊的呼叫都綁到同一份
// 實作，於是兩個函式庫共用**同一份 descriptor list 全域狀態**。
//
// 先前我把它們取名成 cammw_util_gralloc_register/unregister_descriptor_handle
// 並用 extern "C"，又用 --version-script 只匯出 35 個純 C 名字，等於把這個
// 共用機制整個切斷：libcammw.so 退回用自己的副本，兩邊各自維護一份清單，
// 一邊註冊的 buffer 另一邊查不到，於是相機開啟流程卡住直到逾時。
//   cammw_util_build_buffer_descriptor_from_gralloc_handle @ 0x14b50
//
// 這組維護一張「gralloc descriptor -> buffer handle」的對照表，讓
// cammw 之後能用 descriptor 找回原本的 handle 與 mapping 大小。
//
// 反編譯露出的關鍵：
//   - 表本身就是 context 裡那三個內嵌 C++ 物件之一 —— register 用
//     `context + 4` 當 PtrLockList（`(**(code **)(*mutex + 8))(mutex)`
//     是透過 vtable 呼叫 lock），對應 context.cpp 註解裡的
//     ptr_lock_list_vt@+0x04。
//   - 每筆 entry 是 calloc(1, 0x10) = 16 bytes。
//   - unregister 走 PtrLockList::pop，取出後把 handle 與 mapped size
//     寫回呼叫端。

#define LOG_TAG "cammw_util"

#include "cammw_buf.h"
#include <gr_priv_handle.h>
#include "cammw_ptrlist.h"
#include "cammw_util_internal.h"

#include <hardware/gralloc1.h>
#include <system/graphics.h>
#include <log/log.h>

#include <stdlib.h>
#include <string.h>

extern "C" cammw_util_context_t *cammw_util_get_context(void);
extern "C" GRALLOC1_PFN_LOCK cammw_util_gralloc_lock_fn(void);
extern "C" GRALLOC1_PFN_LOCK_FLEX cammw_util_gralloc_lock_flex_fn(void);

namespace {

// 註冊表的 entry。16 bytes（register 的 `calloc(1,0x10)`）。
// 欄位語意來自 unregister：它把 handle 與 mapped size 抄回輸出結構。
struct DescriptorEntry {
  int fd;                     // +0x00  查表的鍵（呼叫端傳進來的 output->fd）
  int reserved;               // +0x04  對齊用（下面的 u64 要 8-byte 對齊）
  unsigned long long descriptor;  // +0x08  gralloc1 的 64-bit descriptor
};

static_assert(sizeof(DescriptorEntry) == 0x10, "register 的 calloc(1, 0x10)");

// 全域註冊表。原版把它內嵌在 context +0x04（那三個 vtable 之一），
// 這裡用獨立的 C++ 物件 —— 重建版不需要複製內嵌佈局，只要行為等價。
cammw::PtrLockList g_descriptor_list;

}  // namespace

// 把一組 fd/descriptor 記進表裡。已經存在就不重複加。
//
// 原版流程：calloc(16) -> 鎖表（透過 vtable）-> 走訪找同 fd
//   -> 沒有就 append -> 解鎖
//
// 簽名照原廠：第一個參數是 **int fd**（呼叫端傳 output->fd），第二個是
// 64-bit 的 gralloc1 descriptor。刻意**不用** extern "C"，這樣才會 mangle
// 成 _Z39cammw_util_gralloc1_add_descriptor_listiy 跟 libcammw.so 對得上。
int cammw_util_gralloc1_add_descriptor_list(int fd, unsigned long long descriptor) {
  auto *entry = static_cast<DescriptorEntry *>(calloc(1, sizeof(DescriptorEntry)));
  if (entry == nullptr) {
    // 原版這裡回 -0x66（跟其他錯誤碼不同，專指配置失敗）。
    return -0x66;
  }

  entry->fd = fd;
  entry->descriptor = descriptor;

  // 已經註冊過就不重複加 —— 原版走訪整串比對 fd。
  for (uint32_t i = 0; i < g_descriptor_list.count(); ++i) {
    auto *existing = static_cast<DescriptorEntry *>(g_descriptor_list.get(i));
    if (existing != nullptr && existing->fd == fd) {
      free(entry);
      return 0;
    }
  }

  auto *node = static_cast<cammw::PtrListNode *>(calloc(1, sizeof(cammw::PtrListNode)));
  if (node == nullptr) {
    free(entry);
    return -0x66;
  }
  node->payload = entry;
  g_descriptor_list.pushLast(node);
  return 0;
}

// 從表裡依 fd 取出並移除，把 descriptor 抄回呼叫端。
// 同樣是 C++ linkage，mangle 成 _Z39cammw_util_gralloc1_del_descriptor_listiPy。
int cammw_util_gralloc1_del_descriptor_list(int fd, unsigned long long *out_descriptor) {
  cammw_util_context_t *context = cammw_util_get_context();
  if (context == nullptr) {
    ALOGE("%s: no util resource", __FUNCTION__);
    return CAMMW_ERR_INVALID_ARG;
  }

  for (uint32_t i = 0; i < g_descriptor_list.count(); ++i) {
    auto *node = g_descriptor_list.getNode(i);
    if (node == nullptr) {
      continue;
    }
    auto *entry = static_cast<DescriptorEntry *>(node->payload);
    if (entry == nullptr || entry->fd != fd) {
      continue;
    }

    if (out_descriptor != nullptr) {
      *out_descriptor = entry->descriptor;
    }
    g_descriptor_list.remove(node);
    free(entry);
    free(node);
    return 0;
  }

  // 找不到 —— 原版同樣視為錯誤。
  return CAMMW_ERR_INVALID_ARG;
}

// cammw_util_gralloc_make_buf_from_private_handle @ 0x14b50 —— 這輪逐行
// 對照 kong 的 decompiled.c 補完（先前遺失的那份是憑印象寫的，這次直接
// 從反編譯出的組合語言等級邏輯轉譯，沒有再靠記憶腦補語意）。
//
// 簽名（來自 kong 的簽名推斷 + 逐行核對實際存取）：
//   (gralloc1_device*, const int *attribute_values, int width, int height,
//    int pixel_format, uint32_t usage_or_color_space,
//    const private_handle_t *handle, uint8_t populate_metadata,
//    cammw_buf_t *out)
//
// lock 分兩條路，由 handle->format 決定：
//   (format | 4) == 0x25  ->  走 gralloc1_lock（RAW 類：0x21/0x25）
//   其餘                   ->  走 gralloc1_lock_flex，再走訪 3 個
//                              android_flex_plane entry（間隔 8 個 word）
//
// gralloc1_lock 的呼叫（9 個實參，因為 producerUsage/consumerUsage 各自
// 是 64-bit，在 32-bit ABI 上各占兩個暫存器/堆疊格）：
//   lock(device, handle, producerUsage=0x66, consumerUsage=0,
//        &rect{0,0,width,height}, &mapped_ptr, /*fence=*/-1)
//
// flex plane 走訪（跟舊筆記的結構性描述吻合，這輪重新核對過索引）：
//   entry 間隔 8 個 word；entry[-5]=top_left（平面起始位址）、
//   entry[-4]=component（1=Y, 2=Cb, 4=Cr）、entry[0]=v_increment。
//   component==1 -> y_top_left；      component==2 -> cb_top_left（連同
//   它的 v_increment 存進 piVar8）；  component==4 -> cr_top_left（同樣
//   把 v_increment 存進 piVar8，兩個分支共用同一個暫存變數，所以最後
//   piVar8 是「隨便一個 2/4 分支的 v_increment」——因為程式只用它乘上
//   stride 算總大小，兩個分支理論上該給同一個 stride，行為上不影響
//   結果，這裡照抄原本的變數重用方式，不強行拆成語意更乾淨但可能猜錯
//   的版本。
extern "C" int cammw_util_gralloc_make_buf_from_private_handle(
    const void *gralloc_device, const int *attribute_values, int width, int height,
    int pixel_format, uint32_t usage_or_color_space, const private_handle_t *handle,
    uint8_t populate_metadata, cammw_buf_t *out) {
  if (gralloc_device == nullptr || handle == nullptr || out == nullptr) {
    ALOGE("E: %s: Invalid Arg", "cammw_util_gralloc_make_buf_from_private_handle");
    return CAMMW_ERR_INVALID_ARG;
  }

  auto *device = const_cast<gralloc1_device_t *>(static_cast<const gralloc1_device_t *>(gralloc_device));
  auto gralloc_handle = reinterpret_cast<buffer_handle_t>(handle);
  constexpr uint64_t kProducerUsage = 0x66;

  int y_top_left = 0;
  int cb_top_left = 0;
  int cr_top_left = 0;
  int v_increment = 0;  // 兩個 2/4 分支共用（見上面註解）
  const int *attrs = attribute_values;

  int rc;
  if ((handle->format | 4) == 0x25) {
    void *mapped = nullptr;
    const gralloc1_rect_t rect = {0, 0, width, height};
    GRALLOC1_PFN_LOCK lock = cammw_util_gralloc_lock_fn();
    rc = lock == nullptr ? GRALLOC1_ERROR_UNSUPPORTED
                          : lock(device, gralloc_handle, kProducerUsage, 0, &rect, &mapped, -1);
    if (rc != GRALLOC1_ERROR_NONE) {
      ALOGE("E: %s: gralloc1_lock failed, err %d",
            "cammw_util_gralloc_make_buf_from_private_handle", rc);
      return CAMMW_ERR_FAILED;
    }
  } else {
    // gralloc 的 BufferManager::GetFlexLayout() 會無條件往 layout->planes[0..2]
    // 寫，自己完全不配置記憶體，所以呼叫端必須先備好至少 3 個 plane 的陣列。
    // 之前這裡只寫 `android_flex_layout flex{}`，planes 是 NULL——實機直接崩在
    // GetFlexLayout+92（SIGSEGV，fault addr 0x8）。
    android_flex_plane_t flex_planes[4]{};
    android_flex_layout flex{};
    flex.num_planes = 4;
    flex.planes = flex_planes;
    GRALLOC1_PFN_LOCK_FLEX lock_flex = cammw_util_gralloc_lock_flex_fn();
    const gralloc1_rect_t rect = {0, 0, width, height};
    rc = lock_flex == nullptr
             ? GRALLOC1_ERROR_UNSUPPORTED
             : lock_flex(device, gralloc_handle, kProducerUsage, 0, &rect, &flex, -1);
    if (rc != GRALLOC1_ERROR_NONE) {
      ALOGE("E: %s: gralloc1_lock_flex_failed", "cammw_util_gralloc_make_buf_from_private_handle");
      return CAMMW_ERR_FAILED;
    }
    for (int i = 0; i < 3 && i < static_cast<int>(flex.num_planes); ++i) {
      const android_flex_plane_t &plane = flex.planes[i];
      if (plane.component == FLEX_COMPONENT_Y) {
        y_top_left = static_cast<int>(reinterpret_cast<intptr_t>(plane.top_left));
      } else if (plane.component == FLEX_COMPONENT_Cb) {
        cb_top_left = static_cast<int>(reinterpret_cast<intptr_t>(plane.top_left));
        v_increment = plane.v_increment;
      } else if (plane.component == FLEX_COMPONENT_Cr) {
        cr_top_left = static_cast<int>(reinterpret_cast<intptr_t>(plane.top_left));
        v_increment = plane.v_increment;
      }
    }
  }

  out->w[kCammwBufBase] = static_cast<int32_t>(handle->base);
  const int32_t handle_size = static_cast<int32_t>(handle->size);
  out->w[kCammwBufSize] = handle_size;
  out->w[kCammwBufFd] = handle->fd;
  out->w[kCammwBufPrivHandle] = reinterpret_cast<intptr_t>(handle);
  out->w[kCammwBufAttrPtr] = reinterpret_cast<intptr_t>(attrs);

  if (populate_metadata == 0) {
    return 0;
  }

  out->w[kCammwBufPixelFormat] = pixel_format;
  out->w[kCammwBufWidth] = width;
  out->w[kCammwBufHeight] = height;
  const int32_t handle_offset = static_cast<int32_t>(handle->offset);
  out->w[kCammwBufPlaneOffset2] = 0;
  out->w[kCammwBufOffset] = handle_offset;
  out->w[10] = 0;
  out->w[kCammwBufStride] = handle->width;
  out->w[kCammwBufVStride] = handle->height;
  out->w[kCammwBufUsageOrColorSpace] = static_cast<int32_t>(usage_or_color_space);
  out->w[0xf] = 0;
  out->w[0x14] = 0; out->w[0x15] = 0;
  out->w[0x16] = 0; out->w[0x17] = 0;
  out->w[0x18] = 0; out->w[0x19] = 0;
  out->w[kCammwBufWidth2] = width;
  out->w[kCammwBufHeight2] = height;
  out->w[kCammwBufImageSize] = handle_size - handle_offset;

  const int32_t stride = out->w[kCammwBufStride];
  const int32_t v_stride = out->w[kCammwBufVStride];
  const int32_t fmt = handle->format;

  // 下面這段是把「哪個 fmt 落在原本哪個 if/else 分支」攤平成單一 switch——
  // 原始反編譯有好幾個分支最後 goto 到同一段共用收尾碼（LAB_00014df6 /
  // LAB_00014db2 / LAB_00014e02），這裡直接照『最終效果』寫，不強行照抄
  // Ghidra 的分支結構（那樣反而更容易抄錯）。每一支後面都註明原本走的
  // 是哪個 goto target，方便之後對照。
  const int32_t d_cb = cb_top_left - y_top_left;
  const int32_t d_cr = cr_top_left - y_top_left;
  switch (fmt) {
    case 0x10:  // -> LAB_00014df6（跟 0x10b 共用）
      out->w[kCammwBufPlaneOffset1] = d_cb;
      out->w[kCammwBufImageSize] = v_stride * v_increment + d_cb;
      break;
    // LAB_00014db2 的共用收尾是
    //   param_9[10] = iVar4;                                   // plane_offset1
    //   param_9[0x13] = iVar4 + ((piVar8 * param_9[0xd]) >> 1) // image_size
    // 其中 piVar8 是 v_increment、param_9[0xd] 是 v_stride。之前這三支
    // （0x11 / 0x109+0x7fa30c04 / 0x113）誤寫成 stride * v_stride。
    case 0x11:  // -> LAB_00014d96 -> LAB_00014db2（跟 0x113 共用）
      out->w[kCammwBufPlaneOffset1] = d_cr;
      out->w[kCammwBufImageSize] = d_cr + ((v_increment * v_stride) >> 1);
      break;
    case 0x21:  // 只有 usage==0x80000 才動，其餘什麼都不做
      if (usage_or_color_space == 0x80000) {
        const uint32_t aligned_w = (static_cast<uint32_t>(width) + 0x1f) & ~0x1fu;
        const int32_t old_stride = out->w[kCammwBufStride];
        out->w[kCammwBufVStride] = old_stride / static_cast<int32_t>(aligned_w);
        out->w[kCammwBufStride] = static_cast<int32_t>(aligned_w);
      }
      break;
    case 0x25:
      out->w[kCammwBufStride] = static_cast<int32_t>((static_cast<uint32_t>(width) + 1) & ~1u);
      out->w[kCammwBufVStride] = static_cast<int32_t>((static_cast<uint32_t>(height) + 1) & ~1u);
      break;
    case 0x102:
      out->w[kCammwBufPlaneOffset1] = d_cb;
      break;
    case 0x109:      // 落到 fmt<0x109 判斷之外，直接 fallthrough 到共用尾段
    case 0x7fa30c04:  // 同上，跟 0x109 共用同一段收尾
      out->w[kCammwBufPlaneOffset1] = d_cb;
      out->w[kCammwBufImageSize] = d_cb + ((v_increment * v_stride) >> 1);
      break;
    case 0x10b:  // -> LAB_00014df6（跟 0x10 共用）
      out->w[kCammwBufPlaneOffset1] = d_cr;
      out->w[kCammwBufImageSize] = v_stride * v_increment + d_cr;
      break;
    case 0x113:  // -> LAB_00014d96（跟 0x11 共用）
      out->w[kCammwBufPlaneOffset1] = d_cr;
      out->w[kCammwBufImageSize] = d_cr + ((v_increment * v_stride) >> 1);
      break;
    case 0x32315659:  // 'YV12'
      out->w[kCammwBufPlaneOffset1] = d_cb;
      out->w[kCammwBufPlaneOffset2] = d_cr;
      out->w[kCammwBufImageSize] = d_cb + ((v_increment * v_stride) >> 1);
      break;
    default:
      // 其餘沒列到的格式：原版在最外層 if/else 找不到分支就直接
      // goto LAB_00014e08（DONE），完全不碰 plane/size 欄位。
      break;
  }
  return 0;
}
