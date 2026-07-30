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
extern "C" imageprocessor::BufEntry* BypassCameraBurstBufferManager_findByNativeHandle(
        imageprocessor::BypassCameraContext* ctx, void* nativeHandle);

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
// 來自 so_32 @ 0x00018d91（完整反編譯，非早期簡化版本）
// 呼叫 callbackFromNative(0x14, requestId, 1, isSuccess, false, false, 0, 0xff, 0xff)
//
// [已嘗試改用 per-request id 移除 shutterDoneSent，裝置實測發現新的真實
// 崩潰，已還原本版本] 完整反編譯這個函式後確認原版確實沒有任何去重
// 邏輯，改用 getUserData() 取得的 per-request requestId 取代
// shutterDoneSent。但把 BypassCameraPhoto_requestSnapshot 也改為依此
// 反編譯結果、每次都建立新的 ProcessCtrlResult 並在 onHandleResult 中
// delete 它之後，實測（20260727 session，photo_settings 測試）出現同一個
// requestId 的 onHandleResult 被連續呼叫兩次、第二次對已釋放的
// ProcessCtrlResult 呼叫 getUserData() 是 use-after-free，並造成 Java 端
// ShutterDoneHandlerCallbackImpl 崩潰（與先前 shutterDoneSent 想避免的
// 崩潰同一個 NPE，但成因不同）。
//
// [動態驗證結果] 用 native backtrace 追蹤確認：onHandleProgress/
// onHandleResult 兩者都是透過真正的跨進程 Binder transact()
// （BnCacaoProcessCallback::onTransact <- BBinder::transact <-
// IPCThreadState::executeCommand <- joinThreadPool，thread pool
// worker thread）送達，不是同一個 thread 內的重複呼叫。但重複送達的
// 次數/時機並不固定：某次拍照觀察到 onHandleProgress 被送達 3 次、
// onHandleResult 只送達 1 次（3 次都在數十毫秒內、跟著同一組
// cald CAP[<--]frame/result 通知），但另一次拍照（20260727 稍早）
// 卻觀察到 onHandleResult 本身被送達 2 次。也就是說 cacaoserver 端對
// 「這次請求已經完整送達」這件事本身沒有可靠的去重保證，是
// non-deterministic 的。這代表任何在 onHandleResult/onHandleProgress
// 內部釋放 per-request 物件的設計都必須假設「同一個 result 指標可能在
// 任意之後的時間點被再呼叫一次」，不能只靠緊接著的下一次呼叫來判斷是否
// 為重複；目前這個簡化版本靠「共用、不在 callback 內刪除」的
// ProcessCtrlResult（見 createResultWithCtx）天生對此免疫，故先維持。
// ─────────────────────────────────────────────────────
void SnapshotCallback::onHandleProgress(const cacao::ProcessResultBase* result) {
    BypassCameraContext* ctx = getCtxFromResult(result);
    if (!ctx) return;

    int32_t  progress = result->getProgress();
    uint32_t errCode  = result->getResult();
    bool isSuccess    = (errCode == 0);

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
// 來自 so_32 @ 0x00018e30
// 呼叫 callbackFromNative(0x15, requestId, 1, false, false, false, 0, 0xff, 0xff)
// [見 onHandleProgress 上方說明：per-request id 版本已還原，這裡沿用
// ctx->requestCounter + 線性搜尋 in-use buffer 的簡化版本]
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
    BypassCameraBufferContext* bc = &ctx->bufCtx;
    BufEntry* entry = nullptr;
    if (bc->byBufferPtr) {
        for (size_t i = 0; i < bc->byBufferPtr->size(); i++) {
            BufEntry* e = (*bc->byBufferPtr)[i].value;
            if (e->state == 2) {
                entry = e;
                break;
            }
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
        if (bc->byBufferPtr) {
            for (size_t i = 0; i < bc->byBufferPtr->size(); i++) {
                BufEntry* e = (*bc->byBufferPtr)[i].value;
                if (e->state == 2) {
                    e->state = 3;
                    BypassCameraBurstBufferManager_queueBuffer(ctx, e);
                    ALOGD("SnapshotFreeCallback::onHandleResult: queued entry=%p tag=%d",
                          e, e->tag);
                    break;
                }
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
// 用於 ctx->burstSnapshotCb（BypassCameraBurst_requestSnapshot 實際使用的
// listener，見該函式反編譯結果）；userData 是 BurstRequestData wrapper，
// 不是裸 ctx 指標（跟 photo 的 SnapshotCallback 一樣，不能用
// getCtxFromResult）。刻意不在這裡 delete result/wrapper/bufVec——原因見
// BurstRequestData 的說明。
// ─────────────────────────────────────────────────────
void BurstCallback::onHandleProgress(const cacao::ProcessResultBase* result) {
    if (!result) return;
    auto* reqData = static_cast<BurstRequestData*>(result->getUserData());
    if (!reqData) return;
    BypassCameraContext* ctx = reqData->ctx;
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
    if (!result) return;
    auto* reqData = static_cast<BurstRequestData*>(result->getUserData());
    if (!reqData) return;
    BypassCameraContext* ctx = reqData->ctx;
    if (!ctx) return;

    // 把這次請求 dispatch 出去的每個 buffer 都標記完成並歸還給 Surface
    // （連拍一次可能有多個 buffer，逐一用 native handle 查回 BufEntry）。
    if (reqData->bufVec) {
        for (size_t i = 0; i < reqData->bufVec->size(); i++) {
            cacao::ImageBuf* imgBuf = (*reqData->bufVec)[i];
            if (!imgBuf) continue;
            BufEntry* entry = BypassCameraBurstBufferManager_findByNativeHandle(ctx, imgBuf->getNative());
            if (entry && entry->state == 2) {
                entry->state = 3;  // done
                BypassCameraBurstBufferManager_queueBuffer(ctx, entry);
            }
        }
    }

    callPhotoCallback(ctx,
                      CB_SNAPSHOT_DONE,
                      (int)reqData->requestId, 1,
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

    // ctx+0x24/0x50：反編譯確認原版分別以 requestId 為 key 追蹤
    // ProcessCtrlResult(+0x24)/BufEntry(+0x50)（見 BypassCamera.h 說明），
    // 但改用這個機制取代 shutterDoneSent 時裝置實測出現真實崩潰，已還原
    // 成簡化版本（見 SnapshotCallback::onHandleProgress 的詳細說明），這裡
    // 目前只建立空 vector、不會被寫入。
    if (!ctx->burstResultsById) {
        ctx->burstResultsById =
            new android::SortedVector<android::key_value_pair_t<int, cacao::ProcessCtrlResult*>>();
    }
    if (!ctx->secondResultsById) {
        ctx->secondResultsById =
            new android::SortedVector<android::key_value_pair_t<int, void*>>();
    }

    ctx->photoInitialized = true;
    ALOGD("BypassCameraPhoto_initialize: OK");
    return 0;
}

// ─────────────────────────────────────────────────────
// BypassCameraPhoto_finalize
// 來自 so_32 @ 0x000184cc
// 釋放 photo/burst 相關 callback、ProcessCtrlResult 與 JNI global ref，
// 供下次 BypassCameraPhoto_initialize 重新配置（ctx 為常駐 singleton，
// 這裡才是實際釋放的地方，不能依賴 ~BypassCameraContext()）
// ─────────────────────────────────────────────────────
extern "C" void BypassCameraPhoto_finalize(JNIEnv* env, jobject /*thiz*/, imageprocessor::BypassCameraContext* ctx) {
    if (!ctx) return;
    pthread_mutex_lock(&ctx->photoLock);
    ctx->photoInitialized = false;

    if (ctx->photoJObj) {
        if (env) env->DeleteGlobalRef(ctx->photoJObj);
        ctx->photoJObj = nullptr;
    }

    delete ctx->snapshotReadyCb;   ctx->snapshotReadyCb   = nullptr;
    delete ctx->snapshotCb;        ctx->snapshotCb        = nullptr;
    delete ctx->burstPrepareCb;    ctx->burstPrepareCb    = nullptr;
    delete ctx->snapshotFreeCb;    ctx->snapshotFreeCb    = nullptr;
    delete ctx->burstFinishCb;     ctx->burstFinishCb     = nullptr;

    delete static_cast<cacao::ProcessCtrlResult*>(ctx->snapshotReadyResult); ctx->snapshotReadyResult = nullptr;
    delete static_cast<cacao::ProcessCtrlResult*>(ctx->burstPrepareResult);  ctx->burstPrepareResult  = nullptr;
    delete static_cast<cacao::ProcessCtrlResult*>(ctx->snapshotFreeResult);  ctx->snapshotFreeResult  = nullptr;
    delete static_cast<cacao::ProcessCtrlResult*>(ctx->burstFinishResult);   ctx->burstFinishResult   = nullptr;

    // 來自 so_32 @ 0x184cc：疊代 ctx->burstResultsById/secondResultsById 刪除
    // 任何殘留 entry 的 value 後銷毀整個 vector（目前兩者都沒有程式碼會寫入
    // entry，見 BypassCamera.h 說明，這裡的疊代純屬防禦性/等效還原原版邏輯）
    if (ctx->burstResultsById) {
        for (size_t i = 0; i < ctx->burstResultsById->size(); i++) {
            delete (*ctx->burstResultsById)[i].value;
        }
        delete ctx->burstResultsById;
        ctx->burstResultsById = nullptr;
    }
    if (ctx->secondResultsById) {
        // 存的是 BufEntry*，生命週期屬於 buffer pool，這裡不 delete value。
        delete ctx->secondResultsById;
        ctx->secondResultsById = nullptr;
    }

    BypassCameraBurstBufferManager_deleteBuffers(ctx, nullptr);
    BypassCameraBurstBufferManager_finalizeSurface(ctx);

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

    // 重設 shutterDoneSent flag，允許新的一次 ShutterDone（見
    // SnapshotCallback::onHandleProgress 的說明：per-request id 版本已
    // 因裝置實測發現的真實崩潰而還原，暫時保留這個 workaround）
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

    // 從 Surface dequeue 一個 buffer (原始 .so @ 0x189ce)
    // so_32 兩個分支都用 level=ERROR 印一筆診斷 log（非真的錯誤，是原版風格），
    // 字串從 rodata 逐字讀出比對。
    imageprocessor::BufEntry* entry = nullptr;
    int dqRet = BypassCameraBurstBufferManager_dequeueBuffer(ctx, &entry);
    if (dqRet != 0 || !entry) {
        __android_log_print(ANDROID_LOG_ERROR, NULL,
            "%s: dequeueBuffer failed.", "BypassCameraPhoto_requestSnapshot");
        pthread_mutex_unlock(&ctx->photoLock);
        return -1;
    }
    __android_log_print(ANDROID_LOG_ERROR, NULL,
        "%s: dequeueBuffer ok.", "BypassCameraPhoto_requestSnapshot");

    // [已嘗試依反編譯結果改用 ctx+0x24/0x50 SortedVector 追蹤 per-request
    // ProcessCtrlResult/BufEntry，裝置實測發現真實崩潰（見
    // SnapshotCallback::onHandleProgress/onHandleResult 上方詳細說明），
    // 已還原成單一共用 ProcessCtrlResult 的簡化版本。]
    cacao::ProcessCtrlResult* result = createResultWithCtx(ctx);

    // 設定 entry tracking（原始 .so: entry->tag = ctx->field_C0，不是
    // requestCounter——這個 tag 只用於診斷 log，不影響邏輯，反編譯
    // so_32 @ 0x188d8 確認）
    entry->tag = (int32_t)ctx->field_C0;

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
