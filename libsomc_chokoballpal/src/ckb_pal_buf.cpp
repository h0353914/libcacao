// ckb::CkbPalBuf —— 逐行對照原廠 libsomc_chokoballpal.so 重建。
//
// 這一層很薄：它把 cammw_util 的 shared-memory buffer 包成
// android::GraphicBuffer 交給上層。兩個入口：
//   createBuf()      配一整塊大 buffer（cammw_util_shmem_alloc_buf）
//   createInstance() 在那塊上開一個「視窗」（cammw_util_shmem_attach_image_buf，
//                    帶 offset），再包成 GraphicBuffer
//
// 位址註記皆為原廠 blob 的 vaddr（Ghidra 載入基底 0x10000，故 0x115a8 = 0x15a8）。

#define LOG_TAG "ckb"

#include "ckb_pal_buf.h"

#include <log/log.h>
#include <stdlib.h>

// cammw_util 的 C API（由 libcammw_util.so 提供）。
extern "C" {
int cammw_util_shmem_alloc_buf(int type, unsigned int flags, unsigned int size,
                               void *out);
int cammw_util_shmem_free_buf(void *buf);
int cammw_util_shmem_attach_image_buf(const int32_t *src, unsigned int width,
                                      unsigned int height, unsigned int format,
                                      const unsigned int *src5,
                                      unsigned int stride, unsigned int scanline,
                                      void *out);
int cammw_util_shmem_detach_image_buf(void *buf);
int cammw_util_shmem_clean_buf(void *buf, unsigned int mode);
int cammw_util_shmem_register_buf(int type, int a, unsigned int size);
int cammw_util_shmem_unregister_buf(int type, int a, unsigned int size);
}

namespace ckb {

// createInstance 的 Format → gralloc pixel format。
// 原版是 .rodata @0x4168 的三個 word：{0x1, 0x10d, 0x10e}。
static const int kGrallocFormat[3] = {0x1, 0x10d, 0x10e};

// cammw_util 的 image buffer（calloc(1, 0x68)）。+0xc 是 private_handle_t*，
// +0x14 是 clean_buf 會清掉的旗標 byte。
static const size_t kCammwImageBufSize = 0x68;
static const size_t kCammwImageBufHandleOffset = 0xc;
static const size_t kCammwImageBufFlagOffset = 0x14;

// cammw_util 的 buf（calloc(1, 0x18)）。
static const size_t kCammwBufSize = 0x18;

// 持有 GraphicBuffer 與 cammw image buffer；CkbPalBuf 只是它的外殼。
// 佈局：+0x0 vptr、+0x4 sp<GraphicBuffer>、+0x8 cammw image buffer。
class CkbPalBufHolder {
 public:
  CkbPalBufHolder() : mImageBuf(calloc(1, kCammwImageBufSize)) {}

  // 原版 0x11a1c：清 vptr、free(imageBuf)、放掉 GraphicBuffer。
  virtual ~CkbPalBufHolder() { free(mImageBuf); }

  // 原版 0x11804。
  int validate() {
    if (mGraphicBuffer != NULL && mGraphicBuffer->initCheck() == 0 &&
        mImageBuf != NULL) {
      return 1;
    }
    return 0;
  }

  android::sp<android::GraphicBuffer> mGraphicBuffer;
  void *mImageBuf;
};

// 原版 0x11a08。
CkbPalBuf::CkbPalBuf() : mHolder(NULL) {}

// 原版 0x11820：先 detach 掉 cammw image buffer，再刪 holder。
CkbPalBuf::~CkbPalBuf() {
  if (mHolder != NULL) {
    cammw_util_shmem_detach_image_buf(mHolder->mImageBuf);
    delete mHolder;
  }
}

// 原版 0x115a8。
CkbPalBuf *CkbPalBuf::createInstance(unsigned int stride, unsigned int scanline,
                                     unsigned int width, unsigned int height,
                                     ckb_buf_t *pBuf, unsigned int offset,
                                     Format format, bool render) {
  if (pBuf == NULL) {
    ALOGE("Invalid ckb_buf_t (pBuf==NULL)");
    return NULL;
  }
  if (pBuf->w0 == 0) {
    ALOGE("Invalid ckb_buf_t (heapId=%d, pBase=0x%08X, privateData=%d)", pBuf->w2,
          0, pBuf->w3);
    return NULL;
  }
  if (format >= 3) {
    ALOGE("unsupported color format 0x%08X", format);
    return NULL;
  }
  // stride 必須 32-byte 對齊，且要蓋得住 width/height。
  if ((stride & 0x1f) != 0 || stride < width || scanline < height) {
    ALOGE("Invalid size. (stride=%d, width=%d, scanline=%d, height=%d)", stride,
          width, scanline, height);
    return NULL;
  }

  CkbPalBufHolder *holder = new CkbPalBufHolder();

  // 從 ckb_buf_t 組出 attach 要的來源 image buffer，type 固定填 1（gralloc）。
  int32_t src[5];
  src[0] = pBuf->w0;
  src[1] = pBuf->w1;
  src[2] = pBuf->w2;
  src[3] = pBuf->w3;
  src[4] = 1;

  // src5[0] 就是這個視窗在大 buffer 裡的 offset。
  unsigned int src5[3];
  src5[0] = offset;
  src5[1] = 0;
  src5[2] = 0;

  int rc = cammw_util_shmem_attach_image_buf(
      src, width, height, static_cast<unsigned int>(format) + 0x4000001, src5,
      stride, scanline, holder->mImageBuf);
  if (rc != 0) {
    ALOGE("cammw_util_shmem_attach_image_buf ERR: %d", rc);
    delete holder;
    return NULL;
  }

  const unsigned int usage = render ? 0x203 : 0x133;
  native_handle_t *handle = *reinterpret_cast<native_handle_t **>(
      reinterpret_cast<char *>(holder->mImageBuf) + kCammwImageBufHandleOffset);

  holder->mGraphicBuffer = new android::GraphicBuffer(
      width, height, kGrallocFormat[format], 1, usage, stride, handle, false);

  if (holder->validate() == 0) {
    ALOGE("GraphicBuffer initCheck invalidate");
    cammw_util_shmem_detach_image_buf(holder->mImageBuf);
    delete holder;
    return NULL;
  }

  CkbPalBuf *self = new CkbPalBuf();
  self->mHolder = holder;
  return self;
}

// 原版 0x118ac。
int CkbPalBuf::createBuf(unsigned int width, unsigned int height,
                         ckb_image_format_t format, bool uncached,
                         ckb_buf_t *pBuf) {
  if (pBuf == NULL) {
    ALOGE("Invalid ckb_buf_t (pBuf==NULL)");
    return 1;
  }

  switch (format) {
    case CKB_IMAGE_FORMAT_YV12:
      // width 對齊 16、height 對齊 2，再補上 chroma 平面。
      width = (width + 0xf) & ~0xfu;
      height = (height + 1) & ~1u;
      width = width + (((width >> 1) + 0xf) & ~0xfu);
      break;
    case CKB_IMAGE_FORMAT_NV21:
    case CKB_IMAGE_FORMAT_NV12:
      width = (width + 0x1f) & ~0x1fu;
      height = ((height + 1) & ~1u) + ((height + 1) >> 1);
      break;
    case CKB_IMAGE_FORMAT_RAW:
      break;
    default:
      ALOGE("unsupported color format 0x%08X", format);
      return 0x11;
  }

  void *buf = calloc(1, kCammwBufSize);
  if (buf == NULL) {
    ALOGE("cammw_buf_t memory allocation Failed");
    return 0x3c;
  }

  const unsigned int size = (width * height + 0xfff) & ~0xfffu;
  const unsigned int flags = uncached ? 0 : 0x1000;

  int rc = cammw_util_shmem_alloc_buf(1, flags, size, buf);
  if (rc != 0) {
    ALOGE("cammw_util_shmem_alloc_buf ERR code:%d,bufSize = %d,format = %d", rc,
          size, format);
    free(buf);
    return 0x43;
  }

  const int32_t *w = static_cast<const int32_t *>(buf);
  pBuf->w0 = w[0];
  pBuf->w1 = w[1];
  pBuf->w2 = w[2];
  pBuf->w3 = w[3];
  pBuf->cammw_buf = buf;
  return 0;
}

// 原版 0x119b8。
void CkbPalBuf::deleteBuf(ckb_buf_t *pBuf) {
  if (pBuf == NULL) {
    return;
  }
  cammw_util_shmem_free_buf(pBuf->cammw_buf);
  free(pBuf->cammw_buf);
}

// 原版 0x119d0：先把 image buffer 的旗標 byte 設 1，再 unlock（mode 1）。
void CkbPalBuf::cleanCache(ckb_image_buf_t *pImageBuf) {
  if (pImageBuf == NULL) {
    return;
  }
  void *inner = pImageBuf->cammw_image_buf;
  if (inner != NULL) {
    *(reinterpret_cast<uint8_t *>(inner) + kCammwImageBufFlagOffset) = 1;
  }
  cammw_util_shmem_clean_buf(inner, 1);
}

// 原版 0x119e6：lock（mode 0）。
void CkbPalBuf::invalidateCache(ckb_image_buf_t *pImageBuf) {
  if (pImageBuf == NULL) {
    return;
  }
  cammw_util_shmem_clean_buf(pImageBuf->cammw_image_buf, 0);
}

// 原版 0x119f2 / 0x119fc。
void CkbPalBuf::increaseMemorySize(unsigned int size) {
  cammw_util_shmem_register_buf(1, 1, size);
}

void CkbPalBuf::decreaseMemorySize(unsigned int size) {
  cammw_util_shmem_unregister_buf(1, 1, size);
}

// 原版 0x11858：先把 sp 複製一份撐住 refcount，再取 native buffer。
ANativeWindowBuffer *CkbPalBuf::getNativeBuffer() {
  if (mHolder == NULL) {
    return NULL;
  }
  android::sp<android::GraphicBuffer> gb = mHolder->mGraphicBuffer;
  return gb->getNativeBuffer();
}

}  // namespace ckb
