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

namespace imageprocessor {

// so_32 @ 0x188d8 (BypassCameraPhoto_requestSnapshot) 反編譯確認：每次拍照請求會
// new 一個小 wrapper {ctx, VectorImpl*(imageBufs), requestId}，把它的指標存進
// 這次請求專屬、新配置的 ProcessCtrlResult::field_10（userData），而非像其他
// callback 一樣直接存 ctx。SnapshotCallback::onHandleProgress/onHandleResult
// 用這個 wrapper 取得 ctx 與 requestId（原版 piVar1=getUserData(); ctx=*piVar1;
// requestId=piVar1[2]）。這裡簡化成只存 ctx+requestId（不需要額外持有
// imageBufs vector 指標，我們的 processAsync 呼叫完就不再需要它）。
struct SnapshotResultUserData {
    BypassCameraContext* ctx;
    uint32_t requestId;
    // [原版沒有，20260731 裝置實測新增] cald 對同一個 requestId 有時會送兩次
    // handleProgress（logcat 實測間隔約 0.7 秒，onHandleResult 都還沒送達）。
    // 原版 onHandleProgress 反編譯確認沒有任何去重邏輯，Java 端
    // ShutterDoneHandlerCallbackImpl 也證實完全沒有防護（BypassCameraController
    // .peekLastSavingPhotoRequest() 在第二次到達、佇列已空時直接回傳 null，
    // Java 端沒檢查就直接用，NullPointerException 崩潰）。這代表原版的協定
    // 假設是「同一個 request 最多只會收到一次 ShutterDone」，cald 對我們的
    // 請求送出這個多餘的第二次是個尚未查出根因的協定落差。這裡用「每個
    // request 自己的 wrapper」記一個旗標，只抑制同一個 request 的重複
    // progress，不影響其他並行請求——跟舊版掛在 ctx 上的全域 shutterDoneSent
    // 不同，範圍精準對應到崩潰現場，不是回退到原本那種粗糙的做法。
    bool shutterDoneSent = false;
};

// 從 SnapshotCallback 專用的 wrapper userData 取出 ctx/requestId。
// 注意：這跟共用的 getCtxFromResult() 不同——SnapshotCallback 的
// ProcessCtrlResult::field_10 存的是上面這個 wrapper 的指標，不是 ctx 直接的
// 指標，不可以互相混用。
static bool getSnapshotCtxAndId(const cacao::ProcessResultBase* result,
                                 BypassCameraContext** outCtx, uint32_t* outReqId) {
    if (!result) return false;
    auto* udata = static_cast<SnapshotResultUserData*>(result->getUserData());
    if (!udata) return false;
    *outCtx = udata->ctx;
    *outReqId = udata->requestId;
    return true;
}

} // namespace imageprocessor

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
// 來自 so_32 @ 0x00018d91（完整反編譯）
// 呼叫 callbackFromNative(0x14, requestId, 1, isSuccess, false, false, 0xff, 0xff, 0xff)
//
// 完整反編譯確認原版對這個函式本身完全沒有任何去重邏輯——cald 每次觸發
// handleProgress 都無條件呼叫 callbackFromNative。原版靠的是
// requestSnapshot 幫每次請求配置專屬的 ProcessCtrlResult + wrapper（見
// SnapshotResultUserData），不是單一共用旗標。progress 參數傳的其實是
// requestId（wrapper[2]），不是 result->getProgress()；isSuccess 除了
// errCode==0 還要求 result->field_1c!=0 才算真的成功。
//
// [原版沒有，20260731 裝置實測新增] 完整實測（logcat + Java 端 smali 交叉
// 比對）確認：cald 有時會對同一個 requestId 送兩次 handleProgress（間隔約
// 0.7 秒，都在 onHandleResult 送達之前），若兩次都無條件轉發，Java 端
// BypassCameraControllerCallbackImpl 第二次會呼叫
// BypassCameraController.peekLastSavingPhotoRequest()——這時候第一次的
// 存檔流程已經把佇列清空，回傳 null，Java 端沒有 null 檢查，直接
// NullPointerException 崩潰（已用真機 logcat 抓到完整 stack trace 確認）。
// 這證實原版的協定假設是「同一個 request 最多只會收到一次 ShutterDone」，
// cald 對我們的請求多送這一次是尚未查出根因的協定落差；在查出根因之前，
// 用 SnapshotResultUserData::shutterDoneSent（每個 request 自己的旗標，
// 不是掛在 ctx 上的全域旗標）只抑制同一個 request 的重複 progress，不會
// 誤擋其他並行請求的合法進度。
// ─────────────────────────────────────────────────────
void SnapshotCallback::onHandleProgress(const cacao::ProcessResultBase* result) {
    if (!result) return;
    auto* udata = static_cast<imageprocessor::SnapshotResultUserData*>(result->getUserData());
    if (!udata) return;
    BypassCameraContext* ctx = udata->ctx;
    uint32_t reqId = udata->requestId;

    uint32_t errCode = result->getResult();
    bool isSuccess = false;
    if (errCode == 0) {
        const auto* r = static_cast<const cacao::ProcessCtrlResult*>(result);
        isSuccess = (r->field_1c != 0);
    }

    pthread_mutex_lock(&ctx->photoLock);
    if (ctx->photoInitialized && !udata->shutterDoneSent) {
        udata->shutterDoneSent = true;
        // so_32 @ 0x18d90 raw 組譯碼逐指令確認：傳給 Java 的最後三個 int 參數
        // 全部是 0xff（stm/strd 三次都存 0xff），不是 0/0xff/0xff。
        callPhotoCallback(ctx,
                          CB_SHUTTER_DONE,
                          (int)reqId, 1,
                          isSuccess, false, false,
                          0xff, 0xff, 0xff);
    }
    pthread_mutex_unlock(&ctx->photoLock);
}

// ─────────────────────────────────────────────────────
// SnapshotCallback::onHandleResult
// 來自 so_32 @ 0x00018e30（完整反編譯）
// 呼叫 callbackFromNative(0x15, requestId, 1, false, false, false, 0, 0xff, 0xff)
//
// 原版用 requestId 查 ctx+0x50（BufEntry*）跟 ctx+0x24（ProcessCtrlResult*，
// 即這次呼叫的 result 本身）兩個 SortedVector：找到 buffer entry 就標記
// state=3 並 queue 回去；找到 result 紀錄後就把 wrapper 與 result 本身
// delete 掉（"delete this" 手法——result 就是 param_1），最後才從
// ctx+0x24 移除並通知 Java。
//
// [原版「查不到」分支的反編譯與後果分析，20260731]
// 原版兩段查表都是：
//     idx = SortedVectorImpl::indexOf(vector, &requestId);
//     if (idx < 0) ptr = (vector位址 + 0x14);  // 查不到
//     else         ptr = (storage位址 + idx*8 + 4);  // 查到
// 反編譯原版 libutils.so 的 SortedVectorImpl::SortedVectorImpl(itemSize,flags)
// 建構子（so_32 libutils.so @ 0x1e630）確認這個物件只有 5 個 4-byte 欄位
// （vtable/mStorage/mCount/flags/itemSize），物件大小剛好是 0x14 bytes——
// 也就是說「vector位址 + 0x14」剛好等於「這個 heap 物件本身結尾之後的下一個
// 位元組」，完全在物件宣告範圍之外。這不是原版設計出來的安全 fallback，
// 是真正的越界讀取（undefined behavior），讀到什麼完全看當下 heap
// allocator 的配置歷史，不是編譯進二進位裡的固定值。
// 往下追這個「查不到」分支實際會做的事：
//   - ctx+0x50（BufEntry*）查不到：把這個越界讀到的值當成 BufEntry*，對
//     `entry->state`（+0xc）寫入 3。若那塊記憶體剛好是 0（新配置、還沒
//     寫過的分頁很常見），等同 `*(int*)0xc = 3`，會 SIGSEGV；若是非 0
//     殘留值，則是對一個隨機位址寫入 3——落在未映射記憶體一樣 SIGSEGV，
//     極少數情況落在別的合法記憶體則是無聲毀損。
//   - ctx+0x24（ProcessCtrlResult*）查不到：把越界讀到的值當成物件指標，
//     直接呼叫它的虛擬函式（getUserData()）甚至 delete 它——隨機位址幾乎
//     必定不是合法 vtable，這條路徑大機率比上面那條更容易直接崩潰。
// 也就是說原版對這個極端情境（cald 對同一 requestId 重複送達
// onHandleResult，已用 native backtrace 證實過確實會發生）大機率本身就是
// 會 SIGSEGV 的，只是觸發頻率低到 Sony 可能沒發現/沒修——不是我們少做了
// 什麼安全機制,而是原版這條路徑本身就是個沒被抓到的 bug。真的 android::
// SortedVector 在我們自己的編譯結果裡，這塊記憶體會是完全不同、無法預測
// 的內容，照抄同一個 +0x14 offset 不會重現原版行為，只會是另一種性質相同
// 但結果不可預測的越界存取。因此這裡改成明確的 indexOf()<0 判斷，查不到
// 就記 log 並跳過對應動作——把原版一個機率性自爆的隱藏 bug，換成一個
// 確定不會自爆的正常分支。
// ─────────────────────────────────────────────────────
void SnapshotCallback::onHandleResult(const cacao::ProcessResultBase* result) {
    BypassCameraContext* ctx = nullptr;
    uint32_t reqId = 0;
    if (!imageprocessor::getSnapshotCtxAndId(result, &ctx, &reqId)) return;

    pthread_mutex_lock(&ctx->photoLock);
    if (!ctx->photoInitialized) {
        pthread_mutex_unlock(&ctx->photoLock);
        return;
    }

    // ctx+0x50 (secondResultsById)：requestId -> BufEntry*
    if (ctx->secondResultsById) {
        android::key_value_pair_t<int, void*> key((int)reqId);
        ssize_t idx = ctx->secondResultsById->indexOf(key);
        if (idx >= 0) {
            auto* entry = static_cast<BufEntry*>((*ctx->secondResultsById)[idx].value);
            entry->state = 3;  // done
            BypassCameraBurstBufferManager_queueBuffer(ctx, entry);
            ALOGD("SnapshotCallback::onHandleResult: queued entry=%p reqId=%u", entry, reqId);
            ctx->secondResultsById->removeItemsAt(idx);
        } else {
            // 查不到：原版這裡會用越界讀取湊出一個假 BufEntry* 繼續寫 state=3
            // （見上方函式註解的完整分析），大機率本身就是 SIGSEGV；我們明確跳過。
            ALOGD("SnapshotCallback::onHandleResult: reqId=%u not found in secondResultsById", reqId);
        }
    }

    // ctx+0x24 (burstResultsById)：requestId -> ProcessCtrlResult*（= 這次呼叫的 result 本身）
    if (ctx->burstResultsById) {
        android::key_value_pair_t<int, cacao::ProcessCtrlResult*> key((int)reqId);
        ssize_t idx = ctx->burstResultsById->indexOf(key);
        if (idx >= 0) {
            imageprocessor::SnapshotResultUserData* udata =
                static_cast<imageprocessor::SnapshotResultUserData*>(result->getUserData());
            ctx->burstResultsById->removeItemsAt(idx);
            delete udata;
            // "delete this" 手法：result 就是這次請求配置的 ProcessCtrlResult 本身
            delete const_cast<cacao::ProcessCtrlResult*>(
                    static_cast<const cacao::ProcessCtrlResult*>(result));
        } else {
            // 查不到：原版這裡會用越界讀取湊出一個假物件指標，直接呼叫它的
            // 虛擬函式甚至 delete 它（見上方函式註解的完整分析），比上面
            // secondResultsById 那條更容易直接崩潰；我們明確跳過。
            ALOGD("SnapshotCallback::onHandleResult: reqId=%u not found in burstResultsById", reqId);
        }
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
    // 由 BypassCameraPhoto_requestSnapshot 寫入、SnapshotCallback::
    // onHandleResult 查表+delete 後移除。
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

    // 來自 so_32 @ 0x184cc：疊代 ctx->burstResultsById 刪除任何殘留 request
    // 的 ProcessCtrlResult 與其 userData wrapper（BypassCameraPhoto_
    // requestSnapshot 配置的 SnapshotResultUserData），再銷毀整個 vector。
    if (ctx->burstResultsById) {
        for (size_t i = 0; i < ctx->burstResultsById->size(); i++) {
            cacao::ProcessCtrlResult* result = (*ctx->burstResultsById)[i].value;
            if (result) {
                delete static_cast<imageprocessor::SnapshotResultUserData*>(result->getUserData());
                delete result;
            }
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
                                         jint inW, jint inH,
                                         jint outW, jint outH, jint captureNum) {
    if (!ctx || !ctx->cacao) return -1;

    // 原始 so_32 @ 0x186dc 反編譯確認：stop() 之後、建構 ProcessCtrlMode 之前，
    // 原版會先把 inW/inH/outW/outH/captureNum 快取進 ctx（ctx+0x40/0x44/0x48/
    // 0x4c/0xC4）。BypassCameraBurstBufferManager_createBuffers 建立 buffer
    // 時的寬高就是從 ctx+0x48/0x4c 讀出來的，不是從 dequeue 到的 buffer 自己
    // 的 width/height 讀，這裡補上這個快取。
    ctx->cachedPhotoInWidth = (uint32_t)inW;
    ctx->cachedPhotoInHeight = (uint32_t)inH;
    ctx->cachedPhotoOutWidth = (uint32_t)outW;
    ctx->cachedPhotoOutHeight = (uint32_t)outH;
    ctx->cachedPhotoCaptureNum = (uint32_t)captureNum;

    // 原始 so_32 @ 0x186dc: 先呼叫 stop() 重置 gateway 狀態
    int ret = ctx->cacao->stop();
    if (ret != 0) {
        ALOGE("%s: cacao stop() ret=%d", __func__, ret);
        if (ret == -0x6e) {  // PAL_ERR_ALREADY
            return (int)0xfffffffe;  // -2
        }
    }

    // [根因已確認，20260801] 逐指令反組譯 so_32 @ 0x186dc 確認：這個函式
    // 實際建構、傳給 cacao->start() 的 ProcessCtrlMode 物件是「先建一個暫存
    // 物件、再手動複製欄位到第二個真正送出去的物件」的兩段式流程；追蹤到
    // 底發現最終送出物件的 field_08（mode-type）是寫死的常數 0（組合語言
    // 直接 `movs r5,#0`、`str r5,[sp,#0x48]`，不是從任何呼叫端參數或 ctx
    // 欄位讀出來的）。這與 Java 端 smali 交叉比對的結果一致：
    // BypassCamera$PhotoMode enum 只有唯一值 NORMAL（ordinal=0），原版乾脆
    // 把這個位置寫死，不透過參數傳遞。因此這裡也直接寫死 0，不接受/使用
    // mode 參數（呼叫端 JNI wrapper 也已同步移除轉發，見 BypassCamera.cpp
    // 的 nativeChangeToPhotoMode）。完整反組譯過程記錄於
    // .tmp/investigation/snapshot_double_progress_findings.md。
    cacao::ProcessCtrlMode ctrlMode;
    ctrlMode.field_08 = 0;
    ctrlMode.field_0c = (int32_t)ctx->cameraMode;  // camera index (facing: 0=back, 1=front)
    ctrlMode.field_10 = (uint32_t)inW;
    ctrlMode.field_14 = (uint32_t)inH;
    ctrlMode.field_18 = (uint32_t)outW;
    ctrlMode.field_1c = (uint32_t)outH;
    ctrlMode.field_20 = (uint32_t)captureNum;
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

    // so_32 @ 0x188d8 反編譯確認：每次請求都 new 一個專屬的 ProcessCtrlResult
    // （不是共用的），userData 指向 {ctx, requestId} 的 wrapper，並以
    // requestId 為 key 分別存進 ctx+0x24（result 本身）與 ctx+0x50
    // （dequeue 到的 buffer entry），供 SnapshotCallback::onHandleResult
    // 完成時查表+delete。
    const uint32_t reqId = ctx->requestCounter;
    auto* udata = new imageprocessor::SnapshotResultUserData{ctx, reqId};
    cacao::ProcessCtrlResult* result = new cacao::ProcessCtrlResult();
    result->field_10 = reinterpret_cast<uintptr_t>(udata);

    // 設定 entry tracking（原始 .so: entry->tag = ctx->field_C0，不是
    // requestCounter——這個 tag 只用於診斷 log，不影響邏輯，反編譯
    // so_32 @ 0x188d8 確認）
    entry->tag = (int32_t)ctx->field_C0;

    if (ctx->burstResultsById) {
        ctx->burstResultsById->add(
                android::key_value_pair_t<int, cacao::ProcessCtrlResult*>((int)reqId, result));
    }
    if (ctx->secondResultsById) {
        ctx->secondResultsById->add(
                android::key_value_pair_t<int, void*>((int)reqId, entry));
    }

    // 建立 ImageBuf vector 傳給 processAsync (原始 .so: VectorImpl(4, 7))
    android::Vector<cacao::ImageBuf*> imageBufs;
    imageBufs.push_back(entry->imageBuf);

    ALOGD("requestSnapshot: dequeued entry=%p imgBuf=%p tag=%d reqId=%u",
          entry, entry->imageBuf, entry->tag, reqId);

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
