// libsomc_chokoballpal —— 從原廠 blob 重建。
//
// 型別名稱、namespace、參數型別都不能改：mangled name 是對外 ABI 的一部分，
// libcammw.so 那邊是照原廠符號解析的。
//   _ZN3ckb9CkbPalBuf14createInstanceEjjjjP9ckb_buf_tjNS0_6FormatEb
//   _ZN3ckb9CkbPalBuf9createBufEjj18ckb_image_format_tbP9ckb_buf_t
//   _ZN3ckb9CkbPalBuf9deleteBufEP9ckb_buf_t
//   _ZN3ckb9CkbPalBuf10cleanCacheEP15ckb_image_buf_t
//   _ZN3ckb9CkbPalBuf15invalidateCacheEP15ckb_image_buf_t
//   _ZN3ckb9CkbPalBuf15getNativeBufferEv
//   _ZN3ckb9CkbPalBuf18increaseMemorySizeEj / 18decreaseMemorySizeEj
//   _ZN3ckb9CkbPalBufC1Ev / C2Ev / D0Ev / D1Ev / D2Ev
//   _ZTVN3ckb15CkbPalBufHolderE / _ZTVN3ckb9CkbPalBufE

#pragma once

#include <stdint.h>

#include <ui/GraphicBuffer.h>
#include <utils/StrongPointer.h>

// createBuf 的輸出／createInstance 的輸入。
// 原版只碰前四個 word 與 +0x10 的指標：
//   createInstance 讀 w[0](0 視為無效)、w[1]、w[2](log 的 heapId)、w[3]
//   createBuf 寫 w[0..3]（從 cammw buf 複製兩個 8-byte word）與 +0x10 的
//   cammw buf 指標；deleteBuf 只用 +0x10。
struct ckb_buf_t {
  int32_t w0;
  int32_t w1;
  int32_t w2;
  int32_t w3;
  void *cammw_buf;
  int32_t reserved;
};

// cleanCache / invalidateCache 的輸入。原版只讀 +0x10。
struct ckb_image_buf_t {
  int32_t reserved[4];
  void *cammw_image_buf;
};

// createBuf 支援的四種 format（原版 switch 的四個常數）。
enum ckb_image_format_t {
  CKB_IMAGE_FORMAT_YV12 = 0x2,
  CKB_IMAGE_FORMAT_NV21 = 0x10,
  CKB_IMAGE_FORMAT_NV12 = 0x20,
  CKB_IMAGE_FORMAT_RAW = 0x40000000,
};

namespace ckb {

class CkbPalBufHolder;

class CkbPalBuf {
 public:
  // createInstance 的 format：原版用 (fmt + 0x4000001) 傳給
  // cammw_util_shmem_attach_image_buf，並用 kGrallocFormat[fmt] 給 GraphicBuffer。
  enum Format {
    FORMAT_0 = 0,
    FORMAT_1 = 1,
    FORMAT_2 = 2,
  };

  CkbPalBuf();
  virtual ~CkbPalBuf();

  static CkbPalBuf *createInstance(unsigned int stride, unsigned int scanline,
                                   unsigned int width, unsigned int height,
                                   ckb_buf_t *pBuf, unsigned int offset,
                                   Format format, bool render);

  static int createBuf(unsigned int width, unsigned int height,
                       ckb_image_format_t format, bool uncached,
                       ckb_buf_t *pBuf);
  static void deleteBuf(ckb_buf_t *pBuf);

  static void cleanCache(ckb_image_buf_t *pImageBuf);
  static void invalidateCache(ckb_image_buf_t *pImageBuf);

  static void increaseMemorySize(unsigned int size);
  static void decreaseMemorySize(unsigned int size);

  ANativeWindowBuffer *getNativeBuffer();

 private:
  CkbPalBufHolder *mHolder;
};

}  // namespace ckb
