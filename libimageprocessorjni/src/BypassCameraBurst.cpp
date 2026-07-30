//
// BypassCameraBurst.cpp — 連拍模式操作
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

extern cacao::ProcessCtrlResult* createResultWithCtx(imageprocessor::BypassCameraContext* ctx);

// 宣告 BypassCameraBurstBufferManager.cpp 中定義的函式
extern "C" int BypassCameraBurstBufferManager_createBufVector(
        imageprocessor::BypassCameraContext* ctx,
        android::Vector<cacao::ImageBuf*>** outVec,
        int count);
extern "C" imageprocessor::BufEntry* BypassCameraBurstBufferManager_findByNativeHandle(
        imageprocessor::BypassCameraContext* ctx, void* nativeHandle);
extern "C" int BypassCameraBurstBufferManager_queueBuffer(
        imageprocessor::BypassCameraContext* ctx,
        imageprocessor::BufEntry* entry);
extern "C" void BypassCameraBurstBufferManager_dump(
        imageprocessor::BypassCameraContext* ctx);

// ─────────────────────────────────────────────────────
// BypassCameraBurst_initialize
// 來自 so_32 @ 0x1b1bc（原文件誤標 0x0000b1bd，缺一位數）
// 建立 burst-shot callback 物件並分配 ProcessCtrlResult；另外重設
// requestCounter(ctx+0xC0)、設定 burstInitialized(ctx+0xBC)=true，並建立
// burst 模組自己的一份 JNI global ref/methodID(ctx+0xC8/0xCC) 與 listener
// 物件(ctx+0xD0，供 BypassCameraBurst_requestSnapshot 使用)。
// ─────────────────────────────────────────────────────
extern "C" int BypassCameraBurst_initialize(JNIEnv* env, jobject thiz,
                                  imageprocessor::BypassCameraContext* ctx) {
    if (!ctx) return -1;

    ctx->field_C0 = 0;
    ctx->burstInitialized = true;

    // burst 模組專用的 global ref（與 photoJObj/videoJObj 同構但獨立）
    if (!ctx->burstJObj && env && thiz) {
        ctx->burstJObj = env->NewGlobalRef(thiz);
    }

    // burst 模組專用的 callbackFromNative methodID（反編譯確認用
    // FindClass 而非 GetObjectClass(thiz)，與 Photo/Video 取得方式不同）
    if (!ctx->burstMethodId && env) {
        jclass clazz = env->FindClass(
                "com/sonymobile/imageprocessor/bypasscamera2/BypassCamera");
        if (clazz) {
            ctx->burstMethodId = env->GetMethodID(clazz,
                    "callbackFromNative", "(IIIZZZIII)V");
            env->DeleteLocalRef(clazz);
        }
    }

    if (!ctx->burstSnapshotCb) {
        ctx->burstSnapshotCb = new imageprocessor::BurstCallback();
    }

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
// 來自 so_32 @ 0x1b238（原文件標示 0x1b264 為函式中段位址，
// decompile_function 仍可正確解析出完整函式本體）
// ctx+0xe8 = bufCtx(+0xd4) + 0x14，經反編譯
// BypassCameraBufferContext::BypassCameraBufferContext()（so_32 @ 0x18014）
// 逐欄位比對確認：+0x00/+0x04 buffers(Vector), +0x08 sp<Surface>,
// +0x0c pthread_mutex_t, +0x10 ready(1 byte)+3 byte pad, +0x14 為此處對應
// 的第一個 uint32_t（即 dequeueCount）。原版此值非 0 時只印一筆診斷 log
// （不影響記憶體清理），格式字串與函式名稱皆從 rodata 逐字讀出比對。
// ─────────────────────────────────────────────────────
extern "C" void BypassCameraBurst_finalize(JNIEnv* env, jobject /*thiz*/,
                                            imageprocessor::BypassCameraContext* ctx) {
    if (!ctx) return;

    if (ctx->bufCtx.dequeueCount != 0) {
        __android_log_print(ANDROID_LOG_WARN, NULL,
            "%s: wrong mDequeueCounter=%d",
            "BypassCameraBurst_finalize", ctx->bufCtx.dequeueCount);
    }

    ctx->burstInitialized = false;

    if (ctx->burstJObj) {
        if (env) env->DeleteGlobalRef(ctx->burstJObj);
        ctx->burstJObj = nullptr;
    }

    delete ctx->burstSnapshotCb;
    ctx->burstSnapshotCb = nullptr;
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
// 來自 so_32 @ 0x1b290（原文件標示 0x0000b291，缺一位數）
// 完整反編譯後確認的 dispatch 形狀：
//   1. listener 是 ctx->burstSnapshotCb（ctx+0xD0），不是 ctx->burstCb。
//   2. 每次呼叫都用 BypassCameraBurstBufferManager_createBufVector 建立
//      一批 buffer（數量 = p21），並各自重新配置一個新的
//      ProcessCtrlResult，userData 帶上 BurstRequestData{ctx, bufVec,
//      requestId} wrapper（原版是 {ctx,0,bufVectorPtr,requestId} 4-word
//      結構，中間固定 0 的 word 用途未確認、我們省略不影響行為）。
//   3. dispatch 呼叫是 processAsync(param, bufVec, listener, result) ——
//      跟 photo 用的是同一個 overload（vtable slot 8 = processAsync），
//      不是另一個 4 參數 overload。
//   4. dispatch 後逐一用 imageBuf->getNative() 查回 BufEntry，標記
//      state=2(in_use)、tag=這次的 requestId（ctx->field_C0，遞增後的值）。
//
// [仍未確認] p11..p20（縮圖/畫質/方向等 EXIF 附加欄位）反編譯出的封包
// 順序跟 photo 版不同、且沒有連拍對應的 Java smali 可交叉比對 consumer
// 端固定 offset，無法安全地按位置對應——猜錯的風險是拍出帶錯誤 EXIF/
// 畫質參數的連拍照片，且目前沒有自動化測試能偵測這種問題。因此這裡只
// 實作反編譯高信心度確認的部分（GPS 座標/字串、buffer dispatch 形狀），
// 其餘 EXIF 欄位維持 ProcessCtrlParam 建構子的預設值（等同關閉這些
// 選項），留給之後能實際手動測試連拍功能時再補齊。
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
    (void)p11; (void)p12; (void)p13; (void)p14;
    (void)p15; (void)p16; (void)p17; (void)p18; (void)p19; (void)p20;

    if (!ctx || !ctx->cacao) {
        ALOGE("BypassCameraBurst_requestSnapshot: null ctx or cacao");
        return -1;
    }
    if (!ctx->burstSnapshotCb) {
        ALOGE("BypassCameraBurst_requestSnapshot: not initialized");
        return -1;
    }

    ctx->field_C0++;
    const uint32_t requestId = ctx->field_C0;

    cacao::ProcessCtrlParam param;
    uint8_t* d = param._data;
    *reinterpret_cast<uint32_t*>(d) = 4;  // mode = 4（跟 photo 共用同一種 ProcessCtrlParam 格式）

    // GPS 座標/字串：與 photo 相同的 consumer 端固定 offset，高信心度對應。
    d[8] = p4 ? 1 : 0;
    d[9] = p5 ? 1 : 0;
    *reinterpret_cast<double*>(d + 0x10) = p6;  // exifGpsLatitude
    *reinterpret_cast<double*>(d + 0x18) = p7;  // exifGpsLongitude
    *reinterpret_cast<double*>(d + 0x20) = p8;  // exifGpsAltitude
    d[0x28] = p9 ? 1 : 0;

    memset(d + 0x29, 0, 0x100);
    if (p10) {
        const char* str = env->GetStringUTFChars(p10, nullptr);
        if (str) {
            strncpy(reinterpret_cast<char*>(d + 0x29), str, 0xFF);
            env->ReleaseStringUTFChars(p10, str);
        }
    }

    android::Vector<cacao::ImageBuf*>* bufVec = nullptr;
    int got = BypassCameraBurstBufferManager_createBufVector(ctx, &bufVec, static_cast<int>(p21));
    if (got == 0) {
        ALOGE("BypassCameraBurst_requestSnapshot: createBufVector got 0 buffers");
        delete bufVec;
        return -1;
    }

    // 每次請求專屬的 wrapper + ProcessCtrlResult（刻意不在 callback 內
    // delete，見 BurstRequestData 的說明）。
    auto* reqData = new imageprocessor::BurstRequestData{ctx, bufVec, requestId};
    cacao::ProcessCtrlResult* result = new cacao::ProcessCtrlResult();
    result->field_10 = reinterpret_cast<uintptr_t>(reqData);

    ctx->cacao->processAsync(&param, bufVec, ctx->burstSnapshotCb, result);

    for (size_t i = 0; i < bufVec->size(); i++) {
        cacao::ImageBuf* imgBuf = (*bufVec)[i];
        imageprocessor::BufEntry* entry =
                BypassCameraBurstBufferManager_findByNativeHandle(ctx, imgBuf->getNative());
        if (entry) {
            entry->state = 2;  // in_use
            entry->tag = static_cast<int32_t>(requestId);
        }
    }

    BypassCameraBurstBufferManager_dump(ctx);
    return 0;
}
