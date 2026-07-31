//
// BypassCamera.h — imageprocessor::BypassCameraContext 及相關回呼類別宣告
// 依照 tools_Libcacao/refs/so_32 / so_64 Ghidra 反編譯結果重建
//
// ARM32 關鍵 byte offsets（已由反編譯驗證；下方標註「已修正」的項目為
// 交叉比對 initialize/finalize/建構子/解構子後發現與舊版文件不符之處）:
//   ctx+0x00: android::Cacao*          (cacao 處理物件，NULL=未初始化)
//   ctx+0x04: uint32_t                 (init 0xFFFFFFFF)
//   ctx+0x08: uint32_t                 (cameraMode/index)
//   ctx+0x0c: jobject                  (global ref to Java BypassCamera)  − photo
//   ctx+0x10: jmethodID                (callbackFromNative method)        − photo
//   ctx+0x14: SnapshotReadyCallback*
//   ctx+0x18: ProcessCtrlResult*       (snapshot ready result, tag=0)
//   ctx+0x1c: SnapshotCallback*
//   ctx+0x20: uint32_t                 (request counter)
//   ctx+0x24: [已確認/已實作，且已修正先前錯誤結論] burstResultsById：
//             以 requestId 為 key、ProcessCtrlResult* 為 value 的
//             SortedVector。**修正**：先前文件宣稱對整個 binary 搜尋
//             `SortedVectorImpl::add` 找不到真實呼叫點、認定此 vector 永遠是
//             空的——這個結論是錯的（當時搜尋範圍/方法有誤）。完整反編譯
//             BypassCameraPhoto_requestSnapshot(so_32 @ 0x188d8) 後確認：
//             每次成功 dequeue 到 buffer 後，會以 requestId 為 key，把「這次
//             請求專屬、新配置的 ProcessCtrlResult」加進這個 vector；
//             SnapshotCallback::onHandleResult(so_32 @ 0x18e30) 完成時再用
//             同一個 requestId 查回、釋放（VectorImpl + userData wrapper +
//             ProcessCtrlResult 本身）並從 vector 移除。這正是原版能安全
//             處理 cald 重複送 progress 而不需要額外去重旗標的關鍵機制——
//             見 BypassCameraPhoto.cpp 內 SnapshotCallback 的完整重寫與說明。
//   ctx+0x28: BurstShotPrepareCallback*
//   ctx+0x2c: ProcessCtrlResult*       (burst prepare result, tag=1)
//   ctx+0x30: SnapshotFreeCallback*
//   ctx+0x34: ProcessCtrlResult*       (snapshot free result)
//   ctx+0x38: BurstShotFinishCallback*
//   ctx+0x3c: ProcessCtrlResult*       (burst finish result)
//   ctx+0x40: BurstCallback*           (0x18-byte, thread-based；finalize 路徑未見清理，
//             疑似原版本身即未釋放，暫不處理)
//   ctx+0x50: [已確認/已實作，且已修正先前錯誤結論] secondResultsById：
//             以 requestId 為 key、**BufEntry***（不是 ProcessCtrlResult*）
//             為 value 的 SortedVector，追蹤「這次請求 dequeue 出來、尚未
//             queue 回去的 buffer」。反編譯確認的建構方式與 +0x24 相同（同一
//             組 SortedVectorImpl(elemSize=8,7)），但存的值型別不同——先前
//             文件「與 +0x24 同構」的推測只對了一半。同樣由
//             BypassCameraPhoto_requestSnapshot 寫入、SnapshotCallback::
//             onHandleResult 取出使用後移除；並非「永遠是空的」。
//   ctx+0x54: pthread_mutex_t          (photo lock；反編譯確認整個結構體僅此一個
//             pthread_mutex_t，解構子只 destroy 這一個)
//   ctx+0x58: bool                     (photo initialized)
//   ctx+0x59-0x5b: pad
//   ctx+0x5c: jobject                  (global ref)                       − video
//   ctx+0x60: jmethodID                (callbackFromNativeVideo method)   − video
//   ctx+0x64: VideoFinishCallback*
//   ctx+0x68: ProcessCtrlResult*
//   ctx+0x6c: VideoPrepareCallback*
//   ctx+0x70: ProcessCtrlResult*
//   ctx+0x74: VideoStartSuperSlowRecCallback*
//   ctx+0x78: ProcessCtrlResult*
//   ctx+0x7c: VideoStartRecCallback* or similar
//   ctx+0x80: ProcessCtrlResult*
//   ctx+0x84: VideoPrepareSuperSlowCallback* (dup? or different one)
//   ctx+0x88: ProcessCtrlResult*
//   ctx+0x8c: [已修正] 原標示「thread-based callback (0x14-byte)」，經
//             BypassCameraVideo_initialize/_finalize 反編譯確認實際是
//             android::VectorImpl*（heap-allocated，元素大小 4）而非callback
//             指標。目前程式碼用 std::deque<VideoThreadCommand*> + 獨立
//             mutex/cond 實作功能相同的 video command 佇列，內部表示法
//             與原版不同但行為等價（此欄位純內部實作細節，不涉及外部 ABI，
//             不需位元組對位）
//   ctx+0x90-0x97: [已確認] 反編譯 BypassCameraContext::BypassCameraContext()
//             (so_32 @ 0x171dc) 確認建構子用 `__aeabi_memclr4(this+0x5c, 0x41)`
//             一次性把 ctx+0x5c..0x9c 這段（涵蓋所有 video 回呼欄位、
//             videoThreadCb、這裡的 0x90-0x97、以及 videoInitialized）整段
//             清零，並非另一個帶 vtable 的物件——先前文件推測的「vtable 常數
//             賦值模式」不成立。純粹是 video 執行緒相關的內部暫存位元組，不
//             涉及外部 ABI，不需位元組對位（沿用目前 videoThreadState[0x10]
//             的通用位元組陣列表示法即可）。
//   ctx+0x98: pthread_t                (video command thread handle)
//   ctx+0x9c: bool                     (video initialized)
//   ctx+0x9d-0xb7: pad
//   ctx+0xB8: [已確認] android::sp<android::IGraphicBufferProducer> videoSurfaceProducer。
//             反編譯 BypassCameraContext::BypassCameraContext()/~BypassCameraContext()
//             (so_32 @ 0x171dc / 0x1723c) 確認建構子呼叫
//             sp<IBinder>::operator=((sp<IBinder>*)(this+0xb8), null)，解構子呼叫
//             sp<IGraphicBufferProducer>::~sp((this+0xb8))——整個 BypassCameraContext
//             建構子/解構子只 init/destroy 一個 pthread_mutex_t（photoLock, +0x54），
//             這裡完全不是 mutex。原本 videoLock 欄位對它做 pthread_mutex_init/destroy
//             是原版沒有的邏輯，已移除；此欄位目前尚未找到實際賦值處（Phase B 待辦，
//             需搜尋 video prepare/changeToVideoMode 路徑找出賦值來源）。
//   ctx+0xBC: [已確認] bool burstInitialized。反編譯 BypassCameraBurst_initialize
//             (so_32 @ 0x1b1bc) 確認建構時設為 1，BypassCameraBurst_finalize
//             (so_32 @ 0x1b238) 設回 0——是單純的「burst 模組已初始化」旗標。
//   ctx+0xBD-0xbf: pad
//   ctx+0xC0: uint32_t                 (=0，BypassCameraBurst_requestSnapshot 中遞增，
//             已在現有 field_C0 正確使用)
//   ctx+0xC4: uint32_t                 (=4)
//   ctx+0xC8: [已確認] jobject burstJObj。反編譯 BypassCameraBurst_initialize 確認
//             是 env->NewGlobalRef(thiz)（burst 模組自己的一份 global ref，與
//             photoJObj/videoJObj 同構但獨立），BypassCameraBurst_finalize 透過
//             JNIEnv vtable+0x58（=DeleteGlobalRef）釋放。之前完全未實作建立，
//             現已補上。
//   ctx+0xCC: [已確認] jmethodID burstMethodId。反編譯確認是
//             env->GetMethodID(FindClass("com/sonymobile/imageprocessor/bypasscamera2/BypassCamera"),
//             "callbackFromNative", "(IIIZZZIII)V")——與 photoMethodId 是同一個
//             Java method，burst 模組另外快取一份。原文件誤標為 uint32_t(=0)。
//   ctx+0xD0: [已確認/部分] IResultListener* burstSnapshotCb。反編譯
//             BypassCameraBurst_initialize 確認是以 operator new(4) 配置的純
//             vtable 物件（sizeof=4，無資料成員），BypassCameraBurst_finalize
//             透過其 vtable slot 1（deleting destructor）釋放；
//             BypassCameraBurst_requestSnapshot(so_32 @ 0x1b290) 中被當作
//             listener 傳入 cacao 的 burst snapshot dispatch 呼叫。其 vtable
//             onHandleProgress/onHandleResult 反編譯結果與 BurstCallback
//             (ctx+0x40) 的實作位址相同（懷疑是編譯器 identical-code-folding
//             把兩者的 4-byte 純 vtable 實作折疊成同一份機器碼，非必然是同一
//             個 class）——沿用 BurstCallback 型別以重現行為，但兩個欄位是各自
//             獨立配置/釋放的物件，不可共用同一個實例。注意：ctx+0x40 標示的
//             「0x18-byte」大小疑似有誤，BurstCallback 實際看起來也是 4-byte
//             純 vtable stateless listener（所有狀態都從 onHandleProgress/
//             onHandleResult 的 param_1 取得，不使用 this 的資料成員）。
//             BypassCameraBurst_requestSnapshot 已依此重寫：每次呼叫都用
//             BypassCameraBurstBufferManager_createBufVector 建立 buffer
//             vector、重新配置 ProcessCtrlResult + BurstRequestData wrapper，
//             並用 4 參數的 processAsync(param, bufVec, listener, result)
//             dispatch（不是 3 參數的 process()），與反編譯出的真實呼叫形狀
//             一致（見 BypassCameraBurst.cpp 內的完整說明）。
//   ctx+0xD4: BypassCameraBufferContext（原版）。**注意**：我們的
//             BypassCameraContext C++ 宣告在 burstSnapshotCb 之後多插入了
//             三個內部快取欄位（cachedVideoOutWidth/Height/
//             cachedSuperSlowFrameNum，供 video dataspace 計算用，純內部
//             實作細節、不對應原版任何欄位），導致 bufCtx 在我們的編譯結果
//             中實際位於 ctx+0xE0 而非 0xD4。因為我們的程式碼一律用具名
//             成員（ctx->bufCtx.xxx）存取，不會有人對 ctx 做原始位元組
//             offset 運算，所以這個位移對邏輯正確性沒有影響，純粹是這份
//             文件在對照原版 byte offset 時要注意的落差。
//   ctx+0xE8: [已確認] = bufCtx(+0xD4) + 0x14 = dequeueCount。反編譯
//             BypassCameraBufferContext::BypassCameraBufferContext()
//             (so_32 @ 0x18014) 逐欄位確認佈局：+0x00/+0x04 buffers(Vector,
//             8 bytes)、+0x08 sp<Surface>(4 bytes)、+0x0c pthread_mutex_t
//             (4 bytes)、+0x10 ready(1 byte)+3 byte pad、+0x14 為第一個
//             uint32_t 欄位。BypassCameraBurst_finalize (so_32 @ 0x1b238)
//             反編譯確認：此值非 0 時印出
//             `"%s: wrong mDequeueCounter=%d"`（函式名稱代入 %s，數值代入
//             %d，皆從 rodata 逐字讀出）——純診斷用途，不影響記憶體清理，
//             已在 BypassCameraBurst_finalize 中實作對應的 log。
//
#pragma once

#include <stdint.h>
#include <pthread.h>
#include <jni.h>
#include <deque>
#include <cacao/ProcessCtrlGatewayBase.h>
#include <gui/Surface.h>
#include <gui/IGraphicBufferProducer.h>
#include <utils/SortedVector.h>
#include <utils/KeyedVector.h>

// 前向宣告
namespace android { class Cacao; }

namespace imageprocessor {

// ─────────────────────────────────────────────────────
// Java 回呼型別常數（與 Java BypassCamera.smali 中的 CALLBACK_TYPE_* 一致）
// ─────────────────────────────────────────────────────
enum CallbackType : int {
    // callbackFromNativeVideo(I)V 使用的型別
    CB_VIDEO_START_REC_DONE           = 0x0,
    CB_VIDEO_STOP_REC_DONE            = 0x1,
    CB_VIDEO_PREPARE_REC_DONE         = 0x2,
    CB_VIDEO_START_SUPER_SLOW_DONE    = 0x3,
    CB_VIDEO_PREPARE_SUPER_SLOW_DONE  = 0x4,

    // callbackFromNative(IIIZZZIII)V 使用的型別
    CB_SNAPSHOT_READY_DONE            = 0xa,
    CB_SHUTTER_DONE                   = 0x14,
    CB_SNAPSHOT_DONE                  = 0x15,
    CB_SNAPSHOT_FREE_DONE             = 0x1e,
    CB_BURST_PREPARE_DONE             = 0x28,
    CB_BURST_FINISH_DONE              = 0x32,
};

// ─────────────────────────────────────────────────────
// IResultListener — 繼承 cacao::ICBProcessResultListener
// 使 callback 類別可直接傳入 cacao->process(param, listener, result)
// ─────────────────────────────────────────────────────
class IResultListener : public cacao::ICBProcessResultListener {
public:
    ~IResultListener() override = default;
    void onHandleProgress(const cacao::ProcessResultBase* result) override {}
    void onHandleResult(const cacao::ProcessResultBase* result) override {}
    ICBProcessResultListener* addRef() override { return this; }
    void onResult(PAL_Err_t err) override {}
    void onProgress(int progress) override {}
};

struct BypassCameraContext;  // 完整定義在下方

// ─────────────────────────────────────────────────────
// BurstRequestData — burst snapshot 每次請求專屬的 userData wrapper
// 對應原版 BypassCameraBurst_requestSnapshot(so_32 @ 0x1b290) 反編譯出的
// puVar2 = {ctx, 0, bufVectorPtr, requestId}（4 個 word；第二個固定為 0
// 的 word 用途未確認，我們的重建省略、不影響行為）。
//
// [重要] 這個 wrapper 連同它指到的 ProcessCtrlResult、bufVec 都不能在
// BurstCallback::onHandleProgress/onHandleResult 內部 delete——反編譯與
// 動態驗證（見 BypassCameraPhoto.cpp 內 SnapshotCallback 的詳細說明）都
// 確認 cacaoserver 對同一個 request 的 callback 遞送次數是不固定的，
// 可能非同步地被呼叫不只一次；在回呼內刪除自己會造成 use-after-free。
// 因此這裡採用跟 photo 一樣「刻意不刪除、允許極小記憶體 leak」的安全作法。
// ─────────────────────────────────────────────────────
struct BurstRequestData {
    BypassCameraContext* ctx;
    android::Vector<cacao::ImageBuf*>* bufVec;
    uint32_t requestId;
};

// ─────────────────────────────────────────────────────
// BufEntry — 每個 buffer 的追蹤結構
// 原始 .so 中為 0x14-byte 結構，儲存在 SortedVector
// ─────────────────────────────────────────────────────
struct BufEntry {
    int               fence;    // +0x00: dequeue fence fd
    cacao::ImageBuf*  imageBuf; // +0x04: ImageBuf 物件
    void*             anwb;     // +0x08: ANativeWindowBuffer*
    uint32_t          state;    // +0x0C: 0=free, 1=dequeued, 2=in_use, 3=done
    int32_t           tag;      // +0x10: request counter (-1=unused)
};

// ─────────────────────────────────────────────────────
// BypassCameraBufferContext（偏移 ctx+0xD4 start）
// 完整反編譯 BypassCameraBufferContext::BypassCameraBufferContext()
// (so_32 @ 0x18014)、BurstBufferManager_createBuffers/dequeueBuffer/
// queueBuffer (so_32 @ 0x1a758/0x1ab64/0x1acf4) 後確認：原版不是單一
// android::Vector 線性搜尋，而是兩個各自獨立的 SortedVector，只在
// createBuffers 時各塞入一次（之後 dequeue/queue 只改 BufEntry::state，
// 不會再 add/remove）：
//   +0x00 (ctx+0xD4)：以 dequeueBuffer 拿到的 ANativeWindowBuffer* 為 key，
//                     dequeueBuffer 內用來把 native dequeue 回傳的指標
//                     對應回 BufEntry。
//   +0x04 (ctx+0xD8)：以 imageBuf->getNative()（vtable slot 4 = ImageBuf
//                     的 getNative()，offset 0x10）回傳的 native handle
//                     為 key，BypassCameraBurst_requestSnapshot 用來把
//                     createBufVector 產生的一批 ImageBuf* 逐一對應回
//                     BufEntry（連拍一次會有多個 buffer 同時在途，線性搜尋
//                     "state==2" 無法區分是哪一個，必須靠這個 keyed lookup）。
// 兩個 vector 的內容彼此相同（同一批 BufEntry*），只是 key 不同，各自服務
// 不同呼叫端；清理/列舉全部項目時任取一個即可。
// ─────────────────────────────────────────────────────
struct BypassCameraBufferContext {
    android::SortedVector<android::key_value_pair_t<void*, BufEntry*>>* byBufferPtr;    // +0x00 (ctx+0xD4)
    android::SortedVector<android::key_value_pair_t<void*, BufEntry*>>* byNativeHandle; // +0x04 (ctx+0xD8)
    android::sp<android::Surface> surface;  // +0x08 (ctx+0xDC)
    pthread_mutex_t mutex;      // +0x0c (ctx+0xE0)
    uint8_t         ready;      // +0x10 (ctx+0xE4): flag: buffers created and ready
    uint8_t         _pad[3];
    uint32_t        dequeueCount; // +0x14 (ctx+0xE8) 目前被 dequeue 的 buffer 數量
    uint32_t        totalBufCount; // +0x18 (ctx+0xEC) 總 buffer 數量 (from Java mode param)

    BypassCameraBufferContext();
    ~BypassCameraBufferContext();
};

// ─────────────────────────────────────────────────────
// BypassCameraContext — 主要 native context 結構
// 此結構在 nativeInitialize 時 heap-allocated，
// nativeFinalize 時釋放。
// ─────────────────────────────────────────────────────
struct VideoThreadCommand;

struct BypassCameraContext {
    // --- photo / common 欄位 ---
    android::Cacao*     cacao;          // +0x00 (ptr)
    uint32_t            initTag;        // +0x04/0x08: init=0xFFFFFFFF
    uint32_t            cameraMode;     // +0x08/0x0c
    jobject             photoJObj;      // +0x0c/0x10: NewGlobalRef(bypassCam)
    jmethodID           photoMethodId;  // +0x10/0x18: callbackFromNative
    IResultListener*    snapshotReadyCb;// +0x14/0x20
    void*               snapshotReadyResult; // +0x18/0x28: ProcessCtrlResult*
    IResultListener*    snapshotCb;     // +0x1c/0x30
    uint32_t            requestCounter; // +0x20/0x38
    android::SortedVector<android::key_value_pair_t<int, cacao::ProcessCtrlResult*>>* burstResultsById; // +0x24
    IResultListener*    burstPrepareCb; // +0x28/0x48
    void*               burstPrepareResult; // +0x2c/0x50: ProcessCtrlResult*
    IResultListener*    snapshotFreeCb; // +0x30/0x58
    void*               snapshotFreeResult; // +0x34/0x60: ProcessCtrlResult*
    IResultListener*    burstFinishCb;  // +0x38/0x68
    void*               burstFinishResult;  // +0x3c/0x70: ProcessCtrlResult*
    IResultListener*    burstCb;        // +0x40/0x78 (0x18 bytes)
    void*               _pad1[2];       // +0x44-0x4f
    // +0x50：反編譯 SnapshotCallback::onHandleResult (so_32 @ 0x18e30) 確認
    // 存的是 BufEntry*（不是 ProcessCtrlResult*，與 +0x24 用途不同，並非
    // 「同構」），以 requestId 為 key 追蹤「這次請求 dequeue 出來的 buffer」。
    // [目前未使用] 曾嘗試依此重寫 requestSnapshot/onHandleResult 的
    // per-request 追蹤機制，裝置實測發現真實崩潰已還原（見
    // BypassCameraPhoto.cpp 內 SnapshotCallback 的說明），型別維持
    // void* 以保留反編譯結論，但欄位目前沒有程式碼會寫入。
    android::SortedVector<android::key_value_pair_t<int, void*>>* secondResultsById; // +0x50
    pthread_mutex_t     photoLock;      // +0x54/0x98
    bool                photoInitialized; // +0x58/0x9c (or nearby offset)
    uint8_t             _pad2[3];

    // --- video 欄位 ---
    jobject             videoJObj;      // +0x5c/...
    jmethodID           videoMethodId;  // +0x60/...
    IResultListener*    videoFinishCb;  // +0x64/...
    void*               videoFinishResult;
    IResultListener*    videoPrepareCb;
    void*               videoPrepareResult;
    IResultListener*    videoStartSuperSlowCb;
    void*               videoStartSuperSlowResult;
    IResultListener*    videoStartRecCb; // ??? (thread-based)
    void*               videoStartRecResult;
    IResultListener*    videoSuperSlowCb;
    void*               videoSuperSlowResult;
    IResultListener*    videoThreadCb;   // +0x8c/... (0x14 bytes)
    uint8_t             videoThreadState[0x10]; // +0x90/...: thread-related
    bool                videoInitialized; // +0x9c/...
    uint8_t             _pad3[0x1b];

    std::deque<VideoThreadCommand*> videoCommandQueue;
    pthread_mutex_t     videoCommandLock;
    pthread_cond_t      videoCommandCond;
    pthread_t           videoCommandThread;
    bool                videoCommandThreadRunning;

    android::sp<android::IGraphicBufferProducer> videoSurfaceProducer; // +0xB8
    bool                burstInitialized; // +0xBC: burst 模組已初始化旗標
    uint8_t             _pad4[3];
    uint32_t            field_C0;       // =0
    // [已修正] 舊文件標「=4」是錯的：反編譯 BypassCameraPhoto_changeToPhotoMode
    // (so_32 @ 0x186dc) 確認這個位置其實是動態存進去的 flags 參數
    // （param_3[0x31] = param_8），見 cachedPhotoFlags。
    uint32_t            field_C4;
    jobject             burstJObj;      // +0xC8: NewGlobalRef(thiz)，burst 模組專用
    jmethodID           burstMethodId;  // +0xCC: callbackFromNative methodID（burst 模組專用快取）
    IResultListener*    burstSnapshotCb;// +0xD0: BypassCameraBurst_requestSnapshot 使用的 listener
    uint32_t            cachedVideoOutWidth;
    uint32_t            cachedVideoOutHeight;
    uint32_t            cachedSuperSlowFrameNum;

    // [新增，20260731] 反編譯 BypassCameraPhoto_changeToPhotoMode
    // (so_32 @ 0x186dc) 確認原版會把 inW/inH/outW/outH/flags 快取進 ctx
    // （ctx+0x40/0x44/0x48/0x4c/0xC4），且 BypassCameraBurstBufferManager_
    // createBuffers (so_32 @ 0x1a758) 建立 buffer 時的寬高就是從
    // ctx+0x48/0x4c（mode==0/photo 時）讀出來的，不是從 dequeue 到的
    // ANativeWindowBuffer 自己的 width/height 讀。我們原本完全沒做這個
    // 快取，buffer 建立時只能用 anwb 自己回報的尺寸，懷疑是這個尺寸來源
    // 不一致導致 cald 內部多跑一個階段（雙重 progress 的根因候選）。
    uint32_t            cachedPhotoInWidth;
    uint32_t            cachedPhotoInHeight;
    uint32_t            cachedPhotoOutWidth;
    uint32_t            cachedPhotoOutHeight;
    uint32_t            cachedPhotoFlags;

    // --- buffer context ---
    BypassCameraBufferContext bufCtx;   // +0xD4/...

    BypassCameraContext();
    ~BypassCameraContext();
};

// ─────────────────────────────────────────────────────
// 回呼類別宣告
//
// 每個回呼繼承 IResultListener，並在建構時持有 BypassCameraContext*
// （由 ProcessCtrlResult::getUserData() 在 onHandleResult 中取得）
// ─────────────────────────────────────────────────────

// 4-byte vtable-only callbacks（只有 vtable ptr，無 data member）
class SnapshotReadyCallback : public IResultListener {
public:
    SnapshotReadyCallback();
    ~SnapshotReadyCallback() override;
    void onHandleProgress(const cacao::ProcessResultBase* result) override;
    void onHandleResult(const cacao::ProcessResultBase* result) override;
};

class SnapshotCallback : public IResultListener {
public:
    SnapshotCallback();
    ~SnapshotCallback() override;
    void onHandleProgress(const cacao::ProcessResultBase* result) override;
    void onHandleResult(const cacao::ProcessResultBase* result) override;
};

class SnapshotFreeCallback : public IResultListener {
public:
    SnapshotFreeCallback();
    ~SnapshotFreeCallback() override;
    void onHandleProgress(const cacao::ProcessResultBase* result) override;
    void onHandleResult(const cacao::ProcessResultBase* result) override;
};

class BurstShotPrepareCallback : public IResultListener {
public:
    BurstShotPrepareCallback();
    ~BurstShotPrepareCallback() override;
    void onHandleProgress(const cacao::ProcessResultBase* result) override;
    void onHandleResult(const cacao::ProcessResultBase* result) override;
};

class BurstShotFinishCallback : public IResultListener {
public:
    BurstShotFinishCallback();
    ~BurstShotFinishCallback() override;
    void onHandleProgress(const cacao::ProcessResultBase* result) override;
    void onHandleResult(const cacao::ProcessResultBase* result) override;
};

// Video callbacks (4-byte)
class VideoPrepareCallback : public IResultListener {
public:
    VideoPrepareCallback();
    ~VideoPrepareCallback() override;
    void onHandleProgress(const cacao::ProcessResultBase* result) override;
    void onHandleResult(const cacao::ProcessResultBase* result) override;
};

class VideoFinishCallback : public IResultListener {
public:
    VideoFinishCallback();
    ~VideoFinishCallback() override;
    void onHandleProgress(const cacao::ProcessResultBase* result) override;
    void onHandleResult(const cacao::ProcessResultBase* result) override;
};

class VideoStartRecCallback : public IResultListener {
public:
    VideoStartRecCallback();
    ~VideoStartRecCallback() override;
    void onHandleProgress(const cacao::ProcessResultBase* result) override;
    void onHandleResult(const cacao::ProcessResultBase* result) override;
};

class VideoPrepareSuperSlowCallback : public IResultListener {
public:
    VideoPrepareSuperSlowCallback();
    ~VideoPrepareSuperSlowCallback() override;
    void onHandleProgress(const cacao::ProcessResultBase* result) override;
    void onHandleResult(const cacao::ProcessResultBase* result) override;
};

class VideoStartSuperSlowRecCallback : public IResultListener {
public:
    VideoStartSuperSlowRecCallback();
    ~VideoStartSuperSlowRecCallback() override;
    void onHandleProgress(const cacao::ProcessResultBase* result) override;
    void onHandleResult(const cacao::ProcessResultBase* result) override;
};

enum class VideoThreadCommandKind : int {
    PrepareVideo = 0,
    StartVideo = 1,
    StopVideo = 2,
    PrepareSuperSlow = 4,
    StartSuperSlow = 5,
};

struct VideoThreadCommand {
    VideoThreadCommandKind kind;
    jobject surface;
    jint p1;
    jint p2;
    jint p3;
};

class BurstCallback : public IResultListener {
public:
    BurstCallback();
    ~BurstCallback() override;
    void onHandleProgress(const cacao::ProcessResultBase* result) override;
    void onHandleResult(const cacao::ProcessResultBase* result) override;
};

// ─────────────────────────────────────────────────────
// 工具函式宣告
// ─────────────────────────────────────────────────────
// 從 result->getUserData() 取得 BypassCameraContext*
BypassCameraContext* getCtxFromResult(const cacao::ProcessResultBase* result);

// 呼叫 Java callbackFromNative（photo）
void callPhotoCallback(BypassCameraContext* ctx,
                       int type, int p1, int p2,
                       bool b1, bool b2, bool b3,
                       int i1, int i2, int i3);

// 呼叫 Java callbackFromNativeVideo（video）
void callVideoCallback(BypassCameraContext* ctx, int type);

bool isVideoThreadDirect();
void setVideoThreadDirect(bool direct);
void enqueueVideoCommand(JNIEnv* env,
                         BypassCameraContext* ctx,
                         VideoThreadCommandKind kind,
                         jobject surface,
                         jint p1, jint p2, jint p3);

// so_32 反編譯確認：prepareVideoRecording/startVideoRecording/stopVideoRecording/
// prepareSuperSlowRecording/startSuperSlowRecording 這 5 個公開函式在原版都只是
// 把命令推進這個佇列（BypassCameraVideo_pushThreadRequest）就立刻返回，實際的
// cacao 呼叫都是這個 worker thread 非同步執行的。
void* BypassCameraVideo_worker(void* arg);

} // namespace imageprocessor
