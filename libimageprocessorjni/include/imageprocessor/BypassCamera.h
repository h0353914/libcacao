//
// BypassCamera.h — imageprocessor::BypassCameraContext 及相關回呼類別宣告
// 依照 tools_Libcacao/refs/so_32 / so_64 Ghidra 反編譯結果重建
//
// ARM32 關鍵 byte offsets（已由反編譯驗證）:
//   ctx+0x00: android::Cacao*          (cacao 處理物件，NULL=未初始化)
//   ctx+0x04: uint32_t                 (init 0xFFFFFFFF)
//   ctx+0x08: uint32_t                 (cameraMode/index)
//   ctx+0x0c: jobject                  (global ref to Java BypassCamera)  − photo
//   ctx+0x10: jmethodID                (callbackFromNative method)        − photo
//   ctx+0x14: SnapshotReadyCallback*
//   ctx+0x18: ProcessCtrlResult*       (snapshot ready result, tag=0)
//   ctx+0x1c: SnapshotCallback*
//   ctx+0x20: uint32_t                 (request counter)
//   ctx+0x24: BurstReadyCallback*      (0x18-byte, thread-based)
//   ctx+0x28: BurstShotPrepareCallback*
//   ctx+0x2c: ProcessCtrlResult*       (burst prepare result, tag=1)
//   ctx+0x30: SnapshotFreeCallback*
//   ctx+0x34: ProcessCtrlResult*       (snapshot free result)
//   ctx+0x38: BurstShotFinishCallback*
//   ctx+0x3c: ProcessCtrlResult*       (burst finish result)
//   ctx+0x40: BurstCallback*           (0x18-byte, thread-based)
//   ctx+0x50: VideoPrepareSuperSlowCallback* (0x18-byte)
//   ctx+0x54: pthread_mutex_t          (photo lock)
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
//   ctx+0x8c: thread-based callback    (0x14-byte)
//   ctx+0x90-0x9b: video thread state
//   ctx+0x9c: bool                     (video initialized)
//   ctx+0x9d-0xb7: pad
//   ctx+0xB8: pthread_mutex_t          (video/burst lock)
//   ctx+0xBC: uint8_t                  (=0)
//   ctx+0xBD-0xbf: pad
//   ctx+0xC0: uint32_t                 (=0)
//   ctx+0xC4: uint32_t                 (=4)
//   ctx+0xC8: uint32_t                 (=0)
//   ctx+0xCC: uint32_t                 (=0)
//   ctx+0xD0: uint32_t                 (=0)
//   ctx+0xD4: BypassCameraBufferContext
//
#pragma once

#include <stdint.h>
#include <pthread.h>
#include <jni.h>
#include <deque>
#include <cacao/ProcessCtrlGatewayBase.h>
#include <gui/Surface.h>

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
// BypassCameraBufferContext（偏移 0xD4 start）
// 欄位佈局依據 Ghidra 反編譯 createBuffers/dequeueBuffer/queueBuffer 驗證
// ─────────────────────────────────────────────────────
struct BypassCameraBufferContext {
    android::Vector<BufEntry*> buffers;  // buffer pool
    android::sp<android::Surface> surface;  // sp<Surface>（匹配原始 .so ctx+0xdc）
    pthread_mutex_t mutex;      // surface/buffer mutex
    uint8_t         ready;      // flag: buffers created and ready
    uint8_t         _pad[3];
    uint32_t        dequeueCount; // 目前被 dequeue 的 buffer 數量
    uint32_t        totalBufCount; // 總 buffer 數量 (from Java mode param)

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
    IResultListener*    burstReadyCb;   // +0x24/0x40 (0x18 bytes, thread-based)
    IResultListener*    burstPrepareCb; // +0x28/0x48
    void*               burstPrepareResult; // +0x2c/0x50: ProcessCtrlResult*
    IResultListener*    snapshotFreeCb; // +0x30/0x58
    void*               snapshotFreeResult; // +0x34/0x60: ProcessCtrlResult*
    IResultListener*    burstFinishCb;  // +0x38/0x68
    void*               burstFinishResult;  // +0x3c/0x70: ProcessCtrlResult*
    IResultListener*    burstCb;        // +0x40/0x78 (0x18 bytes)
    void*               _pad1[2];       // +0x44-0x4f
    IResultListener*    videoPrepareSuperSlowCb; // +0x50/0x90 (0x18 bytes)
    pthread_mutex_t     photoLock;      // +0x54/0x98
    bool                photoInitialized; // +0x58/0x9c (or nearby offset)
    bool                shutterDoneSent; // cald 會送兩次 progress，只允許第一次觸發 ShutterDone
    uint8_t             _pad2[2];

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

    // --- second mutex ---
    pthread_mutex_t     videoLock;      // +0xB8/...
    uint8_t             field_BC;       // +0xBC/...
    uint8_t             _pad4[3];
    uint32_t            field_C0;       // =0
    uint32_t            field_C4;       // =4
    uint32_t            field_C8;       // =0
    uint32_t            field_CC;       // =0
    uint32_t            field_D0;       // =0
    uint32_t            cachedVideoOutWidth;
    uint32_t            cachedVideoOutHeight;
    uint32_t            cachedSuperSlowFrameNum;

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

} // namespace imageprocessor
