// ProcessCtrlGatewayBase.h — cacao::ProcessCtrlGatewayBase 類別宣告
// 依照 libcacao_process_ctrl_gateway.so 反編譯結果重建
// 物件佈局與 vtable 偏移均從 Ghidra 反編譯驗證

#pragma once
#include <cacao/cacao_pal.h>
#include <utils/RefBase.h>
#include <utils/StrongPointer.h>
#include <utils/Vector.h>
#include <stdint.h>

// HIDL 類型前向宣告（完整定義在 ImageBuf.cpp 的 #include 中）
namespace android { namespace hardware { namespace graphics { namespace mapper { namespace V2_0 {
    struct IMapper;
} } } } }
namespace android { namespace hardware { namespace graphics { namespace allocator { namespace V2_0 {
    struct IAllocator;
} } } } }

namespace cacao {

/* ── 前向宣告 ─────────────────────────────────────────────── */
class ProcessModeBase;
class ProcessParamBase;
class ProcessConfigBase;
class ProcessResultBase;
class ICBProcessResultListener;
class ICBProcessEventListener;
class ProcessEventBase;
class ImageBuf;

/* ISerialize::SerializedData — 序列化資料結構（來自 libcacao_pal.so）
 * sizeof = 0x194 = 404 bytes
 *   BnCacao::onTransact 以 by-value 形式將 SD 傳入虛擬呼叫：
 *     r2,r3 = SD[0..7], callee_sp[0..395] = SD[8..403]
 *   若宣告為 alignas(8) (→sizeof=0x198)，callee_sp 佔 400 bytes，
 *   導致後續引數偏移 +4，全部錯位。
 * makeSerializedData 設定 +0x184 = totalSize, +0x188 = bufPtr */
class ISerialize {
public:
    struct SerializedData {
        uint8_t  _pad[0x184];
        uint32_t size;         // +0x184: 緩衝區總大小
        void*    data;         // +0x188: 指向共享記憶體的指標
        uint32_t offset;       // +0x18C: 目前讀寫偏移量
        uint8_t  _pad2[4];    // +0x190: padding to 0x194
    };
};

/* ─────────────────────────────────────────────────────────── */
/* ICacaoGateway — 次級介面（secondary vtable，this+4）
 *
 * 由 _ZThn4_ 非虛擬 thunk 確認的方法集合。
 * ProcessCtrlGatewayBase : public cacao::ObjectBase, public ICacaoGateway
 * → cacao::ObjectBase 的 vtable ptr 在 this+0（主 vtable）
 * → ICacaoGateway 的 vtable ptr 在 this+4（次級 vtable）
 */
class ICacaoGateway {
public:
    virtual ~ICacaoGateway() = default;
    virtual int  subscribeEvents(ICBProcessEventListener* listener, void* cookie) = 0;
    virtual int  start(const ProcessModeBase* mode) = 0;
    virtual int  stop() = 0;
    virtual int  setConfig(const ProcessConfigBase* cfg) = 0;
    virtual int  getConfig(ProcessConfigBase* cfg) = 0;
    virtual int  process(const ProcessParamBase* param,
                         ICBProcessResultListener* listener,
                         ProcessResultBase* result) = 0;
    virtual void processAsync(const ProcessParamBase* param,
                              android::Vector<ImageBuf*>* imageBufs,
                              ICBProcessResultListener* listener,
                              ProcessResultBase* result) = 0;
    virtual int  cancel(void* cookie) = 0;
    virtual void serviceDied() = 0;
};

/* ─────────────────────────────────────────────────────────── */
/* ProcessCtrlGatewayBase
 *
 * 物件佈局（ARM32，從 GatewayBase_ctor 反編譯取得）:
 *  +0x00  vtable ptr (primary — cacao::ObjectBase base)
 *  +0x04  vtable ptr (secondary — ICacaoGateway base)
 *  +0x08  PAL_Thread_t  mThread
 *  +0x0c  PAL_QueId_t   mQueId
 *  +0x10  short         mPriority  (init=0x78)
 *  +0x12  uint16_t      padding
 *  +0x14  cacao_pal::Mutex   mMutex    (size 0x88)
 *  +0x9c  void*         mListener
 *  +0xa0  int           mListenerId
 *  +0xa4  RequestList   mRequests   (size 0x128)
 *  +0x1cc vtable ptr for ConfigList embedded object
 *  +0x1d0 cacao_pal::Mutex mConfigMutex (size 0x88)
 *  +0x258 vtable ptr for 2nd embedded list
 *  +0x25c int           mEventListenerCount (or similar)
 *  +0x260 ..
 *  +0x264 ..
 *  +0x268 cacao_pal::Mutex mStateMutex (size 0x88)
 *  +0x2f0 int           mInitFlag (0=uninit,1=inited,2=started,3=stopping)
 */
class ProcessCtrlGatewayBase : public cacao::ObjectBase, public ICacaoGateway {
public:
    /* ── 巢狀 RequestList ─────────────────────────────── */
    class Request;

    /* RequestList 繼承 PtrLockListDel<Request>：
     * 匹配 REF 佈局：vtable[0]+Mutex[0x88]+PtrList[0x10]=0x9C + Condition[0x8C]=0x128 total */
    class RequestList : public cacao_pal::PtrLockListDel<Request> {
    public:
        RequestList();
        virtual ~RequestList();
        /* removeNode 由 PtrLockListDel 提供（刪除 Request data） */
        int  start(Request* req);
        int  finish(Request* req);
        int  cancel(Request* req);
        int  remove(Request* req);
        void cancelAll();
        static bool compareRequestState(Request* item, void* state);
    private:
        cacao_pal::Condition mCond;  /* +0x9C, size 0x8C，linked to mMutex at +0x04 */
    };

    /* ── 請求類別（巢狀）─────────────────────────────── */
    /* Ghidra: Request 繼承 cacao::ObjectBase */
    class Request : public cacao::ObjectBase {
    public:
        /* 請求狀態機 */
        enum State : uint32_t {
            STATE_IDLE     = 0,
            STATE_CANCELED = 1,
            STATE_ACTIVE   = 2,
            STATE_STOPPING = 3,
            STATE_DONE     = 4,
        };

        Request(const ProcessParamBase* param,
                android::Vector<ImageBuf*>* imageBufs,
                ICBProcessResultListener* listener,
                ProcessResultBase* result,
                uint8_t flags,
                const PAL_Msg_t* replyMsg);
        virtual ~Request();

        void cancel();
        int  start();
        int  finish();
        void notifyResult(PAL_Err_t err);
        void notifyProgress(int progress);
        bool checkState(unsigned int state);   // non-const to match original
        bool checkCancel();                    // non-const to match original
        virtual void onRequestCancel();        // 匯出 — 不可 inline

    protected:
        /* 欄位偏移（從 Request 物件基址）:
         *  +0x00  vtable
         *  +0x04  ProcessParamBase* mParam  (clone)
         *  +0x08  Vector<ImageBuf*>* mImageBufs
         *  +0x0c  ICBProcessResultListener* mListener
         *  +0x10  ProcessResultBase* mResult
         *  +0x14  uint8_t mFlags
         *  +0x18  PAL_Msg_t* mReplyMsg (const)
         *  +0x1c  cacao_pal::Mutex mMutex
         *  +0xa4  uint32_t mState
         */
        char _pad[0xa8];

        friend class ProcessCtrlGatewayBase;
    };

    /* ─────────────────────────────────────────────────── */
    explicit ProcessCtrlGatewayBase();
    virtual ~ProcessCtrlGatewayBase();

    /* ── 公開 API（virtual，對應 REF vtable slot03-05）─── */
    virtual const char* getClassName() const = 0;
    virtual int   init(short priority);
    virtual int   deinit();
    void  cancelAllRequest();

    /* ── ICacaoGateway 介面實作 ───────────────────────── */
    virtual int  subscribeEvents(ICBProcessEventListener* listener, void* cookie) override;
    virtual int  start(const ProcessModeBase* mode) override;
    virtual int  stop() override;
    virtual int  setConfig(const ProcessConfigBase* cfg) override;
    virtual int  getConfig(ProcessConfigBase* cfg) override;
    virtual int  process(const ProcessParamBase* param,
                         ICBProcessResultListener* listener,
                         ProcessResultBase* result) override;
    virtual void processAsync(const ProcessParamBase* param,
                              android::Vector<ImageBuf*>* imageBufs,
                              ICBProcessResultListener* listener,
                              ProcessResultBase* result) override;
    virtual int  cancel(void* cookie) override;
    virtual void serviceDied() override;

    /* ── 可覆寫鉤子（在 .cpp 實作以匯出 symbols）────── */
    virtual int   onInit();
    virtual void  onInitDelayed();
    virtual int   onDeinit();
    virtual int   onStart(const ProcessModeBase* mode);
    virtual void  onStartDelayed();
    virtual int   onStop(bool isCancelAll);
    virtual int   onStopRequest();
    virtual void  onProcess(Request* req) = 0;
    virtual void  onHandleMsg(PAL_Msg_t* msg);
    virtual int   onSetConfig(const ProcessConfigBase* cfg);
    virtual int   onGetConfig(ProcessConfigBase* cfg);
    virtual int   onValidateConfig(const ProcessConfigBase* cfg);
    virtual Request* onCreateRequest(const ProcessParamBase* param,
                                     android::Vector<ImageBuf*>* imageBufs,
                                     ICBProcessResultListener* listener,
                                     ProcessResultBase* result,
                                     uint8_t flags,
                                     const PAL_Msg_t* replyMsg);

protected:
    /* ── 通知輔助 ─────────────────────────────────── */
    void  notifyProgress(Request* req, int progress);
    int   notifyResult(Request* req, PAL_Err_t err);
    void  notifyEvent(ProcessEventBase* ev);
    int   notifyMsg(PAL_Msg_t* msg);

    /* ── 訊息分發 ────────────────────────────────── */
    static int   threadDispatchMsg(const PAL_Msg_t* msg, void* ctx);
    void  onDispatchMsg(PAL_Msg_t* msg);
    void  onHandleInitMsg(PAL_Msg_t* msg);
    void  onHandleDeinitMsg(PAL_Msg_t* msg);
    void  onHandleStartMsg(PAL_Msg_t* msg);
    void  onHandleStopMsg(PAL_Msg_t* msg);
    void  onHandleStopRequestMsg(PAL_Msg_t* msg);
    void  onHandleProcessMsg(PAL_Msg_t* msg);
    void  onAbortMsg();
    void  cleanCanceledRequests();
    void  applyConfigs();

    /* ConfigList 繼承 PtrLockListDel<ProcessConfigBase*>：
     * 匹配 REF 佈局：vtable[0]+Mutex[0x88]+PtrList[0x10] = 0x9C (total) */
    class ConfigList : public cacao_pal::PtrLockListDel<ProcessConfigBase*> {
    public:
        /* Ghidra: ConfigList 有一個非 inline 構造函數（68 bytes），包含 vtable 設定 +
         * Mutex::Mutex() + PtrList 初始化。必須非 inline 才能生成 C1/C2Ev 匯出符號。 */
        ConfigList();
        virtual ~ConfigList();
        static bool compareConfigId(ProcessConfigBase* item, void* key);
    };

private:
    PAL_Thread_t             mThread;         // +0x08
    PAL_QueId_t              mQueId;          // +0x0c
    short                    mPriority;       // +0x10
    uint16_t                 _pad0;           // +0x12

    cacao_pal::Mutex         mMutex;          // +0x14, size 0x88

    void*                    mListener;       // +0x9c
    int                      mListenerId;     // +0xa0

    RequestList              mRequests;       // +0xa4, size 0x128

    /* ConfigList (+0x1cc ~ +0x268, size 0x9c): vtable+Mutex+embedded list */
    ConfigList               mConfigs;        // +0x1cc

    cacao_pal::Mutex         mStateMutex;     // +0x268, size 0x88
    int                      mInitFlag;       // +0x2f0 (0=uninit,1=init,2=start,3=stop)
};

/* ── 其他基底類別前向宣告（完整定義供外部使用）──────────── */

/* ProcessModeBase — 繼承 ObjectBase，為 primary base chain
 * Primary vtable layout (Clang): D1, D0, dump(→getClassName), getType
 * REF onStart 用 mode->getType() 讀取 mode type，
 * 而 mode 指標是 reinterpret_cast<ProcessModeBase*>(objectbase_ptr)，
 * 指向物件開頭（primary base，不是 secondary base）。
 */
class ProcessModeBase : public ObjectBase {
public:
    virtual ~ProcessModeBase() = default;
    virtual uint32_t getType() const = 0;
};

/* ISerializable — 序列化介面（secondary base for ProcessCtrlMode）
 * REF ProcessCtrlMode secondary vtable (0x13838):
 *   [0] D1 thunk, [1] D0 thunk,
 *   [2] getSerializedSize() const,
 *   [3] serialize() const,
 *   [4] deserialize()
 */
class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual uint32_t getSerializedSize() const = 0;
    virtual int serialize(ISerialize::SerializedData* data) const = 0;
    virtual int deserialize(ISerialize::SerializedData* data) = 0;
};

/* ProcessCtrlMode — mode 物件，由 Factory::createMode 建構
 *
 * 繼承 ProcessModeBase (primary, contains ObjectBase) + ISerializable (secondary)
 * Size = 0x2c bytes
 *
 * 物件佈局（ARM32）:
 *   +0x00  primary vtable ptr (ProcessModeBase chain)
 *   +0x04  secondary vtable ptr (ISerializable)
 *   +0x08  uint32_t  field_08 (type, init 0)
 *   +0x0c  int32_t   field_0c (init -1)
 *   +0x10  uint32_t  field_10..field_1c (init 0)
 *   +0x20  uint32_t  field_20..field_24 (init 0)
 *   +0x28  uint32_t  field_28 (init 0)
 */
class ProcessCtrlMode : public ProcessModeBase, public ISerializable {
public:
    ProcessCtrlMode();
    ~ProcessCtrlMode() override;

    /* ObjectBase::dump override → getClassName */
    void dump(unsigned int flags) override;

    /* ProcessModeBase */
    uint32_t getType() const override;

    /* ISerializable overrides (也出現在 primary vtable) */
    uint32_t getSerializedSize() const override;
    int serialize(ISerialize::SerializedData* data) const override;
    int deserialize(ISerialize::SerializedData* data) override;

    /* 欄位 */
    uint32_t  field_08;       // +0x08, init 0 (mode type)
    int32_t   field_0c;       // +0x0c, init -1
    uint32_t  field_10;       // +0x10
    uint32_t  field_14;       // +0x14
    uint32_t  field_18;       // +0x18
    uint32_t  field_1c;       // +0x1c
    uint32_t  field_20;       // +0x20
    uint32_t  field_24;       // +0x24
    uint32_t  field_28;       // +0x28
};

/* ProcessParamBase — 繼承 ObjectBase
 * REF vtable layout (primary):
 *   ObjectBase: [0] D1, [1] D0, [2] dump, [3] getClassName
 *   ProcessParamBase adds: [4] clone, [5] copy
 * Request ctor 使用 vtbl[0x10/4] = slot 4 = clone
 * Request dtor 使用 vtbl[0x04/4] = slot 1 = D0 (delete) */
class ProcessParamBase : public ObjectBase {
public:
    virtual ~ProcessParamBase() = default;
    virtual ProcessParamBase* clone() const = 0;
    virtual void copy(const ProcessParamBase* src) = 0;
};

/* ProcessCtrlParam: 繼承 ProcessParamBase + ISerializable
 * 大小 0x160 bytes
 * Primary vtable (ProcessParamBase chain, 9 entries):
 *   [0] D1, [1] D0, [2] dump, [3] getClassName,
 *   [4] clone, [5] copy, [6] getSerializedSize, [7] serialize, [8] deserialize
 * Secondary vtable (ISerializable):
 *   [0] D1_thunk, [1] D0_thunk, [2] getSerializedSize, [3] serialize_thunk, [4] deserialize_thunk
 * 欄位初始值從原始 .so c6b8 constructor 反組譯取得
 */
class ProcessCtrlParam : public ProcessParamBase, public ISerializable {
public:
    ProcessCtrlParam();
    ~ProcessCtrlParam() override = default;
    const char* getClassName() const override { return "ProcessCtrlParam"; }
    ProcessParamBase* clone() const override;
    void copy(const ProcessParamBase* src) override;

    uint32_t getSerializedSize() const override;
    int serialize(ISerialize::SerializedData* data) const override;
    int deserialize(ISerialize::SerializedData* data) override;

    // 資料欄位：+0x08 到 +0x15F（大部分由 ctor 歸零）
    uint8_t _data[0x160 - 0x08];
};

/* ProcessCtrlVideoRecParam — 繼承 ProcessCtrlParam 的 video 錄影參數
 * 大小 0x170 bytes
 * 額外欄位: +0x160 sp<IGraphicBufferProducer>, +0x164 field, +0x168 field
 * clone/copy 從 REF 反編譯 */
class ProcessCtrlVideoRecParam : public ProcessParamBase, public ISerializable {
public:
    ProcessCtrlVideoRecParam();
    ~ProcessCtrlVideoRecParam() override;
    const char* getClassName() const override { return "ProcessCtrlVideoRecParam"; }
    ProcessParamBase* clone() const override;
    void copy(const ProcessParamBase* src) override;

    uint32_t getSerializedSize() const override;
    int serialize(ISerialize::SerializedData* data) const override;
    int deserialize(ISerialize::SerializedData* data) override;

    // 和 ProcessCtrlParam 相同的欄位 (+0x08 到 +0x15F)
    uint8_t _data[0x160 - 0x08];
    // 額外欄位 (+0x160 到 +0x16F)
    uint32_t _extra[4];  // +0x160: sp<IGraphicBufferProducer>(4 bytes), +0x164, +0x168, padding
};

/* ProcessConfigBase — 繼承 ObjectBase
 * REF vtable layout (primary):
 *   ObjectBase: [0] D1, [1] D0, [2] dump, [3] getClassName
 *   ProcessConfigBase adds: [4] getConfigId, [5] clone, [6] copy
 * setConfig 使用 vtbl[0x10/4] = getConfigId, vtbl[0x14/4] = clone, vtbl[0x18/4] = copy */
class ProcessConfigBase : public ObjectBase {
public:
    virtual ~ProcessConfigBase() = default;
    virtual uint32_t getConfigId() const = 0;
    virtual ProcessConfigBase* clone() const = 0;
    virtual void copy(const ProcessConfigBase* src) = 0;
};

/* ProcessCtrlDynamicParameterConfig — config 物件，由 Factory::createConfig 建構
 * 繼承 ProcessConfigBase (primary, contains ObjectBase) + ISerializable (secondary)
 * Size = 0x0C bytes
 *
 * 物件佈局（ARM32）:
 *   +0x00  primary vtable ptr (ProcessConfigBase chain)
 *   +0x04  secondary vtable ptr (ISerializable)
 *   +0x08  uint32_t configValue (init 0)
 *
 * Primary vtable: D1, D0, dump, getClassName, getConfigId, clone, copy,
 *                 getSerializedSize, serialize, deserialize (10 entries)
 * Secondary vtable: D1_thunk, D0_thunk, getSerializedSize, serialize_thunk,
 *                   deserialize_thunk (5 entries) */
class ProcessCtrlDynamicParameterConfig : public ProcessConfigBase, public ISerializable {
public:
    ProcessCtrlDynamicParameterConfig() : configValue(0) {}
    ~ProcessCtrlDynamicParameterConfig() override = default;

    const char* getClassName() const override { return "ProcessCtrlDynamicParameterConfig"; }
    uint32_t getConfigId() const override { return 0x65; }

    ProcessConfigBase* clone() const override {
        void* mem = ObjectBase::operator new(0xc);
        if (!mem) return nullptr;
        auto* c = ::new(mem) ProcessCtrlDynamicParameterConfig();
        c->configValue = configValue;
        return c;
    }

    void copy(const ProcessConfigBase* src) override {
        const auto* s = static_cast<const ProcessCtrlDynamicParameterConfig*>(src);
        configValue = s->configValue;
    }

    uint32_t getSerializedSize() const override { return 4; }

    int serialize(ISerialize::SerializedData* data) const override;
    int deserialize(ISerialize::SerializedData* data) override;

    uint32_t configValue;    // +0x08
};

/* ProcessResultBase：繼承 ObjectBase，提供 result/progress/userData 虛函數
 * REF ProcessCtrlResult primary vtable analysis (0x237bc):
 *   [0-1] dtors, [2] dump, [3] getClassName,
 *   [4] getResult, [5] setResult, [6] getProgress, [7] setProgress,
 *   [8] getUserData, [9-11] ISerializable methods
 * slots 4-8 屬於 ProcessResultBase 的虛函數
 */
class ProcessResultBase : public ObjectBase {
public:
    virtual ~ProcessResultBase() = default;
    virtual uint32_t getResult() const = 0;
    virtual void setResult(PAL_Err_t err) = 0;
    virtual int32_t getProgress() const = 0;
    virtual void setProgress(int progress) = 0;
    virtual void* getUserData() const = 0;
};

/* ProcessCtrlResult: 繼承 ProcessResultBase + ISerializable
 * 大小 0x34 bytes
 * Primary vtable (ObjectBase→ProcessResultBase chain) at +0
 * Secondary vtable (ISerializable) at +4
 * 欄位初始值從原始 .so c188 constructor 反組譯取得
 */
class ProcessCtrlResult : public ProcessResultBase, public ISerializable {
public:
    ProcessCtrlResult();
    virtual ~ProcessCtrlResult();

    /* ProcessResultBase overrides */
    uint32_t getResult() const override { return field_08; }
    void setResult(PAL_Err_t err) override { field_08 = (uint32_t)err; }
    int32_t getProgress() const override { return field_0c; }
    void setProgress(int progress) override { field_0c = progress; }
    void* getUserData() const override { return reinterpret_cast<void*>(field_10); }

    /* ISerializable overrides */
    uint32_t getSerializedSize() const override { return 0x28; }
    int serialize(ISerialize::SerializedData* data) const override;
    int deserialize(ISerialize::SerializedData* data) override;

    uint32_t field_08;      // +0x08, init 0  — result
    int32_t  field_0c;      // +0x0C, init -1  — progress
    uintptr_t field_10;     // +0x10, init 0  — userData (pointer-sized for 64-bit compat)
    uint32_t field_14;      // +0x14 (32-bit) / +0x18 (64-bit), init 0
    uint32_t field_18;      // +0x18, init 0
    uint32_t field_1c;      // +0x1C, init 0
    uint32_t field_20;      // +0x20, init 0xFFFFFFFF
    uint32_t field_24;      // +0x24, init 0
    uint32_t field_28;      // +0x28, init 0
    uint32_t field_2c;      // +0x2C, init 0xFFFF0000
    uint8_t  field_30;      // +0x30, init 0xFF
    uint8_t  _pad[3];       // padding to 0x34
};

class ICBProcessResultListener {
public:
    virtual ~ICBProcessResultListener() = default;
    virtual void onHandleProgress(const ProcessResultBase* result) = 0; // vtable[0x08/4]
    virtual void onHandleResult(const ProcessResultBase* result) = 0;   // vtable[0x0c/4]
    virtual ICBProcessResultListener* addRef() = 0;    // vtable[0x10/4]
    virtual void onResult(PAL_Err_t err) = 0;          // vtable[0x14/4]
    virtual void onProgress(int progress) = 0;         // vtable[0x18/4]
};

class ICBProcessEventListener {
public:
    virtual ~ICBProcessEventListener() = default;
    virtual void onHandleEvent(const ProcessEventBase* ev) = 0;
};

class ProcessEventBase {
public:
    virtual ~ProcessEventBase() = default;
    virtual void setId(int id) = 0;                    // vtable[0x08]
    virtual int  getId() const = 0;                    // vtable[0x0c]
    virtual int  getType() const = 0;                  // vtable[0x10]
    virtual uint32_t getSize() const = 0;              // vtable[0x14]
    virtual void setSize(uint32_t sz) = 0;             // vtable[0x18]
    virtual uint32_t getSerializedSize() const = 0;    // vtable[0x1c]
    virtual int serialize(ISerialize::SerializedData* sd) const = 0; // vtable[0x20]
};

/* ImageBuf — gralloc-backed 影像緩衝區
 * VTable layout (全 16 slots，從 libcacao_client.so 反編譯確認):
 *   [0] D1 dtor   [1] D0 dtor  [2] dump (inherited ObjectBase)
 *   [3] getClassName  [4] getNative  [5] getBufSize
 *   [6] getBufAddress [7] getImageSize [8] getDirtyFlag [9] setDirtyFlag
 *   [10] clearCache  [11] flushCache  [12] flushAndClearCache
 *   [13] getDataSize [14] setDataSize [15] getDataSpace
 * 物件佈局 (sizeof=0x28):
 *   +0x00 vtable*  +0x04 mWidth  +0x08 mHeight  +0x0c mHandle*
 *   +0x10 mMappedAddr*  +0x14 mBufSize  +0x18 mDataSpace  +0x1c mDataSize
 *   +0x20 mOwnFlag(u8)  +0x21 mDirty(bool)  +0x22 pad[2]  +0x24 mSharedBuf*
 */
class ImageBuf : public ObjectBase {
public:
    /* 建構子 */
    ImageBuf();
    ImageBuf(void* data, ImageSize size, DataSpace ds, ImageFormat fmt);
    ImageBuf(ImageSize size, unsigned int stride, ImageFormat fmt, DataSpace ds);
    ImageBuf(const ImageBuf& other);

    /* 虛擬解構子與 getClassName / getNative */
    virtual ~ImageBuf();
    virtual const char*  getClassName()    const override;        /* slot  3 */
    virtual void*        getNative()       const;                  /* slot  4 */

    /* 緩衝區屬性查詢 */
    virtual uint32_t     getBufSize()      const;                  /* slot  5 */
    virtual void*        getBufAddress();                           /* slot  6 */
    virtual ImageSize    getImageSize()    const;                   /* slot  7 */
    virtual bool         getDirtyFlag()    const;                   /* slot  8 */
    virtual void         setDirtyFlag(bool dirty);                  /* slot  9 */

    /* 快取管理（重要：clearCache/flushCache 由 getBufAddress/destructor 呼叫） */
    virtual void         clearCache();                              /* slot 10 */
    virtual void         flushCache();                              /* slot 11 */
    virtual void         flushAndClearCache();                      /* slot 12 */

    /* 資料大小與格式 */
    virtual uint32_t     getDataSize()     const;                   /* slot 13 */
    virtual void         setDataSize(uint32_t size);                /* slot 14 */
    virtual DataSpace    getDataSpace()    const;                   /* slot 15 */

    /* 非虛擬方法 */
    bool isValid() const;

    /* gralloc singleton 初始化（private helper） */
    int  getMapper();
    int  getAllocator();

    /* 靜態 gralloc singleton */
    static android::sp<android::hardware::graphics::mapper::V2_0::IMapper>     mMapper;
    static android::sp<android::hardware::graphics::allocator::V2_0::IAllocator> mAllocator;

private:
    uint32_t      mWidth;       /* +0x04 */
    uint32_t      mHeight;      /* +0x08 */
    void*         mHandle;      /* +0x0c  native_handle_t* (or gralloc allocated) */
    void*         mMappedAddr;  /* +0x10  CPU 端虛擬位址 (clearCache 後設定) */
    uint32_t      mBufSize;     /* +0x14  緩衝區大小 */
    DataSpace     mDataSpace;   /* +0x18 */
    uint32_t      mDataSize;    /* +0x1c  有效資料大小 */
    uint8_t       mOwnFlag;     /* +0x20  0x01=已分配(需 freeBuffer), 0x00=僅 import */
    bool          mDirty;       /* +0x21 */
    uint8_t       _pad[2];      /* +0x22 */
    SharedBuffer* mSharedBuf;   /* +0x24 */
};

#if defined(__LP64__)
static_assert(sizeof(ImageBuf) == 0x38,
    "ImageBuf size must be 0x38 bytes to match original 64-bit ABI");
#else
static_assert(sizeof(ImageBuf) == 0x28,
    "ImageBuf size must be 0x28 bytes to match original 32-bit ABI");
#endif

/* ─────────────────────────────────────────────────────────── */
/* ProcessFactory — 工廠類別，方法直接轉呼叫 android::Cacao 靜態/實例方法
 *
 * 符號（來自 nm -D libcacao_client_real.so）：
 *   cacao::ProcessFactory::createEvent(unsigned int, SerializedData*)
 *   cacao::ProcessFactory::createProcess()
 *   cacao::ProcessFactory::getJpegBufferSize(ImageSize)
 *   cacao::ProcessCtrlCapsFactory::getCaps(CameraIndex const&, Caps*)
 *
 * 實作分析（binary）：
 *   createEvent  → return 0 (Thumb: movs r0,#0; bx lr)
 *   createProcess → tail-call PLT[android::Cacao::create()]
 *   getJpegBufferSize → tail-call PLT[android::Cacao::getJpegBufferSize()]
 *   getCaps → tail-call PLT[android::Cacao::getCaps()]
 *
 * 注意：android::Cacao::getJpegBufferSize / getCaps 皆透過靜態 mService
 * 存取，不依賴 this 指針，因此可透過 reinterpret_cast 轉呼叫。
 */

namespace ProcessCtrlCaps {
    struct CameraIndex;  /* 定義於 ICacaoService.h：int index; */
}

class Caps;  /* 前向宣告，定義於 server_real/include/CacaoService.h */

} // namespace cacao

namespace android { class Cacao; }  /* 前向宣告，定義於 CacaoClient.h */

namespace cacao {

class ProcessFactory {
public:
    /* createEvent — 永遠回傳 nullptr（原始 .so 確認：movs r0,#0; bx lr）*/
    static ProcessEventBase* createEvent(uint32_t type,
                                         ISerialize::SerializedData* sd);

    /* createProcess — 轉呼叫 android::Cacao::create() */
    android::Cacao* createProcess();

    /* getJpegBufferSize — 轉呼叫 android::Cacao::getJpegBufferSize(size) */
    static int getJpegBufferSize(ImageSize size);
};

class ProcessCtrlCapsFactory {
public:
    /* getCaps — 轉呼叫 android::Cacao::getCaps(camIdx, caps) */
    static int getCaps(const ProcessCtrlCaps::CameraIndex& camIdx, Caps* caps);
};

} // namespace cacao
