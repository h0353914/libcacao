//
// BypassCameraBurstBufferManager.cpp — 連拍緩衝區管理
// 依照 so_32 Ghidra 反編譯結果重建
// 對應 libimageprocessorjni.so 中的 BypassCameraBurstBufferManager_* 函數集
//

#include <android/log.h>
#include <jni.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include <imageprocessor/BypassCamera.h>
#include <cacao/CacaoClient.h>
#include <cacao/ProcessCtrlGatewayBase.h>

// libgui / libui
#include <gui/Surface.h>
#include <gui/IGraphicBufferProducer.h>
#include <binder/IBinder.h>
#include <system/window.h>
#include <android/native_window_jni.h>

// cacao::ImageBuf 建構子 2: ImageBuf(void* handle, ImageSize, DataSpace, ImageFormat)
// 用來包裝從 Surface dequeue 取得的 native_handle

#define LOG_TAG "libimageprocessorjni_real"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

// 前向宣告 (同檔案內的函式)
extern "C" void BypassCameraBurstBufferManager_deleteBuffers(
        imageprocessor::BypassCameraContext* ctx,
        android::Vector<cacao::ImageBuf*>* bufs);

// ─────────────────────────────────────────────────────
// getJpegBufferSize — 計算 JPEG buffer 大小
// ─────────────────────────────────────────────────────
static int32_t getJpegBufferSize(uint32_t w, uint32_t h) {
    // JPEG/BLOB: w * h * 2 + 0x10000 (matching ImageBuf.cpp fmt=0x1000001)
    int32_t sz = (int32_t)w * (int32_t)h * 2 + 0x10000;
    return sz > 0 ? sz : 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraBurstBufferManager_initializeSurface
// 來自 so_32 @ 0x0001a3f0
// 功能：從 JNI Surface 取得 ANativeWindow，設定 BLOB 格式和 buffer 數量
// 原始 .so 步驟：getSurface → getIGraphicBufferProducer → new Surface(producer)
//   → API_CONNECT(CAMERA) → SET_SCALING_MODE(1) → SET_BUFFERS_TRANSFORM(0)
//   → SET_USAGE(0) → query(W/H) → getJpegBufferSize → SET_BUFFER_COUNT → SET_BUFFERS_DIMENSIONS
// param_1=env, param_2=Java Surface, param_3=totalBufCount, param_4=ctx
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraBurstBufferManager_initializeSurface(
        JNIEnv* env, jobject javaSurface, int totalBufCount,
        imageprocessor::BypassCameraContext* ctx) {
    if (!ctx || !javaSurface) return -1;
    imageprocessor::BypassCameraBufferContext* bc = &ctx->bufCtx;

    // 原始 .so 的做法：getSurface → getIGBP → new Surface(producer) 建立獨立 Surface
    // 使用 sp<Surface>::operator= 賦值到 ctx，sp 自動管理舊值的 decStrong
    jclass surfaceClass = env->GetObjectClass(javaSurface);
    if (!surfaceClass) {
        ALOGE("initializeSurface: GetObjectClass failed");
        return -1;
    }
    jfieldID nativeField = env->GetFieldID(surfaceClass, "mNativeObject", "J");
    env->DeleteLocalRef(surfaceClass);
    if (!nativeField) {
        ALOGE("initializeSurface: GetFieldID mNativeObject failed");
        return -1;
    }
    jlong nativePtr = env->GetLongField(javaSurface, nativeField);
    if (!nativePtr) {
        ALOGE("initializeSurface: mNativeObject is null");
        return -1;
    }
    android::Surface* javaNativeSurface =
        reinterpret_cast<android::Surface*>((uintptr_t)nativePtr);

    android::sp<android::IGraphicBufferProducer> producer =
        javaNativeSurface->getIGraphicBufferProducer();
    if (producer == nullptr) {
        ALOGE("initializeSurface: getIGraphicBufferProducer failed");
        return -1;
    }

    // 建立獨立的 native Surface（匹配原始 .so operator_new(0x788) + Surface ctor）
    android::sp<android::Surface> ownSurface = new android::Surface(producer, false);
    if (ownSurface == nullptr) {
        ALOGE("initializeSurface: new Surface failed");
        return -1;
    }

    // sp::operator= 自動 incStrong 新值、decStrong 舊值（匹配原始 .so ctx+0xdc 賦值）
    bc->surface = ownSurface;
    bc->totalBufCount = totalBufCount;

    ANativeWindow* window = bc->surface.get();

    // 原始 .so 操作序列 (Ghidra 0x1a3f0):
    // 1. API_CONNECT(NATIVE_WINDOW_API_CAMERA)
    int ret = native_window_api_connect(window, NATIVE_WINDOW_API_CAMERA);
    if (ret != 0) {
        ALOGE("initializeSurface: api_connect failed: %d", ret);
        goto fail;
    }

    // 2. SET_SCALING_MODE(1) + SET_BUFFERS_TRANSFORM(0) + SET_USAGE(0)
    native_window_set_scaling_mode(window, NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW);
    native_window_set_buffers_transform(window, 0);
    native_window_set_usage(window, 0);

    // 3. query(WIDTH) + query(HEIGHT) → getJpegBufferSize
    {
        int w = 0, h = 0;
        window->query(window, NATIVE_WINDOW_WIDTH, &w);
        window->query(window, NATIVE_WINDOW_HEIGHT, &h);
        ALOGD("initializeSurface: Surface size=%dx%d", w, h);

        int32_t jpegSize = getJpegBufferSize(w, h);
        ALOGD("initializeSurface: jpegBufferSize=%d", jpegSize);
        if (jpegSize < 1) {
            ALOGE("initializeSurface: jpegBufferSize invalid");
            goto fail;
        }

        // 4. SET_BUFFER_COUNT(totalBufCount)
        ret = native_window_set_buffer_count(window, totalBufCount);
        if (ret != 0) {
            ALOGE("initializeSurface: set_buffer_count(%d) failed: %d", totalBufCount, ret);
            goto fail;
        }

        // 5. SET_BUFFERS_DIMENSIONS(jpegSize, 1) — BLOB 格式: width=jpegSize, height=1
        ret = native_window_set_buffers_dimensions(window, jpegSize, 1);
        if (ret != 0) {
            ALOGE("initializeSurface: set_buffers_dimensions(%d,1) failed: %d", jpegSize, ret);
            goto fail;
        }
    }

    ALOGD("initializeSurface: OK window=%p bufCount=%d", window, totalBufCount);
    return 0;

fail:
    native_window_api_disconnect(window, NATIVE_WINDOW_API_CAMERA);
    bc->surface.clear();
    bc->surface = nullptr;
    return -1;
}

// ─────────────────────────────────────────────────────
// BypassCameraBurstBufferManager_finalizeSurface
// 來自 so_32 @ 0x0001a6e1
// ─────────────────────────────────────────────────────
extern "C" void BypassCameraBurstBufferManager_finalizeSurface(
        imageprocessor::BypassCameraContext* ctx) {
    if (!ctx) return;
    imageprocessor::BypassCameraBufferContext* bc = &ctx->bufCtx;

    pthread_mutex_lock(&bc->mutex);
    if (bc->surface != nullptr) {
        ANativeWindow* window = bc->surface.get();
        native_window_api_disconnect(window, NATIVE_WINDOW_API_CAMERA);
        bc->surface.clear();
        bc->surface = nullptr;
    }
    bc->ready = 0;
    bc->dequeueCount = 0;
    bc->totalBufCount = 0;
    pthread_mutex_unlock(&bc->mutex);
    ALOGD("BypassCameraBurstBufferManager_finalizeSurface: done");
}

// ─────────────────────────────────────────────────────
// BypassCameraBurstBufferManager_createBuffers
// 來自 so_32 @ 0x0001a758
// 功能：從 Surface dequeue 所有 buffers，包裝成 ImageBuf
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraBurstBufferManager_createBuffers(
        imageprocessor::BypassCameraContext* ctx) {
    if (!ctx) return -1;
    imageprocessor::BypassCameraBufferContext* bc = &ctx->bufCtx;
    ANativeWindow* window = bc->surface.get();
    if (!window) {
        ALOGE("BypassCameraBurstBufferManager_createBuffers: no surface");
        return -1;
    }

    // 先刪除舊 buffer
    BypassCameraBurstBufferManager_deleteBuffers(ctx, nullptr);

    int count = bc->totalBufCount;
    ALOGD("BypassCameraBurstBufferManager_createBuffers: creating %d buffers", count);

    for (int i = 0; i < count; i++) {
        ANativeWindowBuffer* anwb = nullptr;
        int fence = -1;

        int ret = window->dequeueBuffer(window, &anwb, &fence);
        if (ret != 0 || !anwb) {
            ALOGE("BypassCameraBurstBufferManager_createBuffers: dequeueBuffer[%d] failed: %d", i, ret);
            return -1;
        }

        // 用 ANativeWindowBuffer 的 handle 建構 ImageBuf
        // ImageBuf(void* handle, ImageSize{w,h}, DataSpace(1), ImageFormat(0))
        cacao::ImageSize sz;
        sz.width = anwb->width;
        sz.height = anwb->height;
        cacao::ImageBuf* imgBuf = new cacao::ImageBuf(
            (void*)anwb->handle, sz, cacao::DataSpace(1), cacao::ImageFormat(0));

        imageprocessor::BufEntry* entry = new imageprocessor::BufEntry();
        entry->fence = fence;
        entry->imageBuf = imgBuf;
        entry->anwb = anwb;
        entry->state = 0;  // free
        entry->tag = -1;

        bc->buffers.push_back(entry);
    }

    // Queue 所有 buffer 回 Surface (原始 .so 的做法)
    // 這樣它們就進入 Surface 的 buffer queue，之後可以再 dequeue
    for (size_t i = 0; i < bc->buffers.size(); i++) {
        imageprocessor::BufEntry* e = bc->buffers[i];
        window->cancelBuffer(window, (ANativeWindowBuffer*)e->anwb, e->fence);
        e->fence = -1;
        e->state = 0;
    }

    bc->ready = 1;
    bc->dequeueCount = 0;

    ALOGD("BypassCameraBurstBufferManager_createBuffers: done, %zu buffers created",
        bc->buffers.size());
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraBurstBufferManager_deleteBuffers
// 來自 so_32 @ 0x0001a9c5
// ─────────────────────────────────────────────────────
extern "C" void BypassCameraBurstBufferManager_deleteBuffers(
        imageprocessor::BypassCameraContext* ctx,
        android::Vector<cacao::ImageBuf*>* bufs) {
    if (!ctx) return;
    imageprocessor::BypassCameraBufferContext* bc = &ctx->bufCtx;

    for (size_t i = 0; i < bc->buffers.size(); i++) {
        imageprocessor::BufEntry* e = bc->buffers[i];
        if (e) {
            if (e->imageBuf) {
                delete e->imageBuf;
            }
            delete e;
        }
    }
    bc->buffers.clear();
    bc->ready = 0;
    bc->dequeueCount = 0;
    ALOGD("BypassCameraBurstBufferManager_deleteBuffers: done");
}

// ─────────────────────────────────────────────────────
// BypassCameraBurstBufferManager_dequeueBuffer
// 來自 so_32 @ 0x0001ab64
// 功能：從 Surface dequeue 一個 buffer，找到對應的 BufEntry 返回
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraBurstBufferManager_dequeueBuffer(
        imageprocessor::BypassCameraContext* ctx,
        imageprocessor::BufEntry** outEntry) {
    if (!ctx || !outEntry) return -1;
    *outEntry = nullptr;

    imageprocessor::BypassCameraBufferContext* bc = &ctx->bufCtx;
    pthread_mutex_lock(&bc->mutex);

    if (!bc->ready) {
        ALOGE("BypassCameraBurstBufferManager_dequeueBuffer: not ready");
        pthread_mutex_unlock(&bc->mutex);
        return -1;
    }

    ANativeWindow* window = bc->surface.get();
    if (!window) {
        ALOGE("BypassCameraBurstBufferManager_dequeueBuffer: no surface");
        pthread_mutex_unlock(&bc->mutex);
        return -1;
    }

    ANativeWindowBuffer* anwb = nullptr;
    int fence = -1;
    int ret = window->dequeueBuffer(window, &anwb, &fence);
    if (ret != 0 || !anwb) {
        ALOGE("BypassCameraBurstBufferManager_dequeueBuffer: dequeueBuffer failed: %d", ret);
        pthread_mutex_unlock(&bc->mutex);
        return -1;
    }

    // 找到匹配的 BufEntry (by ANativeWindowBuffer pointer)
    imageprocessor::BufEntry* found = nullptr;
    for (size_t i = 0; i < bc->buffers.size(); i++) {
        if (bc->buffers[i]->anwb == anwb) {
            found = bc->buffers[i];
            break;
        }
    }

    if (!found) {
        ALOGE("BypassCameraBurstBufferManager_dequeueBuffer: buffer not found in pool");
        window->cancelBuffer(window, anwb, fence);
        pthread_mutex_unlock(&bc->mutex);
        return -1;
    }

    found->fence = fence;
    found->state = 1;  // dequeued
    bc->dequeueCount++;

    *outEntry = found;
    ALOGD("BypassCameraBurstBufferManager_dequeueBuffer: got entry=%p imgBuf=%p anwb=%p",
        found, found->imageBuf, found->anwb);

    pthread_mutex_unlock(&bc->mutex);
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraBurstBufferManager_queueBuffer
// 來自 so_32 @ 0x0001acf5
// 功能：把寫完的 buffer queue 回 Surface，讓 Java ImageReader 接收
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraBurstBufferManager_queueBuffer(
        imageprocessor::BypassCameraContext* ctx,
        imageprocessor::BufEntry* entry) {
    if (!ctx || !entry) return -1;
    imageprocessor::BypassCameraBufferContext* bc = &ctx->bufCtx;

    ANativeWindow* window = bc->surface.get();
    if (!window) return -1;

    // Queue buffer 回 Surface — Java ImageReader 會收到 onImageAvailable
    int ret = window->queueBuffer(window, (ANativeWindowBuffer*)entry->anwb, entry->fence);
    entry->fence = -1;
    entry->state = 0;  // free

    if (bc->dequeueCount > 0) bc->dequeueCount--;

    ALOGD("BypassCameraBurstBufferManager_queueBuffer: ret=%d entry=%p", ret, entry);
    return ret;
}

// ─────────────────────────────────────────────────────
// BypassCameraBurstBufferManager_cancelBuffer
// 來自 so_32 @ 0x0001ad9d
// ─────────────────────────────────────────────────────
extern "C" void BypassCameraBurstBufferManager_cancelBuffer(
        imageprocessor::BypassCameraContext* ctx,
        imageprocessor::BufEntry* entry) {
    if (!ctx || !entry) return;
    imageprocessor::BypassCameraBufferContext* bc = &ctx->bufCtx;
    ANativeWindow* window = bc->surface.get();
    if (!window) return;

    window->cancelBuffer(window, (ANativeWindowBuffer*)entry->anwb, entry->fence);
    entry->fence = -1;
    entry->state = 0;
    if (bc->dequeueCount > 0) bc->dequeueCount--;
}

// ─────────────────────────────────────────────────────
// BypassCameraBurstBufferManager_cancelBuffers
// 來自 so_32 @ 0x0001ae2d
// ─────────────────────────────────────────────────────
extern "C" void BypassCameraBurstBufferManager_cancelBuffers(
        imageprocessor::BypassCameraContext* ctx,
        android::Vector<cacao::ImageBuf*>* bufs) {
    (void)bufs;
    if (!ctx) return;
    imageprocessor::BypassCameraBufferContext* bc = &ctx->bufCtx;
    ANativeWindow* window = bc->surface.get();
    if (!window) return;

    for (size_t i = 0; i < bc->buffers.size(); i++) {
        imageprocessor::BufEntry* e = bc->buffers[i];
        if (e && e->state != 0) {
            window->cancelBuffer(window, (ANativeWindowBuffer*)e->anwb, e->fence);
            e->fence = -1;
            e->state = 0;
        }
    }
    bc->dequeueCount = 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraBurstBufferManager_createBufVector
// 來自 so_32 @ 0x0001aa5d
// ─────────────────────────────────────────────────────
extern "C" android::Vector<cacao::ImageBuf*>* BypassCameraBurstBufferManager_createBufVector(
        int capacity) {
    (void)capacity;
    return new android::Vector<cacao::ImageBuf*>();
}

// ─────────────────────────────────────────────────────
// BypassCameraBurstBufferManager_dump
// 來自 so_32 @ 0x0001b049
// ─────────────────────────────────────────────────────
extern "C" void BypassCameraBurstBufferManager_dump(
        imageprocessor::BypassCameraContext* ctx) {
    if (!ctx) return;
    imageprocessor::BypassCameraBufferContext* bc = &ctx->bufCtx;
    ALOGD("BurstBufMgr: ready=%d count=%zu dequeued=%u total=%u",
        bc->ready, bc->buffers.size(), bc->dequeueCount, bc->totalBufCount);
}

// ─────────────────────────────────────────────────────
// BypassCameraBurstBufferManager_cancelAllBuffer
// 來自 so_32 @ 0x0000af65
// 功能：對 bufs 中每個 ImageBuf 對應的 ANativeWindowBuffer 呼叫 cancelBuffer
// ─────────────────────────────────────────────────────
extern "C" void BypassCameraBurstBufferManager_cancelAllBuffer(
        imageprocessor::BypassCameraContext* ctx,
        android::Vector<cacao::ImageBuf*>* bufs) {
    if (!ctx || !bufs) return;
    imageprocessor::BypassCameraBufferContext* bc = &ctx->bufCtx;
    ANativeWindow* window = bc->surface.get();
    if (!window) return;

    for (size_t i = 0; i < bc->buffers.size(); i++) {
        imageprocessor::BufEntry* e = bc->buffers[i];
        if (!e || e->state == 0) continue;
        // 檢查此 BufEntry 的 imageBuf 是否在 bufs 中
        for (size_t j = 0; j < bufs->size(); j++) {
            if ((*bufs)[j] == e->imageBuf) {
                window->cancelBuffer(window, (ANativeWindowBuffer*)e->anwb, e->fence);
                e->fence = -1;
                e->state = 0;
                bc->dequeueCount = (bc->dequeueCount > 0) ? bc->dequeueCount - 1 : 0;
                break;
            }
        }
    }
}
