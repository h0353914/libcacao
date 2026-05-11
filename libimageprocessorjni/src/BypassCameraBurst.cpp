//
// BypassCameraBurst.cpp — 連拍模式操作
// 依照 so_32 Ghidra 反編譯結果重建
//

#include <android/log.h>
#include <jni.h>

#include <imageprocessor/BypassCamera.h>
#include <cacao/CacaoClient.h>
#include <cacao/ProcessCtrlGatewayBase.h>

#define LOG_TAG "libimageprocessorjni_real"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

extern cacao::ProcessCtrlResult* createResultWithCtx(imageprocessor::BypassCameraContext* ctx);

// ─────────────────────────────────────────────────────
// BypassCameraBurst_initialize
// 來自 so_32 @ 0x0000b1bd
// 建立 burst-shot callback 物件並分配 ProcessCtrlResult
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraBurst_initialize(JNIEnv* /*env*/, jobject /*thiz*/,
                                  imageprocessor::BypassCameraContext* ctx) {
    if (!ctx) return -1;

    if (!ctx->burstPrepareCb) {
        ctx->burstPrepareCb = new imageprocessor::BurstShotPrepareCallback();
    }
    if (!ctx->burstFinishCb) {
        ctx->burstFinishCb = new imageprocessor::BurstShotFinishCallback();
    }
    if (!ctx->burstCb) {
        ctx->burstCb = new imageprocessor::BurstCallback();
    }

    if (!ctx->burstPrepareResult) {
        ctx->burstPrepareResult = createResultWithCtx(ctx);
    }
    if (!ctx->burstFinishResult) {
        ctx->burstFinishResult = createResultWithCtx(ctx);
    }

    ALOGD("BypassCameraBurst_initialize: OK");
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraBurst_finalize
// ─────────────────────────────────────────────────────
extern "C" void BypassCameraBurst_finalize(imageprocessor::BypassCameraContext* ctx) {
    (void)ctx;
    // 實際釋放在 BypassCameraContext::~BypassCameraContext() 中完成
}

// ─────────────────────────────────────────────────────
// BypassCameraPhoto_prepareBurstShot
// 來自 so_32（nativeRequestPrepareBurstShot(J)I → BypassCameraPhoto_prepareBurstShot）
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraPhoto_prepareBurstShot(imageprocessor::BypassCameraContext* ctx) {
    if (!ctx || !ctx->cacao) return -1;
    if (!ctx->burstPrepareCb) {
        ALOGE("BypassCameraPhoto_prepareBurstShot: not initialized");
        return -1;
    }

    if (!ctx->burstPrepareResult) {
        ctx->burstPrepareResult = createResultWithCtx(ctx);
    }

    cacao::ProcessCtrlParam param;
    ctx->cacao->process(
            &param,
            ctx->burstPrepareCb,
            static_cast<cacao::ProcessCtrlResult*>(ctx->burstPrepareResult));
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraPhoto_finishBurstShot
// 來自 so_32（nativeRequestFinishBurstShot → 此函式）
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraPhoto_finishBurstShot(imageprocessor::BypassCameraContext* ctx) {
    if (!ctx || !ctx->cacao) return -1;
    if (!ctx->burstFinishCb) {
        ALOGE("BypassCameraPhoto_finishBurstShot: not initialized");
        return -1;
    }

    if (!ctx->burstFinishResult) {
        ctx->burstFinishResult = createResultWithCtx(ctx);
    }

    cacao::ProcessCtrlParam param;
    ctx->cacao->process(
            &param,
            ctx->burstFinishCb,
            static_cast<cacao::ProcessCtrlResult*>(ctx->burstFinishResult));
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraBurst_requestSnapshot
// 來自 so_32 @ 0x0000b291
// 遞增 requestId，取 jstring UTF chars，調用 cacao burst snapshot 請求
// 參數分析依照 ARM 反組譯：r0=env, r1=thiz, r2=ctx, r3=p4(jboolean)
// 棧上: p5(jboolean), [pad], p6-p8(jdouble×3), p9(jboolean), p10(jstring),
//       p11-p12(jint), p13-p14(jboolean×jint), ...
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraBurst_requestSnapshot(
        JNIEnv* env, jobject /*thiz*/,
        imageprocessor::BypassCameraContext* ctx,
        jboolean p4, jboolean p5,
        jdouble p6, jdouble p7, jdouble p8,
        jboolean p9, jstring p10,
        jint p11, jint p12,
        jboolean p13, jint p14,
        jboolean p15, jboolean p16, jboolean p17, jboolean p18,
        jint p19, jboolean p20, jint p21) {
    (void)p4; (void)p5; (void)p6; (void)p7; (void)p8;
    (void)p9; (void)p10; (void)p11; (void)p12;
    (void)p13; (void)p14; (void)p15; (void)p16; (void)p17; (void)p18;
    (void)p19; (void)p20; (void)p21;
    (void)env;
    if (!ctx || !ctx->cacao) {
        ALOGE("BypassCameraBurst_requestSnapshot: null ctx or cacao");
        return -1;
    }
    if (!ctx->burstCb) {
        ALOGE("BypassCameraBurst_requestSnapshot: not initialized");
        return -1;
    }

    ctx->field_C0++;

    if (!ctx->burstPrepareResult) {
        ctx->burstPrepareResult = createResultWithCtx(ctx);
    }

    cacao::ProcessCtrlParam param;
    ctx->cacao->process(
            &param,
            ctx->burstCb,
            static_cast<cacao::ProcessCtrlResult*>(ctx->burstPrepareResult));
    return 0;
}
