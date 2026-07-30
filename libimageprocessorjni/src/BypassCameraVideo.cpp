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

// so_32 @ 0x19fd8（BypassCameraVideo_thread worker，case 4/PrepareSuperSlow）反編譯
// 確認：基礎值是 0x103（非 0x102，0x102 是 case 0/PrepareVideo 專用的基礎值），
// 像素數門檻與 override 值(0x104)則與 PrepareVideo 一致。
static uint32_t getSuperSlowDataspace(const imageprocessor::BypassCameraContext* ctx) {
    if (!ctx) {
        return 0x103;
    }

    const uint64_t pixelCount =
        static_cast<uint64_t>(ctx->cachedVideoOutWidth) *
        static_cast<uint64_t>(ctx->cachedVideoOutHeight);
    return pixelCount > 0x000e0fffull ? 0x104u : 0x103u;
}

// [已確認] 反編譯 so_32 @ 0x19fd8（BypassCameraVideo_thread 內 case 0 = PrepareVideo）：
// dataspace 並非直接取自 JNI 傳入的 p1（先前的實作是錯的），而是依 p1/p2/p3 是否
// 全部為 0、以及 cached 輸出寬高像素數判斷：
//   若 p1==0 || p2==0 || p3==0            → 0x12060000
//   否則若 cachedW*cachedH > 0xe0fff(px)  → 0x104
//   否則                                    → 0x102
// 用原本的「直接把 p1 當 dataspace」在 120fps 連續慢動作模式下會送出不被
// cacao 接受的 dataspace（logcat: "[CAC] Specified dataspace is not supported"），
// 導致 prepareVideoRec 失敗、STATE_錄影計時 永遠不出現。
static uint32_t getVideoRecDataspace(const imageprocessor::BypassCameraContext* ctx,
                                      jint p1, jint p2, jint p3) {
    if (p1 == 0 || p2 == 0 || p3 == 0) {
        return 0x12060000u;
    }
    if (!ctx) {
        return 0x102u;
    }
    const uint64_t pixelCount =
        static_cast<uint64_t>(ctx->cachedVideoOutWidth) *
        static_cast<uint64_t>(ctx->cachedVideoOutHeight);
    return pixelCount > 0x000e0fffull ? 0x104u : 0x102u;
}

static uint32_t getSuperSlowModeType(jint mode) {
    return static_cast<uint32_t>(mode) + 4u;
}

// so_32 @ 0x1738c (Java_..._nativeChangeToVideoMode) 反編譯確認：mode（Java VideoMode
// ordinal，範圍 0-4，呼叫端已驗證 <5 才會呼叫這裡）在送進 ProcessCtrlMode 前，會先
// 透過查表轉換成實際送給 cald 的值，而非直接使用原始 ordinal。表內容位於 0x20570，
// 5 個 uint32：{1, 2, 3, 1, 2}（原版若 mode>=5 會直接記 log 並回傳 -1，不會走到這裡）。
static uint32_t getVideoModeType(jint mode) {
    static const uint32_t kTable[5] = {1, 2, 3, 1, 2};
    return kTable[mode];
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
    // so_32 @ 0x195b8 反編譯確認：原版用 FindClass(固定類別名字串，位於 0x1ef03
    // "com/sonymobile/imageprocessor/bypasscamera2/BypassCamera") 而非 GetObjectClass(thiz)。
    if (!ctx->videoMethodId) {
        jclass clazz = env->FindClass(
                "com/sonymobile/imageprocessor/bypasscamera2/BypassCamera");
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

    // so_32 反編譯確認：prepareVideoRecording/startVideoRecording/stopVideoRecording/
    // prepareSuperSlowRecording/startSuperSlowRecording 這 5 個公開函式在原版都只是
    // 呼叫 BypassCameraVideo_pushThreadRequest 把命令排進佇列就立刻返回，真正的
    // cacao 呼叫在這個 worker thread 內非同步執行。之前的實作漏了啟動這個
    // thread，導致下面那 5 個函式改成直接同步呼叫 cacao（見各函式內註解），
    // 這個時序落差會讓 cald 收到請求的順序/時機跟原版不同。
    if (!ctx->videoCommandThreadRunning) {
        ctx->videoCommandThreadRunning = true;
        if (pthread_create(&ctx->videoCommandThread, nullptr,
                            imageprocessor::BypassCameraVideo_worker, ctx) != 0) {
            ALOGE("BypassCameraVideo_initialize: pthread_create failed");
            ctx->videoCommandThreadRunning = false;
            return -1;
        }
    }

    ctx->videoInitialized = true;
    ALOGD("BypassCameraVideo_initialize: OK");
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraVideo_finalize
// 來自 so_32 @ 0x00019910
// 停止並 join video command worker thread，釋放 video 相關 callback、
// ProcessCtrlResult 與 JNI global ref，供下次 BypassCameraVideo_initialize
// 重新配置（ctx 為常駐 singleton，這裡才是實際釋放的地方）
// ─────────────────────────────────────────────────────
extern "C" void BypassCameraVideo_finalize(JNIEnv* env, jobject /*thiz*/, imageprocessor::BypassCameraContext* ctx) {
    if (!ctx) return;

    if (ctx->videoInitialized) {
        if (ctx->videoCommandThreadRunning) {
            pthread_mutex_lock(&ctx->videoCommandLock);
            ctx->videoCommandThreadRunning = false;
            pthread_cond_broadcast(&ctx->videoCommandCond);
            pthread_mutex_unlock(&ctx->videoCommandLock);
            pthread_join(ctx->videoCommandThread, nullptr);
        }
        ctx->videoInitialized = false;
    }

    while (!ctx->videoCommandQueue.empty()) {
        delete ctx->videoCommandQueue.front();
        ctx->videoCommandQueue.pop_front();
    }

    if (ctx->videoJObj) {
        if (env) env->DeleteGlobalRef(ctx->videoJObj);
        ctx->videoJObj = nullptr;
    }

    delete ctx->videoFinishCb;         ctx->videoFinishCb         = nullptr;
    delete ctx->videoPrepareCb;        ctx->videoPrepareCb        = nullptr;
    delete ctx->videoStartSuperSlowCb; ctx->videoStartSuperSlowCb = nullptr;
    delete ctx->videoStartRecCb;       ctx->videoStartRecCb       = nullptr;
    delete ctx->videoSuperSlowCb;      ctx->videoSuperSlowCb      = nullptr;

    delete static_cast<cacao::ProcessCtrlResult*>(ctx->videoFinishResult);         ctx->videoFinishResult         = nullptr;
    delete static_cast<cacao::ProcessCtrlResult*>(ctx->videoPrepareResult);        ctx->videoPrepareResult        = nullptr;
    delete static_cast<cacao::ProcessCtrlResult*>(ctx->videoStartSuperSlowResult); ctx->videoStartSuperSlowResult = nullptr;
    delete static_cast<cacao::ProcessCtrlResult*>(ctx->videoStartRecResult);       ctx->videoStartRecResult       = nullptr;
    delete static_cast<cacao::ProcessCtrlResult*>(ctx->videoSuperSlowResult);      ctx->videoSuperSlowResult      = nullptr;
}

// ─────────────────────────────────────────────────────
// BypassCameraVideo_changeToVideoMode
// 來自 so_32 @ 0x0000996d
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraVideo_changeToVideoMode(imageprocessor::BypassCameraContext* ctx,
                                         jint mode, jint inW, jint inH,
                                         jint outW, jint outH, jint flags) {
    if (!ctx || !ctx->cacao) return -1;

    // so_32 @ 0x1738c (Java_..._nativeChangeToVideoMode) 反編譯確認：mode>=5 時原版
    // 直接記 log 並回傳 -1，完全不呼叫 cacao stop()/start()。
    if (mode < 0 || mode >= 5) {
        ALOGE("%s: invalid mode=%d", __func__, mode);
        return -1;
    }

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
    ctrlMode.field_08 = getVideoModeType(mode);
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
// 來自 so_32 @ 0x19b54（原文件標示 0x00009b55 缺一位數）
// params: surface（輸出 GraphicBufferProducer），p1-p3：不是寬/高/格式，而是
// 用來判斷 dataspace 該用哪個公式的旗標（見 getVideoRecDataspace）
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraVideo_prepareVideoRecording(JNIEnv* env,
                                             imageprocessor::BypassCameraContext* ctx,
                                             jobject surface,
                                             jint p1, jint p2, jint p3) {
    if (!ctx || !ctx->cacao) return -1;

    // so_32 @ 0x19b54 反編譯確認：原版這個函式只是把命令（kind=0/PrepareVideo）
    // 推進 videoCommandQueue 就立刻返回，真正呼叫 cacao 是 worker thread 非同步
    // 執行的（下面 isVideoThreadDirect()==true 那個分支，由 worker thread 呼叫）。
    if (!imageprocessor::isVideoThreadDirect()) {
        imageprocessor::enqueueVideoCommand(env, ctx,
            imageprocessor::VideoThreadCommandKind::PrepareVideo, surface, p1, p2, p3);
        return 0;
    }

    // cmd 0 (prepareVideoRec)：使用 ctx+0x64 (videoFinishCb slot) 的回呼
    if (!ctx->videoFinishCb) {
        ALOGE("BypassCameraVideo_prepareVideoRecording: not initialized");
        return -1;
    }

    if (!ctx->videoFinishResult) {
        ctx->videoFinishResult = createResultWithCtx(ctx);
    }

    // dispatcher case 0：ProcessCtrlVideoRecParam，_data[0]=10，_extra[2]=dataspace
    cacao::ProcessCtrlVideoRecParam param;
    // type = 10 (prepareVideoRec)，通過 onProcess switch 路由至 prepareVideoRec handler
    *reinterpret_cast<uint32_t*>(param._data) = 10;
    // p1/p2/p3 本身不是 dataspace/寬/高，而是「是否全部非 0」的旗標，實際 dataspace
    // 依 getVideoRecDataspace() 計算（詳見該函式反編譯依據的註解）
    // 64-bit: _extra starts at this+0x168, [0,1]=sp.m_ptr, [2]=dataspace@0x170, [3]=frameRate@0x174
    param._extra[2] = getVideoRecDataspace(ctx, p1, p2, p3);

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

    // so_32 @ 0x19cd8 反編譯確認：原版只是把命令（kind=4/PrepareSuperSlow）推進
    // videoCommandQueue 就返回，真正呼叫 cacao 由 worker thread 非同步執行。
    if (!imageprocessor::isVideoThreadDirect()) {
        imageprocessor::enqueueVideoCommand(env, ctx,
            imageprocessor::VideoThreadCommandKind::PrepareSuperSlow, surface, p1, p2, p3);
        return 0;
    }

    ALOGD("BypassCameraVideo_prepareSuperSlowRecording: p1=%d p2=%d p3=%d surface=%p",
          (int)p1, (int)p2, (int)p3, surface);

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

    // so_32 @ 0x19dc4 反編譯確認：原版只是把命令（kind=1/StartVideo）推進
    // videoCommandQueue 就返回（函式本體只有兩行），真正呼叫 cacao 由 worker
    // thread 非同步執行。這是 slow_120 卡在 STATE_錄影計時 逾時的關鍵根因：
    // 之前直接同步呼叫 processAsync()，跳過了佇列序列化，導致這個 startVideoRec
    // 請求送到 cald 的時機跟排隊等待中的 super-slow 相關請求時序不同於原版，
    // 使 cald 對這次請求回覆 result[5]==0（命中 mode==0 排除邏輯）而非
    // result[5]==8（正常完成路徑）。
    if (!imageprocessor::isVideoThreadDirect()) {
        imageprocessor::enqueueVideoCommand(nullptr, ctx,
            imageprocessor::VideoThreadCommandKind::StartVideo, nullptr, 0, 0, 0);
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

    // dispatcher case 1：反編譯 so_32 @ 0x19fd8 (BypassCameraVideo_thread) 確認建構子
    // 符號為 cacao::ProcessCtrlParam::ProcessCtrlParam（非 ProcessCtrlVideoRecParam），
    // 且該 case 完全沒有寫入任何額外欄位。之前誤用 ProcessCtrlVideoRecParam 會多序列化
    // 8 bytes（_extra[1]/_extra[2]，dataspace/frameRate 皆為 0），送給 cald 的 start
    // 請求 payload 比原版多 8 bytes，改用陽春版 ProcessCtrlParam 修正。
    cacao::ProcessCtrlParam param;
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

    // so_32 @ 0x19df8 反編譯確認：函式本體只有兩行——
    // `local_10 = 5; BypassCameraVideo_pushThreadRequest(ctx, &local_10);`——
    // 這個 5 是「送進本模組內部 video 工作執行緒佇列」的內部標籤（對應
    // VideoThreadCommandKind::StartSuperSlow=5），不是送給 cacao 的
    // ProcessCtrlVideoRecParam type 值；worker thread 收到這個佇列項目後才會
    // 呼叫這個函式本體（下面 isVideoThreadDirect()==true 分支），內部真正送給
    // cacao 的 type 是 13（triggerSuperSlow，見 ProcessCtrlGateway.cpp onProcess
    // 分派表）。之前只修正了 type 值（13）但一直漏了佇列本身沒有真的被使用
    // （worker thread 從未啟動），現已一併補上。
    if (!imageprocessor::isVideoThreadDirect()) {
        imageprocessor::enqueueVideoCommand(nullptr, ctx,
            imageprocessor::VideoThreadCommandKind::StartSuperSlow, nullptr, 0, 0, 0);
        return 0;
    }

    ALOGD("BypassCameraVideo_startSuperSlowRecording");

    // cmd 5 (startSuperSlow)：使用 ctx+0x84 (videoSuperSlowCb slot) 的回呼
    if (!ctx->videoSuperSlowCb) {
        ALOGE("BypassCameraVideo_startSuperSlowRecording: not initialized");
        return -1;
    }

    if (!ctx->videoSuperSlowResult) {
        ctx->videoSuperSlowResult = createResultWithCtx(ctx);
    }

    // 反編譯 so_32 @ 0x19fd8 case 5 確認建構子符號為 cacao::ProcessCtrlParam::ProcessCtrlParam
    // （非 ProcessCtrlVideoRecParam），同 case 1 的理由改用陽春版。
    cacao::ProcessCtrlParam param;
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

    // so_32 @ 0x19e2c 反編譯確認：原版只是把命令（kind=2/StopVideo）推進
    // videoCommandQueue 就返回，真正呼叫 cacao 由 worker thread 非同步執行。
    if (!imageprocessor::isVideoThreadDirect()) {
        imageprocessor::enqueueVideoCommand(nullptr, ctx,
            imageprocessor::VideoThreadCommandKind::StopVideo, nullptr, 0, 0, 0);
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

    // dispatcher case 2：設定 type=12 使 onProcess switch 路由至 stopVideoRec handler，
    // 避免 passRequest cald「already starting」錯誤。反編譯 so_32 @ 0x19fd8 case 2
    // (LAB_0001a094 共用路徑) 確認建構子符號為 cacao::ProcessCtrlParam::ProcessCtrlParam
    // （非 ProcessCtrlVideoRecParam），同 case 1 的理由改用陽春版。
    cacao::ProcessCtrlParam param;
    *reinterpret_cast<uint32_t*>(param._data) = 12;
    // dispatcher case 2：ctx+0x6c cb，ctx+0x70 result
        ctx->cacao->processAsync(
            &param,
            nullptr,
            ctx->videoPrepareCb,
            static_cast<cacao::ProcessCtrlResult*>(ctx->videoPrepareResult));
    return 0;
}
