//
// BypassCameraPhoto.cpp — 相片模式操作
// 依照 so_32 Ghidra 反編譯結果重建
//

#include <string.h>
#include <android/log.h>
#include <jni.h>

#include <imageprocessor/BypassCamera.h>
#include <cacao/CacaoClient.h>
#include <cacao/ProcessCtrlGatewayBase.h>

#define LOG_TAG "libimageprocessorjni_real"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

// 宣告 Caps.cpp 中定義的輔助函式
extern cacao::ProcessCtrlResult* createResultWithCtx(imageprocessor::BypassCameraContext* ctx);

// 宣告 BypassCameraBurstBufferManager.cpp 中定義的函式
extern "C" int BypassCameraBurstBufferManager_initializeSurface(
        JNIEnv* env, jobject javaSurface, int totalBufCount,
        imageprocessor::BypassCameraContext* ctx);
extern "C" void BypassCameraBurstBufferManager_finalizeSurface(
        imageprocessor::BypassCameraContext* ctx);
extern "C" int BypassCameraBurstBufferManager_createBuffers(
        imageprocessor::BypassCameraContext* ctx);
extern "C" void BypassCameraBurstBufferManager_deleteBuffers(
        imageprocessor::BypassCameraContext* ctx,
        android::Vector<cacao::ImageBuf*>* bufs);
extern "C" int BypassCameraBurstBufferManager_dequeueBuffer(
        imageprocessor::BypassCameraContext* ctx,
        imageprocessor::BufEntry** outEntry);
extern "C" int BypassCameraBurstBufferManager_queueBuffer(
        imageprocessor::BypassCameraContext* ctx,
        imageprocessor::BufEntry* entry);

// ─────────────────────────────────────────────────────
// SnapshotReadyCallback::onHandleResult
// 來自 so_32 @ 0x00008c91
// 呼叫 callbackFromNative(0xa, 0, 1, isSuccessful, isDataValid, ?, exif...)
// ─────────────────────────────────────────────────────
namespace imageprocessor {

// Callback constructors / destructors / onHandleProgress
SnapshotReadyCallback::SnapshotReadyCallback() = default;
SnapshotReadyCallback::~SnapshotReadyCallback() = default;
void SnapshotReadyCallback::onHandleProgress(const cacao::ProcessResultBase*) {}

SnapshotCallback::SnapshotCallback() = default;
SnapshotCallback::~SnapshotCallback() = default;

SnapshotFreeCallback::SnapshotFreeCallback() = default;
SnapshotFreeCallback::~SnapshotFreeCallback() = default;
void SnapshotFreeCallback::onHandleProgress(const cacao::ProcessResultBase*) {}

void SnapshotReadyCallback::onHandleResult(const cacao::ProcessResultBase* result) {
    BypassCameraContext* ctx = getCtxFromResult(result);
    if (!ctx) return;

    pthread_mutex_lock(&ctx->photoLock);
    if (!ctx->photoInitialized) {
        pthread_mutex_unlock(&ctx->photoLock);
        return;
    }

    // 依照原始 .so 邏輯讀取 result 欄位
    // b1 = field_1c != 0, b2 = field_2c != 0, b3/i1/i2 = bytes of field_2c, i3 = field_30 byte
    uint32_t errCode = result->getResult();
    bool b1, b2, b3;
    int i1, i2, i3;
    if (errCode == 0) {
        const auto* r = static_cast<const cacao::ProcessCtrlResult*>(result);
        b1 = (r->field_1c != 0);
        b2 = (r->field_2c != 0);
        b3 = ((r->field_2c >> 8)  & 0xFF) != 0;
        i1 = (r->field_2c >> 16) & 0xFF;
        i2 = (r->field_2c >> 24) & 0xFF;
        i3 = r->field_30 & 0xFF;
    } else {
        b1 = b2 = b3 = false;
        i1 = 0; i2 = 0xFF; i3 = 0xFF;
    }

    // CB_SNAPSHOT_READY_DONE = 0xa
    callPhotoCallback(ctx,
                      CB_SNAPSHOT_READY_DONE,
                      0, 1,
                      b1, b2, b3,
                      i1, i2, i3);
    pthread_mutex_unlock(&ctx->photoLock);
}

// ─────────────────────────────────────────────────────
// SnapshotCallback::onHandleProgress
// 來自 so_32 @ 0x00008d91
// 呼叫 callbackFromNative(0x14, progress, 1, isSuccess, false, false, 0, 0xff, 0xff)
// ─────────────────────────────────────────────────────
void SnapshotCallback::onHandleProgress(const cacao::ProcessResultBase* result) {
    BypassCameraContext* ctx = getCtxFromResult(result);
    if (!ctx) return;

    int32_t  progress = result->getProgress();
    uint32_t errCode  = result->getResult();
    bool isSuccess    = (errCode == 0);

    // CB_SHUTTER_DONE = 0x14
    // cald 會對同一次 snapshot 送兩次 handleProgress (progress=0, progress=1)，
    // 但 Java 端只 enqueue 一個 saving request，第二次會導致 NPE。
    // 用 shutterDoneSent flag 確保每次 snapshot 只發一次 ShutterDone。
    pthread_mutex_lock(&ctx->photoLock);
    if (ctx->photoInitialized && !ctx->shutterDoneSent) {
        ctx->shutterDoneSent = true;
        callPhotoCallback(ctx,
                          CB_SHUTTER_DONE,
                          progress, 1,
                          isSuccess, false, false,
                          0, 0xff, 0xff);
    }
    pthread_mutex_unlock(&ctx->photoLock);
}

// ─────────────────────────────────────────────────────
// SnapshotCallback::onHandleResult
// 來自 so_32 @ 0x00008e31
// 呼叫 callbackFromNative(0x15, requestId, 1, false, false, false, 0, 0xff, 0xff)
// ─────────────────────────────────────────────────────
void SnapshotCallback::onHandleResult(const cacao::ProcessResultBase* result) {
    BypassCameraContext* ctx = getCtxFromResult(result);
    if (!ctx) return;

    pthread_mutex_lock(&ctx->photoLock);
    if (!ctx->photoInitialized) {
        pthread_mutex_unlock(&ctx->photoLock);
        return;
    }

    uint32_t reqId = ctx->requestCounter;

    // 找到 state==2 (in_use) 的 entry，queue buffer 回 Surface
    // 原始 .so 用 SortedVector 追蹤，我們用線性搜尋
    BypassCameraBufferContext* bc = &ctx->bufCtx;
    BufEntry* entry = nullptr;
    for (size_t i = 0; i < bc->buffers.size(); i++) {
        if (bc->buffers[i]->state == 2) {
            entry = bc->buffers[i];
            break;
        }
    }

    if (entry) {
        entry->state = 3;  // done
        BypassCameraBurstBufferManager_queueBuffer(ctx, entry);
        ALOGD("SnapshotCallback::onHandleResult: queued entry=%p tag=%d", entry, entry->tag);
    } else {
        ALOGD("SnapshotCallback::onHandleResult: no in-use entry found");
    }

    // CB_SNAPSHOT_DONE = 0x15
    callPhotoCallback(ctx,
                      CB_SNAPSHOT_DONE,
                      (int)reqId, 1,
                      false, false, false,
                      0, 0xff, 0xff);

    pthread_mutex_unlock(&ctx->photoLock);
}

// ─────────────────────────────────────────────────────
// SnapshotFreeCallback::onHandleResult
// 來自 so_32 @ 0x00008f3d
// 呼叫 callbackFromNative(0x1e, 0, 1, false, false, false, 0, 0xff, 0xff)
// ─────────────────────────────────────────────────────
void SnapshotFreeCallback::onHandleResult(const cacao::ProcessResultBase* result) {
    BypassCameraContext* ctx = getCtxFromResult(result);
    if (!ctx) return;

    // QV700WMR11 上 type=4 的 handleResult 不被 cald 呼叫，
    // 在 snapshotFree 完成時 Java 已完全處理 CB_SHUTTER_DONE 且狀態機就緒，
    // 此時 queue buffer 是最安全的時機。
    pthread_mutex_lock(&ctx->photoLock);
    if (ctx->photoInitialized) {
        BypassCameraBufferContext* bc = &ctx->bufCtx;
        for (size_t i = 0; i < bc->buffers.size(); i++) {
            if (bc->buffers[i]->state == 2) {
                bc->buffers[i]->state = 3;
                BypassCameraBurstBufferManager_queueBuffer(ctx, bc->buffers[i]);
                ALOGD("SnapshotFreeCallback::onHandleResult: queued entry=%p tag=%d",
                      bc->buffers[i], bc->buffers[i]->tag);
                break;
            }
        }
    }
    pthread_mutex_unlock(&ctx->photoLock);

    // CB_SNAPSHOT_FREE_DONE = 0x1e
    callPhotoCallback(ctx,
                      CB_SNAPSHOT_FREE_DONE,
                      0, 1,
                      false, false, false,
                      0, 0xff, 0xff);
}

// Burst Callback constructors / destructors / onHandleProgress
BurstShotPrepareCallback::BurstShotPrepareCallback() = default;
BurstShotPrepareCallback::~BurstShotPrepareCallback() = default;
void BurstShotPrepareCallback::onHandleProgress(const cacao::ProcessResultBase*) {}

BurstShotFinishCallback::BurstShotFinishCallback() = default;
BurstShotFinishCallback::~BurstShotFinishCallback() = default;
void BurstShotFinishCallback::onHandleProgress(const cacao::ProcessResultBase*) {}

BurstCallback::BurstCallback() = default;
BurstCallback::~BurstCallback() = default;

// ─────────────────────────────────────────────────────
// BurstShotPrepareCallback::onHandleResult
// 來自 so_32 @ 0x00008f99
// ─────────────────────────────────────────────────────
void BurstShotPrepareCallback::onHandleResult(const cacao::ProcessResultBase* result) {
    BypassCameraContext* ctx = getCtxFromResult(result);
    if (!ctx) return;

    bool isSuccess = (result->getResult() == 0);
    // CB_BURST_PREPARE_DONE = 0x28
    callPhotoCallback(ctx,
                      CB_BURST_PREPARE_DONE,
                      0, 1,
                      isSuccess, false, false,
                      0, 0xff, 0xff);
}

// ─────────────────────────────────────────────────────
// BurstShotFinishCallback::onHandleResult
// 來自 so_32 @ 0x0000902d
// ─────────────────────────────────────────────────────
void BurstShotFinishCallback::onHandleResult(const cacao::ProcessResultBase* result) {
    BypassCameraContext* ctx = getCtxFromResult(result);
    if (!ctx) return;

    bool isSuccess = (result->getResult() == 0);
    // CB_BURST_FINISH_DONE = 0x32
    callPhotoCallback(ctx,
                      CB_BURST_FINISH_DONE,
                      0, 1,
                      isSuccess, false, false,
                      0, 0xff, 0xff);
}

// ─────────────────────────────────────────────────────
// BurstCallback::onHandleProgress / onHandleResult
// ─────────────────────────────────────────────────────
void BurstCallback::onHandleProgress(const cacao::ProcessResultBase* result) {
    BypassCameraContext* ctx = getCtxFromResult(result);
    if (!ctx) return;
    int32_t progress = result->getProgress();
    bool isSuccess   = (result->getResult() == 0);
    // 拍攝中 progress → shutter done
    callPhotoCallback(ctx,
                      CB_SHUTTER_DONE,
                      progress, 1,
                      isSuccess, false, false,
                      0, 0xff, 0xff);
}

void BurstCallback::onHandleResult(const cacao::ProcessResultBase* result) {
    BypassCameraContext* ctx = getCtxFromResult(result);
    if (!ctx) return;
    callPhotoCallback(ctx,
                      CB_SNAPSHOT_DONE,
                      (int)ctx->requestCounter, 1,
                      false, false, false,
                      0, 0xff, 0xff);
}

} // namespace imageprocessor

// ─────────────────────────────────────────────────────
// BypassCameraPhoto_initialize
// 來自 so_32 @ 0x00008309
// 功能：建立 callback 物件、分配 ProcessCtrlResult、取得 JNI global refs
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraPhoto_initialize(JNIEnv* env, jobject thiz, imageprocessor::BypassCameraContext* ctx) {
    if (!env || !thiz || !ctx) return -1;

    // 儲存 Java BypassCamera 物件的 global ref（用於回呼）
    if (!ctx->photoJObj) {
        ctx->photoJObj = env->NewGlobalRef(thiz);
        if (!ctx->photoJObj) {
            ALOGE("BypassCameraPhoto_initialize: NewGlobalRef failed");
            return -1;
        }
    }

    // 取得 callbackFromNative 方法 ID
    if (!ctx->photoMethodId) {
        jclass clazz = env->GetObjectClass(thiz);
        if (!clazz) return -1;
        ctx->photoMethodId = env->GetMethodID(clazz,
                "callbackFromNative", "(IIIZZZIII)V");
        env->DeleteLocalRef(clazz);
        if (!ctx->photoMethodId) {
            ALOGE("BypassCameraPhoto_initialize: GetMethodID(callbackFromNative) failed");
            return -1;
        }
    }

    // 建立 callback 物件
    if (!ctx->snapshotReadyCb) {
        ctx->snapshotReadyCb = new imageprocessor::SnapshotReadyCallback();
    }
    if (!ctx->snapshotCb) {
        ctx->snapshotCb = new imageprocessor::SnapshotCallback();
    }
    if (!ctx->snapshotFreeCb) {
        ctx->snapshotFreeCb = new imageprocessor::SnapshotFreeCallback();
    }

    // 分配 ProcessCtrlResult（帶 ctx userdata）
    if (!ctx->snapshotReadyResult) {
        ctx->snapshotReadyResult = createResultWithCtx(ctx);
    }
    if (!ctx->snapshotFreeResult) {
        ctx->snapshotFreeResult = createResultWithCtx(ctx);
    }

    ctx->photoInitialized = true;
    ALOGD("BypassCameraPhoto_initialize: OK");
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraPhoto_finalize
// ─────────────────────────────────────────────────────
extern "C" void BypassCameraPhoto_finalize(imageprocessor::BypassCameraContext* ctx) {
    if (!ctx) return;
    pthread_mutex_lock(&ctx->photoLock);
    ctx->photoInitialized = false;
    pthread_mutex_unlock(&ctx->photoLock);
}

// ─────────────────────────────────────────────────────
// BypassCameraPhoto_changeToPhotoMode
// 來自 so_32 @ 0x000086dd
// 功能：先呼叫 stop() 重置，建立 ProcessCtrlMode，呼叫 cacao->start(mode) 切換到拍照模式
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraPhoto_changeToPhotoMode(imageprocessor::BypassCameraContext* ctx,
                                         jint mode, jint inW, jint inH,
                                         jint outW, jint outH, jint flags) {
    if (!ctx || !ctx->cacao) return -1;

    // 原始 so_32 @ 0x186dc: 先呼叫 stop() 重置 gateway 狀態
    int ret = ctx->cacao->stop();
    if (ret != 0) {
        ALOGE("%s: cacao stop() ret=%d", __func__, ret);
        if (ret == -0x6e) {  // PAL_ERR_ALREADY
            return (int)0xfffffffe;  // -2
        }
    }

    cacao::ProcessCtrlMode ctrlMode;
    ctrlMode.field_08 = (uint32_t)mode;   // mode type（PhotoMode enum 值）
    ctrlMode.field_0c = (int32_t)ctx->cameraMode;  // camera index (facing: 0=back, 1=front)
    ctrlMode.field_10 = (uint32_t)inW;
    ctrlMode.field_14 = (uint32_t)inH;
    ctrlMode.field_18 = (uint32_t)outW;
    ctrlMode.field_1c = (uint32_t)outH;
    ctrlMode.field_20 = (uint32_t)flags;
    ctrlMode.field_24 = 0;
    ctrlMode.field_28 = 0;

    ret = ctx->cacao->start(&ctrlMode);
    if (ret != 0) {
        ALOGE("%s: cacao start() failed. ERR=%d", __func__, ret);
        return -1;
    }
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraPhoto_requestSnapshotReady
// 來自 so_32 @ 0x00018835
// 功能：傳送 snapshotReady 請求給 cacao (type=0, null imageBufs)
// 原始 .so 使用 processAsync(param, 0, listener, result)
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraPhoto_requestSnapshotReady(imageprocessor::BypassCameraContext* ctx) {
    if (!ctx || !ctx->cacao) return -1;

    if (!ctx->snapshotReadyCb || !ctx->snapshotReadyResult) {
        ALOGE("BypassCameraPhoto_requestSnapshotReady: not initialized");
        return -1;
    }

    cacao::ProcessCtrlParam param;
    // type=0: SNAPSHOT_READY (預設值)

    ctx->cacao->processAsync(
            &param,
            nullptr,
            ctx->snapshotReadyCb,
            static_cast<cacao::ProcessCtrlResult*>(ctx->snapshotReadyResult));
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraPhoto_prepareSnapshot
// 來自 so_32 @ 0x00018b50
// 功能：initialise Surface buffer 管理，建立 ImageBuf pool
// 原始 .so: check dequeueCount → initializeSurface → createBuffers
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraPhoto_prepareSnapshot(JNIEnv* env,
                                       imageprocessor::BypassCameraContext* ctx,
                                       jobject surface, jint totalBufCount) {
    if (!ctx || !ctx->cacao) return -1;

    // 原始 .so 在此檢查 ctx+0xE8 (dequeueCount)，如果非零則 log 警告
    if (ctx->bufCtx.dequeueCount != 0) {
        ALOGD("BypassCameraPhoto_prepareSnapshot: dequeueCount=%u (non-zero)",
              ctx->bufCtx.dequeueCount);
    }

    int ret = BypassCameraBurstBufferManager_initializeSurface(env, surface, totalBufCount, ctx);
    if (ret != 0) {
        ALOGE("BypassCameraPhoto_prepareSnapshot: initializeSurface failed");
        BypassCameraBurstBufferManager_finalizeSurface(ctx);
        return ret;
    }

    ret = BypassCameraBurstBufferManager_createBuffers(ctx);
    if (ret != 0) {
        ALOGE("BypassCameraPhoto_prepareSnapshot: createBuffers failed");
        BypassCameraBurstBufferManager_deleteBuffers(ctx, nullptr);
        BypassCameraBurstBufferManager_finalizeSurface(ctx);
        return ret;
    }

    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraPhoto_requestSnapshot
// 來自 so_32 @ 0x000188d8
// 功能：傳送拍照請求，建立包含拍照參數的 ProcessCtrlParam
// 原始 .so 使用 processAsync(param, imageBufs, listener, result)
//
// JNI 參數對應 (從 smali BypassCamera$SnapshotInfo):
//   p1  = isValid (Z)
//   p2  = isExifGpsEnabled (Z)
//   p3  = exifGpsLatitude (D)
//   p4  = exifGpsLongitude (D)
//   p5  = exifGpsAltitude (D)
//   p6  = isExifGpsProcMethodEnabled (Z)
//   p7  = exifGpsProcMethod (String)
//   p8  = isExifGpsTimestampEnabled (Z)
//   p9  = exifGpsTimestamp (I)
//   p10 = isExifOrientationEnabled (Z)
//   p11 = exifOrientation (I)
//   p12 = isExifThumbEnabled (Z)
//   p13 = exifThumbWidth (I)
//   p14 = exifThumbHeight (I)
//   p15 = exifThumbQuality (I)
//   p16 = quality (I)
//   p17 = isQualityAutoControlEnabled (Z)
//   p18 = captureNum (I)
//
// _data layout (驗證自 copyProcessCtrlParam 中的讀取偏移):
//   [0..3]       type = 4
//   [8]          isExifGpsProcMethodEnabled (p6)
//   [9]          isValid (p1)
//   [0x10..0x17] exifGpsLatitude (p3)
//   [0x18..0x1F] exifGpsLongitude (p4)
//   [0x20..0x27] exifGpsAltitude (p5)
//   [0x28]       isExifGpsTimestampEnabled (p8)
//   [0x29..0x128] exifGpsProcMethod string (p7, strncpy 0xFF)
//   [0x129]      isExifThumbEnabled (p12)
//   [0x130..0x137] exifGpsTimestamp (p9, stored as int64)
//   [0x138]      isExifOrientationEnabled (p10) with rotation mapping
//   [0x13C..0x13F] orientation enum (from p11: 0→0, 90→1, 180→2, 270→3)
//   [0x140]      isExifThumbEnabled (p12) — gateway 中另一個 bool
//   [0x144..0x147] exifThumbWidth (p13)
//   [0x148..0x14B] exifThumbHeight (p14)
//   [0x14C..0x14F] exifThumbQuality (p15)
//   [0x150..0x153] quality (p16)
//   [0x154]      isQualityAutoControlEnabled (p17)
// ─────────────────────────────────────────────────────
namespace imageprocessor {
int BypassCameraPhoto_requestSnapshot(JNIEnv* env, jobject thiz, imageprocessor::BypassCameraContext* ctx,
                                       jboolean p1, jboolean p2,
                                       jdouble p3, jdouble p4, jdouble p5,
                                       jboolean p6, jstring p7,
                                       jboolean p8, jint p9, jboolean p10,
                                       jint p11, jboolean p12, jint p13,
                                       jint p14, jint p15, jint p16,
                                       jboolean p17) {
    (void)thiz;
    if (!ctx || !ctx->cacao) return -1;
    if (!ctx->snapshotCb) {
        ALOGE("BypassCameraPhoto_requestSnapshot: snapshotCb not initialized");
        return -1;
    }

    pthread_mutex_lock(&ctx->photoLock);

    // 重設 shutterDoneSent flag，允許新的一次 ShutterDone
    ctx->shutterDoneSent = false;

    // 遞增請求計數器 (原始 .so: *(param_2 + 0x20) += 1)
    ctx->requestCounter++;

    cacao::ProcessCtrlParam param;
    uint8_t* d = param._data;

    // type = 4 (SNAPSHOT)
    *reinterpret_cast<uint32_t*>(d) = 4;

    // 填入拍照參數 (對照 so_32 assembly + copyProcessCtrlParam 讀取位置)
    d[8]  = p6 ? 1 : 0;   // isExifGpsProcMethodEnabled
    d[9]  = p1 ? 1 : 0;   // isValid

    *reinterpret_cast<double*>(d + 0x10) = p3;  // exifGpsLatitude
    *reinterpret_cast<double*>(d + 0x18) = p4;  // exifGpsLongitude
    *reinterpret_cast<double*>(d + 0x20) = p5;  // exifGpsAltitude

    d[0x28] = p8 ? 1 : 0;  // isExifGpsTimestampEnabled

    // exifGpsProcMethod string (strncpy 0xFF bytes 到 _data[0x29])
    memset(d + 0x29, 0, 0x100);
    if (p7) {
        const char* str = env->GetStringUTFChars(p7, nullptr);
        if (str) {
            strncpy(reinterpret_cast<char*>(d + 0x29), str, 0xFF);
            env->ReleaseStringUTFChars(p7, str);
        }
    }

    d[0x129] = p12 ? 1 : 0;  // isExifThumbEnabled

    // exifGpsTimestamp as int64 (原始 .so: local_1b0 = param_18, local_1ac = param_18 >> 31)
    *reinterpret_cast<int64_t*>(d + 0x130) = (int64_t)p9;

    // rotation flag + enum (原始 .so 的 switch 邏輯)
    if (p10) {
        d[0x138] = 1;
        uint32_t rotEnum;
        switch (p11) {
            case 0:   rotEnum = 0; break;
            case 90:  rotEnum = 1; break;
            case 180: rotEnum = 2; break;
            case 270: rotEnum = 3; break;
            default:  d[0x138] = 0; rotEnum = 0; break;
        }
        *reinterpret_cast<uint32_t*>(d + 0x13C) = rotEnum;
    } else {
        d[0x138] = 0;
    }

    d[0x140] = p12 ? 1 : 0;  // isExifThumbEnabled (second copy in gateway)
    *reinterpret_cast<int32_t*>(d + 0x144) = p13;  // exifThumbWidth
    *reinterpret_cast<int32_t*>(d + 0x148) = p14;  // exifThumbHeight
    *reinterpret_cast<int32_t*>(d + 0x14C) = p15;  // exifThumbQuality
    *reinterpret_cast<int32_t*>(d + 0x150) = p16;  // quality
    d[0x154] = p17 ? 1 : 0;  // isQualityAutoControlEnabled

    // 建立新的 ProcessCtrlResult (原始 .so 每次 snapshot 都建立新的)
    cacao::ProcessCtrlResult* result = createResultWithCtx(ctx);

    // 從 Surface dequeue 一個 buffer (原始 .so @ 0x189ce)
    imageprocessor::BufEntry* entry = nullptr;
    int dqRet = BypassCameraBurstBufferManager_dequeueBuffer(ctx, &entry);
    if (dqRet != 0 || !entry) {
        ALOGE("BypassCameraPhoto_requestSnapshot: dequeueBuffer failed");
        pthread_mutex_unlock(&ctx->photoLock);
        return -1;
    }

    // 設定 entry tracking
    entry->tag = (int32_t)ctx->requestCounter;

    // 建立 ImageBuf vector 傳給 processAsync (原始 .so: VectorImpl(4, 7))
    android::Vector<cacao::ImageBuf*> imageBufs;
    imageBufs.push_back(entry->imageBuf);

    ALOGD("requestSnapshot: dequeued entry=%p imgBuf=%p tag=%d",
          entry, entry->imageBuf, entry->tag);

    // 呼叫 processAsync(param, imageBufs, snapshotCb, result)
    ctx->cacao->processAsync(
            &param,
            &imageBufs,
            ctx->snapshotCb,
            result);

    // 設定 buffer state = 2 (in_use, 正由 cald 處理)
    entry->state = 2;

    pthread_mutex_unlock(&ctx->photoLock);
    return 0;
}
} // namespace imageprocessor

// ─────────────────────────────────────────────────────
// BypassCameraPhoto_requestSnapshotFree
// 來自 so_32 @ 0x00018b05
// 原始 .so 使用 processAsync(param, 0, snapshotFreeCb, snapshotFreeResult)
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraPhoto_requestSnapshotFree(imageprocessor::BypassCameraContext* ctx) {
    if (!ctx || !ctx->cacao) return -1;
    if (!ctx->snapshotFreeCb) {
        ALOGE("BypassCameraPhoto_requestSnapshotFree: not initialized");
        return -1;
    }

    if (!ctx->snapshotFreeResult) {
        ctx->snapshotFreeResult = createResultWithCtx(ctx);
    }

    cacao::ProcessCtrlParam param;
    // type=1: SNAPSHOT_FREE (原始 so 反編譯取得)
    *reinterpret_cast<uint32_t*>(param._data) = 1;
    ctx->cacao->processAsync(
            &param,
            nullptr,
            ctx->snapshotFreeCb,
            static_cast<cacao::ProcessCtrlResult*>(ctx->snapshotFreeResult));
    return 0;
}
