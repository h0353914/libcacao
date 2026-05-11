//
// BypassCameraVideo.cpp — 影片/超慢動作模式操作
// 依照 so_32 Ghidra 反編譯結果重建
//

#include <string.h>
#include <android/log.h>
#include <jni.h>
#include <binder/IInterface.h>

#include <imageprocessor/BypassCamera.h>
#include <cacao/CacaoClient.h>
#include <cacao/ProcessCtrlGatewayBase.h>
#include <gui/IGraphicBufferProducer.h>
#include <gui/Surface.h>

#define LOG_TAG "libimageprocessorjni_real"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

extern "C" JNIEnv* getJNIEnv(void);
extern cacao::ProcessCtrlResult* createResultWithCtx(imageprocessor::BypassCameraContext* ctx);

static uint32_t getSuperSlowDataspace(const imageprocessor::BypassCameraContext* ctx) {
    if (!ctx) {
        return 0x102;
    }

    const uint64_t pixelCount =
        static_cast<uint64_t>(ctx->cachedVideoOutWidth) *
        static_cast<uint64_t>(ctx->cachedVideoOutHeight);
    return pixelCount > 0x000e0fffull ? 0x104u : 0x102u;
}

static uint32_t getSuperSlowModeType(jint mode) {
    return static_cast<uint32_t>(mode) + 4u;
}

static bool getSurfaceProducerBinder(JNIEnv* env,
                                     jobject javaSurface,
                                     android::sp<android::IBinder>* binderOut) {
    if (!binderOut) return false;
    binderOut->clear();

    if (!javaSurface) {
        ALOGE("getSurfaceProducerBinder: javaSurface is null");
        return false;
    }

    if (!env) {
        env = getJNIEnv();
    }
    if (!env) {
        ALOGE("getSurfaceProducerBinder: env is null");
        return false;
    }

    jclass surfaceClass = env->GetObjectClass(javaSurface);
    if (!surfaceClass) {
        ALOGE("getSurfaceProducerBinder: GetObjectClass failed");
        return false;
    }

    jfieldID nativeField = env->GetFieldID(surfaceClass, "mNativeObject", "J");
    env->DeleteLocalRef(surfaceClass);
    if (!nativeField) {
        ALOGE("getSurfaceProducerBinder: GetFieldID(mNativeObject) failed");
        return false;
    }

    jlong nativePtr = env->GetLongField(javaSurface, nativeField);
    if (!nativePtr) {
        ALOGE("getSurfaceProducerBinder: mNativeObject is null");
        return false;
    }

    android::Surface* nativeSurface =
        reinterpret_cast<android::Surface*>((uintptr_t)nativePtr);
    android::sp<android::IGraphicBufferProducer> producer =
        nativeSurface->getIGraphicBufferProducer();
    if (producer == nullptr) {
        ALOGE("getSurfaceProducerBinder: getIGraphicBufferProducer failed");
        return false;
    }

    *binderOut = android::IInterface::asBinder(producer);
    if (*binderOut == nullptr) {
        ALOGE("getSurfaceProducerBinder: asBinder failed");
        return false;
    }

    return true;
}

// ─────────────────────────────────────────────────────
// Video callback onHandleResult 實作
// 來自 so_32 @ 0x00009e71 等
// ─────────────────────────────────────────────────────
namespace imageprocessor {

// Video Callback constructors / destructors / onHandleProgress
VideoPrepareCallback::VideoPrepareCallback() = default;
VideoPrepareCallback::~VideoPrepareCallback() = default;
void VideoPrepareCallback::onHandleProgress(const cacao::ProcessResultBase*) {}

VideoFinishCallback::VideoFinishCallback() = default;
VideoFinishCallback::~VideoFinishCallback() = default;
void VideoFinishCallback::onHandleProgress(const cacao::ProcessResultBase*) {}

VideoStartRecCallback::VideoStartRecCallback() = default;
VideoStartRecCallback::~VideoStartRecCallback() = default;
void VideoStartRecCallback::onHandleProgress(const cacao::ProcessResultBase*) {}

VideoPrepareSuperSlowCallback::VideoPrepareSuperSlowCallback() = default;
VideoPrepareSuperSlowCallback::~VideoPrepareSuperSlowCallback() = default;
void VideoPrepareSuperSlowCallback::onHandleProgress(const cacao::ProcessResultBase*) {}

VideoStartSuperSlowRecCallback::VideoStartSuperSlowRecCallback() = default;
VideoStartSuperSlowRecCallback::~VideoStartSuperSlowRecCallback() = default;
void VideoStartSuperSlowRecCallback::onHandleProgress(const cacao::ProcessResultBase*) {}

void VideoFinishCallback::onHandleResult(const cacao::ProcessResultBase* result) {
    ALOGD("VideoFinishCallback::onHandleResult");
    BypassCameraContext* ctx = getCtxFromResult(result);
    if (!ctx) return;
    // CB_VIDEO_STOP_REC_DONE = 0x1
    callVideoCallback(ctx, CB_VIDEO_STOP_REC_DONE);
}

void VideoPrepareCallback::onHandleResult(const cacao::ProcessResultBase* result) {
    ALOGD("VideoPrepareCallback::onHandleResult");
    BypassCameraContext* ctx = getCtxFromResult(result);
    if (!ctx) return;
    // CB_VIDEO_PREPARE_REC_DONE = 0x2
    callVideoCallback(ctx, CB_VIDEO_PREPARE_REC_DONE);
}

void VideoStartRecCallback::onHandleResult(const cacao::ProcessResultBase* result) {
    ALOGD("VideoStartRecCallback::onHandleResult");
    BypassCameraContext* ctx = getCtxFromResult(result);
    if (!ctx) return;
    // CB_VIDEO_START_REC_DONE = 0x0
    callVideoCallback(ctx, CB_VIDEO_START_REC_DONE);
}

void VideoPrepareSuperSlowCallback::onHandleResult(const cacao::ProcessResultBase* result) {
    ALOGD("VideoPrepareSuperSlowCallback::onHandleResult");
    BypassCameraContext* ctx = getCtxFromResult(result);
    if (!ctx) return;
    // CB_VIDEO_PREPARE_SUPER_SLOW_DONE = 0x4
    callVideoCallback(ctx, CB_VIDEO_PREPARE_SUPER_SLOW_DONE);
}

void VideoStartSuperSlowRecCallback::onHandleResult(const cacao::ProcessResultBase* result) {
    ALOGD("VideoStartSuperSlowRecCallback::onHandleResult");
    BypassCameraContext* ctx = getCtxFromResult(result);
    if (!ctx) return;
    // CB_VIDEO_START_SUPER_SLOW_DONE = 0x3
    callVideoCallback(ctx, CB_VIDEO_START_SUPER_SLOW_DONE);
}

} // namespace imageprocessor

// ─────────────────────────────────────────────────────
// BypassCameraVideo_initialize
// 來自 so_32 @ 0x000095b9
// 功能：建立 video callback 物件、分配 ProcessCtrlResult、取得 JNI global refs
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraVideo_initialize(JNIEnv* env, jobject thiz, imageprocessor::BypassCameraContext* ctx) {
    if (!env || !thiz || !ctx) return -1;

    // 儲存 Java BypassCamera 物件的 global ref（用於 video 回呼）
    if (!ctx->videoJObj) {
        ctx->videoJObj = env->NewGlobalRef(thiz);
        if (!ctx->videoJObj) {
            ALOGE("BypassCameraVideo_initialize: NewGlobalRef failed");
            return -1;
        }
    }

    // 取得 callbackFromNativeVideo 方法 ID
    if (!ctx->videoMethodId) {
        jclass clazz = env->GetObjectClass(thiz);
        if (!clazz) return -1;
        ctx->videoMethodId = env->GetMethodID(clazz,
                "callbackFromNativeVideo", "(I)V");
        env->DeleteLocalRef(clazz);
        if (!ctx->videoMethodId) {
            ALOGE("BypassCameraVideo_initialize: GetMethodID(callbackFromNativeVideo) failed");
            return -1;
        }
    }

    // 建立 callback 物件（偏移量依照 BypassCameraVideo_thread dispatcher 反編譯結果）
    // ctx+0x64 (videoFinishCb slot)：cmd 0 (prepareVideoRec) 的回呼 → fires CB=2
    if (!ctx->videoFinishCb) {
        ctx->videoFinishCb = new imageprocessor::VideoPrepareCallback();
    }
    // ctx+0x6c (videoPrepareCb slot)：cmd 2 (stopVideoRec) 的回呼 → fires CB=1
    if (!ctx->videoPrepareCb) {
        ctx->videoPrepareCb = new imageprocessor::VideoFinishCallback();
    }
    // ctx+0x74 (videoStartSuperSlowCb slot)：cmd 1 (startVideoRec) 的回呼 → fires CB=0
    if (!ctx->videoStartSuperSlowCb) {
        ctx->videoStartSuperSlowCb = new imageprocessor::VideoStartRecCallback();
    }
    // ctx+0x7c (videoStartRecCb slot)：cmd 4 (prepareSuperSlow) 的回呼 → fires CB=4
    if (!ctx->videoStartRecCb) {
        ctx->videoStartRecCb = new imageprocessor::VideoPrepareSuperSlowCallback();
    }
    // ctx+0x84 (videoSuperSlowCb slot)：cmd 5 (startSuperSlow) 的回呼 → fires CB=3
    if (!ctx->videoSuperSlowCb) {
        ctx->videoSuperSlowCb = new imageprocessor::VideoStartSuperSlowRecCallback();
    }

    // 分配 ProcessCtrlResult
    if (!ctx->videoFinishResult) {
        ctx->videoFinishResult = createResultWithCtx(ctx);
    }
    if (!ctx->videoPrepareResult) {
        ctx->videoPrepareResult = createResultWithCtx(ctx);
    }
    if (!ctx->videoStartSuperSlowResult) {
        ctx->videoStartSuperSlowResult = createResultWithCtx(ctx);
    }
    if (!ctx->videoStartRecResult) {
        ctx->videoStartRecResult = createResultWithCtx(ctx);
    }
    if (!ctx->videoSuperSlowResult) {
        ctx->videoSuperSlowResult = createResultWithCtx(ctx);
    }

    ctx->videoInitialized = true;
    ALOGD("BypassCameraVideo_initialize: OK");
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraVideo_finalize
// ─────────────────────────────────────────────────────
extern "C" void BypassCameraVideo_finalize(imageprocessor::BypassCameraContext* ctx) {
    if (!ctx) return;
    pthread_mutex_lock(&ctx->videoLock);
    ctx->videoInitialized = false;
    pthread_mutex_unlock(&ctx->videoLock);
}

// ─────────────────────────────────────────────────────
// BypassCameraVideo_changeToVideoMode
// 來自 so_32 @ 0x0000996d
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraVideo_changeToVideoMode(imageprocessor::BypassCameraContext* ctx,
                                         jint mode, jint inW, jint inH,
                                         jint outW, jint outH, jint flags) {
    if (!ctx || !ctx->cacao) return -1;

    ctx->cachedVideoOutWidth = static_cast<uint32_t>(outW);
    ctx->cachedVideoOutHeight = static_cast<uint32_t>(outH);
    ctx->cachedSuperSlowFrameNum = 0;

    // 原始 so_32 @ 0x1996c: 先呼叫 stop() 重置 gateway 狀態
    int ret = ctx->cacao->stop();
    if (ret != 0) {
        ALOGE("%s: cacao stop() ret=%d", __func__, ret);
        if (ret == -0x6e) {  // PAL_ERR_ALREADY
            return (int)0xfffffffe;  // -2
        }
    }

    cacao::ProcessCtrlMode ctrlMode;
    ctrlMode.field_08 = (uint32_t)mode;
    ctrlMode.field_0c = (int32_t)ctx->cameraMode;  // camera index (facing)
    ctrlMode.field_10 = (uint32_t)inW;
    ctrlMode.field_14 = (uint32_t)inH;
    ctrlMode.field_18 = (uint32_t)outW;
    ctrlMode.field_1c = (uint32_t)outH;
    ctrlMode.field_20 = 1;
    ctrlMode.field_24 = 0;
    ctrlMode.field_28 = (uint32_t)flags;

    ret = ctx->cacao->start(&ctrlMode);
    if (ret != 0) {
        ALOGE("%s: cacao start() failed. ERR=%d", __func__, ret);
        return -1;
    }
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraVideo_changeToSuperSlowMode
// 來自 so_32 @ 0x00009a25
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraVideo_changeToSuperSlowMode(imageprocessor::BypassCameraContext* ctx,
                                             jint mode, jint inW, jint inH,
                                             jint outW, jint outH,
                                             jint flags, jint fps, jint frameNum) {
    if (!ctx || !ctx->cacao) return -1;

    ctx->cachedVideoOutWidth = static_cast<uint32_t>(outW);
    ctx->cachedVideoOutHeight = static_cast<uint32_t>(outH);
    ctx->cachedSuperSlowFrameNum = static_cast<uint32_t>(frameNum);

    // 原始 so_32 @ 0x19a24: 先呼叫 stop() 重置 gateway 狀態
    int ret = ctx->cacao->stop();
    if (ret != 0) {
        ALOGE("%s: cacao stop() ret=%d", __func__, ret);
        if (ret == -0x6e) {  // PAL_ERR_ALREADY
            return (int)0xfffffffe;  // -2
        }
    }

    cacao::ProcessCtrlMode ctrlMode;
    ctrlMode.field_08 = getSuperSlowModeType(mode);
    ctrlMode.field_0c = (int32_t)ctx->cameraMode;  // camera index (facing)
    ctrlMode.field_10 = (uint32_t)inW;
    ctrlMode.field_14 = (uint32_t)inH;
    ctrlMode.field_18 = (uint32_t)outW;
    ctrlMode.field_1c = (uint32_t)outH;
    ctrlMode.field_20 = 1;
    ctrlMode.field_24 = (uint32_t)fps;
    ctrlMode.field_28 = (uint32_t)flags;

    ret = ctx->cacao->start(&ctrlMode);
    if (ret != 0) {
        ALOGE("%s: cacao start() failed. ERR=%d", __func__, ret);
        return -1;
    }
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraVideo_prepareVideoRecording
// 來自 so_32 @ 0x00009b55
// params: surface（輸出 GraphicBufferProducer），p1-p3（寬/高/格式等）
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraVideo_prepareVideoRecording(JNIEnv* env,
                                             imageprocessor::BypassCameraContext* ctx,
                                             jobject surface,
                                             jint p1, jint p2, jint p3) {
    if (!ctx || !ctx->cacao) return -1;

    if (!imageprocessor::isVideoThreadDirect() && ctx->videoCommandThreadRunning) {
        imageprocessor::enqueueVideoCommand(env, ctx,
                imageprocessor::VideoThreadCommandKind::PrepareVideo,
                surface, p1, p2, p3);
        return 0;
    }

    // cmd 0 (prepareVideoRec)：使用 ctx+0x64 (videoFinishCb slot) 的回呼
    if (!ctx->videoFinishCb) {
        ALOGE("BypassCameraVideo_prepareVideoRecording: not initialized");
        return -1;
    }

    // dispatcher case 0：ProcessCtrlVideoRecParam，_data[0]=10，_extra[2]=dataspace
    cacao::ProcessCtrlVideoRecParam param;
    // type = 10 (prepareVideoRec)，通過 onProcess switch 路由至 prepareVideoRec handler
    *reinterpret_cast<uint32_t*>(param._data) = 10;
    // p1=dataspace hint (or 0), p2=w, p3=h
    // 64-bit: _extra starts at this+0x168, [0,1]=sp.m_ptr, [2]=dataspace@0x170, [3]=frameRate@0x174
    param._extra[2] = (uint32_t)p1;  // dataspace: 0x12060000/0x102/0x104
    (void)p2; (void)p3;

    if (!ctx->videoFinishResult) {
        ctx->videoFinishResult = createResultWithCtx(ctx);
    }

    android::sp<android::IBinder> producerBinder;
    if (!getSurfaceProducerBinder(env, surface, &producerBinder)) {
        return -1;
    }

    ctx->cacao->processAsyncWithBinder(
            &param,
            producerBinder,
            ctx->videoFinishCb,
            static_cast<cacao::ProcessCtrlResult*>(ctx->videoFinishResult));
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraVideo_prepareSuperSlowRecording
// 來自 so_32 @ 0x00009cd9
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraVideo_prepareSuperSlowRecording(JNIEnv* env,
                                                 imageprocessor::BypassCameraContext* ctx,
                                                 jobject surface,
                                                 jint p1, jint p2, jint p3) {
    if (!ctx || !ctx->cacao) return -1;
    ALOGD("BypassCameraVideo_prepareSuperSlowRecording: threadDirect=%d running=%d p1=%d p2=%d p3=%d surface=%p",
          imageprocessor::isVideoThreadDirect() ? 1 : 0,
          ctx->videoCommandThreadRunning ? 1 : 0,
          (int)p1, (int)p2, (int)p3, surface);

    if (!imageprocessor::isVideoThreadDirect() && ctx->videoCommandThreadRunning) {
        imageprocessor::enqueueVideoCommand(env, ctx,
                imageprocessor::VideoThreadCommandKind::PrepareSuperSlow,
                surface, p1, p2, p3);
        return 0;
    }

    // cmd 4 (prepareSuperSlow)：使用 ctx+0x7c (videoStartRecCb slot) 的回呼
    if (!ctx->videoStartRecCb) {
        ALOGE("BypassCameraVideo_prepareSuperSlowRecording: not initialized");
        return -1;
    }

        // dispatcher case 4：原始 worker 以 cached 輸出尺寸推導 dataspace，並使用 cached frameNum
    cacao::ProcessCtrlVideoRecParam param;
    // type = 10 (prepareVideoRec)，與 cmd 0 相同的 handler
    *reinterpret_cast<uint32_t*>(param._data) = 10;
        param._extra[2] = getSuperSlowDataspace(ctx);  // 64-bit: dataspace@_extra[2]=this+0x170
        param._extra[3] = ctx->cachedSuperSlowFrameNum; // 64-bit: frameRate@_extra[3]=this+0x174

        ALOGD("BypassCameraVideo_prepareSuperSlowRecording: cached out=%ux%u dataspace=0x%x frameNum=%u",
            ctx->cachedVideoOutWidth,
            ctx->cachedVideoOutHeight,
            param._extra[2],
            param._extra[3]);
        (void)p1;
        (void)p2;
        (void)p3;

    if (!ctx->videoStartRecResult) {
        ctx->videoStartRecResult = createResultWithCtx(ctx);
    }

    android::sp<android::IBinder> producerBinder;
    if (!getSurfaceProducerBinder(env, surface, &producerBinder)) {
        return -1;
    }

    ctx->cacao->processAsyncWithBinder(
            &param,
            producerBinder,
            ctx->videoStartRecCb,
            static_cast<cacao::ProcessCtrlResult*>(ctx->videoStartRecResult));
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraVideo_startVideoRecording
// 來自 so_32 @ 0x00009dc5
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraVideo_startVideoRecording(imageprocessor::BypassCameraContext* ctx) {
    if (!ctx || !ctx->cacao) return -1;

    if (!imageprocessor::isVideoThreadDirect() && ctx->videoCommandThreadRunning) {
        imageprocessor::enqueueVideoCommand(nullptr, ctx,
                imageprocessor::VideoThreadCommandKind::StartVideo,
                nullptr, 0, 0, 0);
        return 0;
    }

    // cmd 1 (startVideoRec)：使用 ctx+0x74 (videoStartSuperSlowCb slot) 的回呼
    if (!ctx->videoStartSuperSlowCb) {
        ALOGE("BypassCameraVideo_startVideoRecording: not initialized");
        return -1;
    }

    if (!ctx->videoStartSuperSlowResult) {
        ctx->videoStartSuperSlowResult = createResultWithCtx(ctx);
    }

    // dispatcher case 1：ProcessCtrlVideoRecParam，_data[0]=11 (startVideoRec)
    cacao::ProcessCtrlVideoRecParam param;
    *reinterpret_cast<uint32_t*>(param._data) = 11;
    // dispatcher case 1：ctx+0x74 cb，ctx+0x78 result
        ctx->cacao->processAsync(
            &param,
            nullptr,
            ctx->videoStartSuperSlowCb,
            static_cast<cacao::ProcessCtrlResult*>(ctx->videoStartSuperSlowResult));
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraVideo_startSuperSlowRecording
// 來自 so_32 @ 0x00009df9
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraVideo_startSuperSlowRecording(imageprocessor::BypassCameraContext* ctx) {
    if (!ctx || !ctx->cacao) return -1;
    ALOGD("BypassCameraVideo_startSuperSlowRecording: threadDirect=%d running=%d",
          imageprocessor::isVideoThreadDirect() ? 1 : 0,
          ctx->videoCommandThreadRunning ? 1 : 0);

    if (!imageprocessor::isVideoThreadDirect() && ctx->videoCommandThreadRunning) {
        imageprocessor::enqueueVideoCommand(nullptr, ctx,
                imageprocessor::VideoThreadCommandKind::StartSuperSlow,
                nullptr, 0, 0, 0);
        return 0;
    }

    // cmd 5 (startSuperSlow)：使用 ctx+0x84 (videoSuperSlowCb slot) 的回呼
    if (!ctx->videoSuperSlowCb) {
        ALOGE("BypassCameraVideo_startSuperSlowRecording: not initialized");
        return -1;
    }

    if (!ctx->videoSuperSlowResult) {
        ctx->videoSuperSlowResult = createResultWithCtx(ctx);
    }

    // dispatcher case 5：ProcessCtrlVideoRecParam，_data[0]=13 (triggerSuperSlow)
    cacao::ProcessCtrlVideoRecParam param;
    *reinterpret_cast<uint32_t*>(param._data) = 13;
    // dispatcher case 5：ctx+0x84 cb，ctx+0x88 result
        ctx->cacao->processAsync(
            &param,
            nullptr,
            ctx->videoSuperSlowCb,
            static_cast<cacao::ProcessCtrlResult*>(ctx->videoSuperSlowResult));
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraVideo_stopVideoRecording
// 來自 so_32（stop 操作通知 VideoFinishCallback）
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraVideo_stopVideoRecording(imageprocessor::BypassCameraContext* ctx) {
    if (!ctx || !ctx->cacao) return -1;

    if (!imageprocessor::isVideoThreadDirect() && ctx->videoCommandThreadRunning) {
        imageprocessor::enqueueVideoCommand(nullptr, ctx,
                imageprocessor::VideoThreadCommandKind::StopVideo,
                nullptr, 0, 0, 0);
        return 0;
    }

    // cmd 2 (stopVideoRec)：使用 ctx+0x6c (videoPrepareCb slot) 的回呼
    if (!ctx->videoPrepareCb) {
        ALOGE("BypassCameraVideo_stopVideoRecording: not initialized");
        return -1;
    }

    if (!ctx->videoPrepareResult) {
        ctx->videoPrepareResult = createResultWithCtx(ctx);
    }

    // dispatcher case 2：ProcessCtrlVideoRecParam，_data[0]=12 (stopVideoRec)
    // 設定 type=12 使 onProcess switch 路由至 stopVideoRec handler，避免 passRequest cald「already starting」錯誤
    cacao::ProcessCtrlVideoRecParam param;
    *reinterpret_cast<uint32_t*>(param._data) = 12;
    // dispatcher case 2：ctx+0x6c cb，ctx+0x70 result
        ctx->cacao->processAsync(
            &param,
            nullptr,
            ctx->videoPrepareCb,
            static_cast<cacao::ProcessCtrlResult*>(ctx->videoPrepareResult));
    return 0;
}
