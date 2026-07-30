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
#include <unistd.h>

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
// 功能：從 Surface dequeue 所有 buffers，包裝成 ImageBuf，同時以
// ANativeWindowBuffer* 與 imageBuf->getNative()（native handle）兩種 key
// 各塞進一個 SortedVector（見 BypassCamera.h 的 BypassCameraBufferContext
// 說明）——這兩個 vector 只在這裡塞入一次，之後 dequeue/queue 都只改
// BufEntry::state，不會再 add/remove。
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

    bc->byBufferPtr = new android::SortedVector<android::key_value_pair_t<void*, imageprocessor::BufEntry*>>();
    bc->byNativeHandle = new android::SortedVector<android::key_value_pair_t<void*, imageprocessor::BufEntry*>>();

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

        bc->byBufferPtr->add(android::key_value_pair_t<void*, imageprocessor::BufEntry*>(
                static_cast<void*>(anwb), entry));
        bc->byNativeHandle->add(android::key_value_pair_t<void*, imageprocessor::BufEntry*>(
                imgBuf->getNative(), entry));
    }

    // Queue 所有 buffer 回 Surface (原始 .so 的做法)
    // 這樣它們就進入 Surface 的 buffer queue，之後可以再 dequeue
    for (size_t i = 0; i < bc->byBufferPtr->size(); i++) {
        imageprocessor::BufEntry* e = (*bc->byBufferPtr)[i].value;
        window->cancelBuffer(window, (ANativeWindowBuffer*)e->anwb, e->fence);
        e->fence = -1;
        e->state = 0;
    }

    bc->ready = 1;
    bc->dequeueCount = 0;

    ALOGD("BypassCameraBurstBufferManager_createBuffers: done, %zu buffers created",
        bc->byBufferPtr->size());
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

    if (bc->byBufferPtr) {
        for (size_t i = 0; i < bc->byBufferPtr->size(); i++) {
            imageprocessor::BufEntry* e = (*bc->byBufferPtr)[i].value;
            if (e) {
                if (e->imageBuf) {
                    delete e->imageBuf;
                }
                // [已確認，原本遺漏] 反編譯 so_32 @ 0x1a9c5 確認原版在刪除每個
                // BufEntry 前，若 fence != -1 會呼叫 close(fence) 釋放 sync
                // fence fd。先前的重建版本完全沒有這一步，等於每次
                // deleteBuffers（app 重啟/切換模式時都會呼叫）都洩漏一個
                // fence fd。裝置實測（20260727 session）在反覆重啟相機 App
                // 多次後，vendor camera provider process
                // （vendor.somc.hardware.camera.provider@1.0-service）在
                // gralloc buffer 清理路徑上以 SIGABRT 崩潰
                // （"invalid pthread_t 0x84 passed to libc"），懷疑與長期
                // 洩漏的 fence fd 耗盡/汙染 vendor 端資源池有關，修正後待
                // 進一步驗證是否根治。
                if (e->fence != -1) {
                    close(e->fence);
                }
                delete e;
            }
        }
        delete bc->byBufferPtr;
        bc->byBufferPtr = nullptr;
    }
    if (bc->byNativeHandle) {
        // 同一批 BufEntry* 已經在上面刪過，這裡只需要銷毀容器本身
        delete bc->byNativeHandle;
        bc->byNativeHandle = nullptr;
    }
    bc->ready = 0;
    bc->dequeueCount = 0;
    ALOGD("BypassCameraBurstBufferManager_deleteBuffers: done");
}

// ─────────────────────────────────────────────────────
// BypassCameraBurstBufferManager_dequeueBuffer
// 來自 so_32 @ 0x0001ab64
// 功能：從 Surface dequeue 一個 buffer，以 ANativeWindowBuffer* 為 key
// 查回對應的 BufEntry（反編譯確認原版用 SortedVectorImpl::indexOf，不是
// 線性搜尋）
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

    imageprocessor::BufEntry* found = nullptr;
    if (bc->byBufferPtr) {
        ssize_t idx = bc->byBufferPtr->indexOf(
                android::key_value_pair_t<void*, imageprocessor::BufEntry*>(static_cast<void*>(anwb)));
        if (idx >= 0) {
            found = (*bc->byBufferPtr)[idx].value;
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
// BypassCameraBurstBufferManager_findByNativeHandle
// 反編譯 BypassCameraBurst_requestSnapshot (so_32 @ 0x1b290) 確認：連拍
// 一次可能同時有多個 buffer 在途，必須用 imageBuf->getNative() 對應的
// native handle 為 key 查回 BufEntry，"state==2" 線性搜尋無法區分是哪一個。
// ─────────────────────────────────────────────────────
extern "C" imageprocessor::BufEntry* BypassCameraBurstBufferManager_findByNativeHandle(
        imageprocessor::BypassCameraContext* ctx, void* nativeHandle) {
    if (!ctx) return nullptr;
    imageprocessor::BypassCameraBufferContext* bc = &ctx->bufCtx;
    if (!bc->byNativeHandle) return nullptr;
    ssize_t idx = bc->byNativeHandle->indexOf(
            android::key_value_pair_t<void*, imageprocessor::BufEntry*>(nativeHandle));
    if (idx < 0) return nullptr;
    return (*bc->byNativeHandle)[idx].value;
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

    if (bc->byBufferPtr) {
        for (size_t i = 0; i < bc->byBufferPtr->size(); i++) {
            imageprocessor::BufEntry* e = (*bc->byBufferPtr)[i].value;
            if (e && e->state != 0) {
                window->cancelBuffer(window, (ANativeWindowBuffer*)e->anwb, e->fence);
                e->fence = -1;
                e->state = 0;
            }
        }
    }
    bc->dequeueCount = 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraBurstBufferManager_createBufVector
// 來自 so_32 @ 0x0001aa5c
// 功能：連續 dequeue `count` 個 buffer，把每個 ImageBuf* 塞進一個新配置的
// android::Vector<ImageBuf*>，供 BypassCameraBurst_requestSnapshot 一次
// dispatch 多張連拍影格。dequeue 失敗的張數會被跳過（不會讓整體失敗），
// 但若一張都沒拿到就回傳 0 顆（讓呼叫端視為失敗）。
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraBurstBufferManager_createBufVector(
        imageprocessor::BypassCameraContext* ctx,
        android::Vector<cacao::ImageBuf*>** outVec,
        int count) {
    if (!outVec) return 0;
    *outVec = new android::Vector<cacao::ImageBuf*>();
    if (!ctx || count < 1) {
        ALOGD("BypassCameraBurstBufferManager_createBufVector: count=%d, empty vector", count);
        return 0;
    }

    int gotten = 0;
    for (int i = 0; i < count; i++) {
        imageprocessor::BufEntry* entry = nullptr;
        if (BypassCameraBurstBufferManager_dequeueBuffer(ctx, &entry) == 0 && entry) {
            (*outVec)->push_back(entry->imageBuf);
            gotten++;
        }
    }
    ALOGD("BypassCameraBurstBufferManager_createBufVector: requested=%d got=%d", count, gotten);
    return gotten;
}

// ─────────────────────────────────────────────────────
// BypassCameraBurstBufferManager_dump
// 來自 so_32 @ 0x0001b048
// ─────────────────────────────────────────────────────
extern "C" void BypassCameraBurstBufferManager_dump(
        imageprocessor::BypassCameraContext* ctx) {
    if (!ctx) return;
    imageprocessor::BypassCameraBufferContext* bc = &ctx->bufCtx;
    ALOGD("BurstBufMgr: ready=%d count=%zu dequeued=%u total=%u",
        bc->ready, bc->byBufferPtr ? bc->byBufferPtr->size() : 0, bc->dequeueCount, bc->totalBufCount);
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

    if (!bc->byBufferPtr) return;
    for (size_t i = 0; i < bc->byBufferPtr->size(); i++) {
        imageprocessor::BufEntry* e = (*bc->byBufferPtr)[i].value;
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
