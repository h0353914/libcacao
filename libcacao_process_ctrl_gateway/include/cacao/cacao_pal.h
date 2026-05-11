// cacao_pal.h — PAL C API 存根宣告
// 依據 libcacao_process_ctrl_gateway.so 反編譯結果建立
// 實際符號在執行期由 libcacao_pal.so 提供

#pragma once
#include <stddef.h>
#include <stdint.h>
#include <pthread.h>
#include <new>

/* PAL_Err_t — 必須是具名 C++ 型別才能匹配原始 .so 的 name mangling（9PAL_Err_t）
 * 是否用 enum 小技: typedef int 在 C++ 會 mangle 成 'i'，但原始 .so 顯示 PAL_Err_t。 */
#ifdef __cplusplus
enum PAL_Err_t : int {
    PAL_ERR_OK            = 0,
    PAL_ERR_NO_INIT       = -0x65,   /* -101: 未初始化 */
    PAL_ERR_ALREADY       = -0x6d,   /* -109: already in state */
    PAL_ERR_NOMEM         = -0x6f,   /* -111: no memory/no service */
    PAL_ERR_INVAL         = -0x67,   /* -103: invalid param */
    PAL_ERR_NOT_FOUND     = -0x72,   /* -114 */
    PAL_ERR_NULL          = -0x72,
    PAL_ERR_NOT_SUPPORTED = -0x6a,   /* -106: not supported */
    PAL_ERR_CANCELED      = -0x6c,   /* -108: request canceled */
    PAL_ERR_INVALID_STATE = -0x6e,
};
#else
typedef int PAL_Err_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ── PAL 基本型別 ─────────────────────────────────────────── */
typedef void*   PAL_Thread_t;
typedef int     PAL_QueId_t;

/* PAL_Msg_t 佈局 (from decompile):
 *  +0x00  msg_id    (uint32)
 *  +0x04  reserved  (uint32)
 *  +0x08  reserved  (uint32)
 *  +0x0c  dest      (PAL_QueId_t)
 *  +0x10  reserved  (uint32)
 *  +0x14  size      (uint32, 含 header = 0x18 或 0x1c)
 *  +0x18  payload…
 */
typedef struct PAL_Msg_t {
    uint32_t    msg_id;
    uint32_t    reserved0;
    uint32_t    reserved1;
    PAL_QueId_t dest;
    uint32_t    reserved2;
    uint32_t    size;
    /* 變長 payload */
} PAL_Msg_t;

/* ── 訊息 ID ───────────────────────────────────────────────── */
#define PAL_MSG_INIT           0x01000000u
#define PAL_MSG_DEINIT         0x01000001u
#define PAL_MSG_START          0x01000002u
#define PAL_MSG_STOP_REQ       0x01000003u   /* sync stop */
#define PAL_MSG_STOP           0x01000004u   /* async stop */
#define PAL_MSG_PROCESS        0x01000005u
#define PAL_MSG_CLEANUP        0x01000006u
#define PAL_MSG_PROGRESS       0x01000007u   /* caldo→gateway */
#define PAL_MSG_RESULT         0x01000008u
#define PAL_MSG_SUPER_SLOW     0x01000009u

/* ── PAL API ───────────────────────────────────────────────── */
extern PAL_Err_t PAL_Create(void);
extern PAL_Err_t PAL_Delete(void);
extern void      PAL_MemSet(void* dst, int val, size_t len);
extern void*     PAL_MemAllocEx(int alignment, size_t size, const char* file, int line, int flags);
extern void      PAL_MemFreeEx(void* ptr, int flags);
extern int       PAL_StrNPrintf(char* buf, size_t n, const char* fmt, ...);
extern size_t    PAL_StrLen(const char* s);
extern void      PAL_LogPrint(const char* file, int line, int level, int tag,
                              const char* fmt, ...);
extern const char* PAL_ErrToString(int err);

typedef int (*PAL_ThreadCallback_t)(void* msg, void* ctx);
extern PAL_Err_t    PAL_ThreadCreateWithMsgQue(const char* name, int prio,
                                               PAL_ThreadCallback_t callback,
                                               void* ctx,
                                               void* /* PAL_Thread_t* */ out);
extern PAL_QueId_t  PAL_ThreadGetQueId(PAL_Thread_t thread);
extern PAL_Err_t    PAL_ThreadClose(PAL_Thread_t thread);

extern PAL_Err_t    PAL_MsgSendRecv(PAL_Msg_t* msg);
extern PAL_Err_t    PAL_MsgSend(PAL_Msg_t* msg);
extern PAL_Err_t    PAL_MsgReply(PAL_Msg_t* msg, int result);

extern uint64_t     PAL_TimeGetMonotonic(void);

/* ── PAL 回傳碼（保留宏定義以相容） ────────────────────── */
#ifdef __cplusplus
/* 在 C++ 中 enum 值已包含必要的常數 */
#else
#define PAL_ERR_OK            0
#define PAL_ERR_NO_INIT       (-0x65)
#define PAL_ERR_ALREADY       (-0x6d)
#define PAL_ERR_NOMEM         (-0x6f)
#define PAL_ERR_INVAL         (-0x67)
#define PAL_ERR_NOT_FOUND     (-0x72)
#define PAL_ERR_NULL          (-0x72)
#define PAL_ERR_NOT_SUPPORTED (-0x6a)
#define PAL_ERR_CANCELED      (-0x6c)
#define PAL_ERR_INVALID_STATE (-0x6e)
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

/* ── C++ PAL namespace ─────────────────────────────────────── */
#ifdef __cplusplus

namespace cacao {
/* ObjectBase: 從 libcacao_pal.so 匯入，在 cacao 命名空間
 * vtable layout: [0] D1, [1] D0, [2] dump, [3] getClassName
 * getClassName 在 REF 所有 derived 類別（ProcessCtrlMode, ProcessCtrlParam,
 * ProcessCtrlResult, ProcessCtrlDynamicParameterConfig, ProcessCtrlGatewayBase）
 * 的 vtable slot [3] 都存在，因此必須在 ObjectBase 宣告 */
class ObjectBase {
public:
    ObjectBase();
    virtual ~ObjectBase();
    virtual void dump(unsigned int flags);
    virtual const char* getClassName() const { return "ObjectBase"; }
    static void* operator new(size_t size);
    static void  operator delete(void* p);
};

/* 基本型別 — 與 libcacao_client_real.so 的 ABI 匹配 */
struct ImageSize {
    uint32_t width;
    uint32_t height;
};
enum DataSpace : uint32_t {};
enum ImageFormat : uint32_t {};

/* SharedBuffer — ref-counted 共享緩衝區 handle (sizeof=8)
 * +0: int32_t  refCount
 * +4: pthread_mutex_t mutex
 * 由 libcacao_client_real.so 實作並匯出 */
class SharedBuffer {
public:
    SharedBuffer();
    ~SharedBuffer();
    static SharedBuffer* create();
    static SharedBuffer* share(SharedBuffer* buf);
    static int           release(SharedBuffer* buf);
    void  incRefCnt();
    int   decRefCnt();
private:
    int32_t           refCount; /* +0 */
    pthread_mutex_t   mutex;    /* +4 */
};

} // namespace cacao

namespace cacao_pal {

/* Mutex */
class Mutex {
public:
    Mutex();
    ~Mutex();
    int  lock();
    int  tryLock();
    void unlock();
private:
    char mImpl[0x88];   /* 內部 PAL mutex 資料；無 vtable 故就是完整大小 */
};

/* AutoLock RAII — ctor/dtor 在 PAL 外部函式庫定義（PLT 呼叫），
 * 大小 8 bytes 與 REF ABI 一致：{Mutex* mMutex; int mErrno} */
class AutoLock {
public:
    explicit AutoLock(Mutex& m);
    ~AutoLock();
private:
    void* mMutex;   /* +0: Mutex* (4 bytes) */
    int   mErrno;   /* +4: lock() 回傳值 (4 bytes) */
};

/* Condition — 從 nm: Condition::Condition(Mutex&), timedWait(uint,uint)
 * 無 virtual 方法；sizeof = 0x8C（REF RequestList 從 +0x9C 到 +0x127 = 0x8C bytes） */
class Condition {
public:
    explicit Condition(Mutex& mutex);
    ~Condition();
    void signal();
    int  timedWait(unsigned int sec, unsigned int nsec);
    void wait();
private:
    char mImpl[0x8C];
};

/* ── 範本容器（inline 實作，與原始 .so 內部佈局一致）─────── */

/*
 * 原始 .so 佈局（由 Ghidra 反編譯取得）:
 *
 * PtrList<T> (size 0x88):
 *   +0x00  vtable ptr (由 ctor 設定)
 *   +0x04  count  (uint32_t)
 *   +0x08  root   (Node*)
 *   +0x0C  tail   (Node*)
 *   +0x10..0x87  padding/其他
 *
 * Node<T> (size 0x10):
 *   +0x00  vtable ptr (virtual dtor)
 *   +0x04  data (T*)
 *   +0x08  next (Node*)
 *   +0x0C  prev (Node*)
 *
 * PtrLockList<T> (size 0x9C):
 *   +0x00       vtable ptr (由 ctor 設定)
 *   +0x04       Mutex (size 0x88, 到 +0x8B)
 *   +0x8C       PtrList 開始 (vtable ptr)
 *   +0x90       count  (PtrList+0x04)
 *   +0x94       root   (PtrList+0x08)
 *   +0x98       tail   (PtrList+0x0C)
 *
 * 注意：本 class 不宣告 virtual，所以 this == &mImpl[0]
 */

template<typename T>
class PtrList {
public:
    /* Node 佈局匹配 Ghidra REF: {vtable, data, next, prev}, size=0x10
     * vtable 用於 deleting destructor：removeNode 最後呼叫 node->vtable[1](node) */
    struct Node {
        virtual ~Node() = default;
        T*    data;
        Node* next;
        Node* prev;
    };

    /* inline ctor: 匹配 REF — 直接 inline store，不呼叫 PAL_MemSet */
    inline PtrList() : mCount(0), mRoot(nullptr), mTail(nullptr) {}
    __attribute__((noinline)) virtual ~PtrList() {
        removeAll();
    }

    /* insertNode — 匹配 Ghidra PtrList::insertNode(node, position)
     * position=0: 插入到開頭, position=count: 插入到末尾 */
    __attribute__((noinline)) int insertNode(Node* n, uint32_t position) {
        if (!n) return (int)0xffffff99u;
        if (position > mCount) return (int)0xffffff99u;

        if (position == 0) {
            if (mRoot == nullptr) {
                mRoot = n;
                mTail = n;
            } else {
                n->next = mRoot;
                mRoot->prev = n;
                mRoot = n;
            }
        } else if (position == mCount) {
            if (mTail == nullptr) {
                mRoot = n;
                mTail = n;
            } else {
                n->prev = mTail;
                mTail->next = n;
                mTail = n;
            }
        } else {
            Node* cur = mRoot;
            for (uint32_t i = 0; i < position && cur; i++)
                cur = cur->next;
            if (!cur) return (int)0xffffff99u;
            n->next = cur;
            n->prev = cur->prev;
            if (cur->prev)
                cur->prev->next = n;
            cur->prev = n;
        }
        mCount++;
        return 0;
    }

    /* removeNode — 匹配 Ghidra: 從 list 移除 node 後透過 vtable[1] 刪除 */
    __attribute__((noinline)) void removeNode(Node* n) {
        if (!n) return;
        if (mRoot == n)
            mRoot = n->next;
        if (mTail == n)
            mTail = n->prev;
        if (n->prev)
            n->prev->next = n->next;
        if (n->next)
            n->next->prev = n->prev;
        mCount--;
        delete n;
    }

    /* pushLast — 匹配 Ghidra: new Node, set fields, insertNode(this, node, count) */
    __attribute__((noinline)) int pushLast(T* item) {
        if (!item) return (int)0xffffff99u;
        Node* n = new Node();
        n->data = item;
        n->next = nullptr;
        n->prev = nullptr;
        return insertNode(n, mCount);
    }

    /* pushRoot — 匹配 Ghidra: new Node, set fields, insertNode(this, node, 0) */
    __attribute__((noinline)) int pushRoot(T* item) {
        if (!item) return (int)0xffffff99u;
        Node* n = new Node();
        n->data = item;
        n->next = nullptr;
        n->prev = nullptr;
        return insertNode(n, 0);
    }

    int  insert(T* item);
    T*   getNode(unsigned idx) const;

    __attribute__((noinline)) int remove(T* item) {
        Node* cur = mRoot;
        for (uint32_t i = 0; i < mCount && cur; i++, cur = cur->next) {
            if (cur->data == item) {
                removeNode(cur);
                return 0;
            }
        }
        return -1;
    }

    __attribute__((noinline)) int removeAll() {
        while (mRoot) {
            removeNode(mRoot);
        }
        return 0;
    }

    __attribute__((noinline)) int getIndex(T* item) const {
        const Node* cur = mRoot;
        for (uint32_t i = 0; i < mCount && cur; i++, cur = cur->next) {
            if (cur->data == item)
                return static_cast<int>(i);
        }
        return -1;
    }

public:  /* 欄位公開以允許 PtrLockList 存取（原版為 raw memory，Go layout match） */
    uint32_t mCount;  /* +0x04 = PtrList[+0x04], PtrLockList[+0x90] */
    Node*    mRoot;   /* +0x08 = PtrList[+0x08], PtrLockList[+0x94] */
    Node*    mTail;   /* +0x0C = PtrList[+0x0C], PtrLockList[+0x98] */
};

template<typename T>
class PtrLockList {
public:
    /* inline ctor — 成員自動初始化（Mutex() + PtrList() inline）
     * 匹配 REF：不呼叫 memset，使用 inline store 設置 mList.count/root/tail=0 及 vtable */
    inline PtrLockList() {}
    __attribute__((noinline)) virtual ~PtrLockList() {
        /* Ghidra: ~PtrLockList body 呼叫 PtrLockList::removeAll()（帶 mutex lock）
         * C++ 會自動析構成員：mList（~PtrList）先，然後 mMutex（~Mutex）。
         * 不可明確呼叫 mMutex.~Mutex() — 會雙重析構。 */
        removeAll();
    }

    /* virtual removeNode — 匹配 REF PtrLockList vtable slot[2]
     * 供 PtrLockListDel 覆寫以實作「刪除 data 再解除連結」語意 */
    __attribute__((noinline)) virtual void removeNode(typename PtrList<T>::Node* n) {
        mList.removeNode(n);
    }

    /* -- 以下為 noinline 實作：匹配 Ghidra (原版為獨立函式，非 inline) -- */

    __attribute__((noinline)) int pushLast(T* item) {
        /* 匹配 Ghidra: lock(mutex), PtrList::pushLast(+0x8c, item), unlock */
        AutoLock lk(mMutex);
        return mList.pushLast(item);
    }

    inline int pushRoot(T* item) {
        /* 匹配 Ghidra: lock(mutex), PtrList::pushRoot(+0x8c, item), unlock */
        AutoLock lk(mMutex);
        return mList.pushRoot(item);
    }

    __attribute__((noinline)) T* getRoot() const {
        AutoLock lk(const_cast<Mutex&>(mMutex));
        return mList.mRoot ? mList.mRoot->data : nullptr;
    }

    __attribute__((noinline)) T* popRoot() {
        AutoLock lk(mMutex);
        typename PtrList<T>::Node* r = mList.mRoot;
        if (!r) return nullptr;
        T* data = r->data;
        mList.removeNode(r);
        return data;
    }

    __attribute__((noinline)) T* popLast() {
        AutoLock lk(mMutex);
        typename PtrList<T>::Node* t = mList.mTail;
        if (!t) return nullptr;
        T* data = t->data;
        mList.removeNode(t);
        return data;
    }

    __attribute__((noinline)) int removeRoot() {
        AutoLock lk(mMutex);
        if (!mList.mRoot) return -0x68;
        mList.removeNode(mList.mRoot);
        return 0;
    }

    /* 匹配 REF PtrLockList::get(ListPos, compareFn, searchData) → T* */
    __attribute__((noinline)) T* findOne(int listPos,
                            bool (*compareFn)(T* item, void* data),
                            void* searchData) const {
        AutoLock lk(const_cast<Mutex&>(mMutex));
        typename PtrList<T>::Node* cur = (listPos == 0) ? mList.mRoot : mList.mTail;
        if (!compareFn) return cur ? cur->data : nullptr;
        for (uint32_t i = 0; i < mList.mCount && cur; i++) {
            if (compareFn(cur->data, searchData)) return cur->data;
            cur = (listPos == 0) ? cur->next : cur->prev;
        }
        return nullptr;
    }

    __attribute__((noinline)) unsigned int get(int type, const unsigned char* comparator,
                            unsigned int key, unsigned int flags,
                            void* extra, void* result) const {
        AutoLock lk(const_cast<Mutex&>(mMutex));
        typename PtrList<T>::Node* cur = mList.mRoot;
        uint32_t* arr = reinterpret_cast<uint32_t*>(result);
        uint32_t resultCount = 0;
        typedef bool (*CompareFn)(T* item, void* key);
        CompareFn cmpFn = reinterpret_cast<CompareFn>(const_cast<unsigned char*>(comparator));
        uint32_t maxCount = (flags == 0) ? mList.mCount : flags;
        for (uint32_t i = 0; i < mList.mCount && cur && resultCount < maxCount; i++, cur = cur->next) {
            bool match = (!cmpFn) || cmpFn(cur->data, reinterpret_cast<void*>((uintptr_t)key));
            if (match) {
                if (arr[2] == 0) {
                    arr[2] = reinterpret_cast<uint32_t>(
                        PAL_MemAllocEx(0, sizeof(T*) * mList.mCount, __FILE__, __LINE__, 0));
                    if (!arr[2]) break;
                }
                reinterpret_cast<T**>(arr[2])[resultCount] = cur->data;
                resultCount++;
            }
        }
        arr[1] = resultCount;
        return resultCount;
    }

    __attribute__((noinline)) T* get(int listPos,
                  bool (*comparatorFn)(T*, void*), void* userData) const {
        AutoLock lk(const_cast<Mutex&>(mMutex));
        typename PtrList<T>::Node* cur = (listPos == 0) ? mList.mRoot : mList.mTail;
        if (!comparatorFn) return cur ? cur->data : nullptr;
        for (uint32_t i = 0; i < mList.mCount && cur; i++) {
            if (comparatorFn(cur->data, userData)) return cur->data;
            cur = (listPos == 0) ? cur->next : cur->prev;
        }
        return nullptr;
    }

    __attribute__((noinline)) T* pop(int listPos,
                  bool (*comparatorFn)(T*, void*), void* userData) const {
        AutoLock lk(const_cast<Mutex&>(mMutex));
        typename PtrList<T>::Node* cur = (listPos == 0) ? mList.mRoot : mList.mTail;
        if (!comparatorFn) {
            if (!cur) return nullptr;
            T* data = cur->data;
            const_cast<PtrLockList<T>*>(this)->mList.removeNode(cur);
            return data;
        }
        for (uint32_t i = 0; i < mList.mCount && cur; i++) {
            typename PtrList<T>::Node* next = cur->next;
            if (comparatorFn(cur->data, userData)) {
                T* data = cur->data;
                const_cast<PtrLockList<T>*>(this)->mList.removeNode(cur);
                return data;
            }
            cur = next;
        }
        return nullptr;
    }

    __attribute__((noinline)) int cancel(unsigned int key) {
        AutoLock lk(mMutex);
        (void)key;
        return 0;
    }

    __attribute__((noinline)) int size() const {
        AutoLock lk(const_cast<Mutex&>(mMutex));
        return static_cast<int>(mList.mCount);
    }

    __attribute__((noinline)) bool isEmpty() const {
        AutoLock lk(const_cast<Mutex&>(mMutex));
        return mList.mCount == 0;
    }

    __attribute__((noinline)) int getIndex(T* item) const {
        AutoLock lk(const_cast<Mutex&>(mMutex));
        return mList.getIndex(item);
    }

    int          start(T* item);
    int          finish(T* item);
    int          remove(T* item);
    bool         compareRequestState(T* item, int state) const;

    __attribute__((noinline)) int removeAll() {
        AutoLock lk(mMutex);
        return mList.removeAll();
    }

protected:
    Mutex       mMutex;  /* +0x04, size 0x88 */
    PtrList<T>  mList;   /* +0x8C，C++ 自動設置 vtable at [this+0x8C] */
};

template<typename T>
class Array {
public:
    inline Array() {
        /* REF 佈局（含 vptr）:
         * +0x00 vptr
         * +0x04 count
         * +0x08 data_ptr
         * +0x0c reserved */
        *reinterpret_cast<uint32_t*>(mImpl + 0x00) = 0;
        *reinterpret_cast<void**>(mImpl + 0x04) = nullptr;
        *reinterpret_cast<uint32_t*>(mImpl + 0x08) = 0;
    }
    __attribute__((noinline)) virtual ~Array() {
        free();
    }

    inline int alloc(unsigned size) {
        uint32_t& cnt = *reinterpret_cast<uint32_t*>(mImpl + 0x00);
        void*& ptr = *reinterpret_cast<void**>(mImpl + 0x04);
        int ret = 0;
        if (size == 0) ret = (int)0xffffff99u;
        if (ptr != nullptr) ret = (int)0xffffff93u;
        if (size != 0 && ptr == nullptr) {
            ptr = PAL_MemAllocEx(0, sizeof(T) * size, __FILE__, __LINE__, 0);
            if (!ptr) { ret = (int)0xffffff9au; }
            else { cnt = size; }
        }
        return ret;
    }

    inline void free() {
        uint32_t& cnt = *reinterpret_cast<uint32_t*>(mImpl + 0x00);
        void*& ptr = *reinterpret_cast<void**>(mImpl + 0x04);
        if (ptr) {
            PAL_MemFreeEx(ptr, 0);
            cnt = 0;
            ptr = nullptr;
        }
    }

    __attribute__((noinline)) T& operator[](unsigned idx) {
        uint32_t cnt = *reinterpret_cast<uint32_t*>(mImpl + 0x00);
        int err = 0;
        if (cnt < idx) {
            PAL_LogPrint(__FILE__, 0x108, 2, 1, "Array::operator[] out of range cnt=%u idx=%u", cnt, idx);
            cnt = *reinterpret_cast<uint32_t*>(mImpl + 0x00);
            err = -0x67;
        }
        if (cnt == 0 || *reinterpret_cast<void**>(mImpl + 0x04) == nullptr) {
            PAL_LogPrint(__FILE__, 0x10c, 2, 1, "Array::operator[] null cnt=%u idx=%u", cnt, idx);
        } else if (err >= 0) {
            T* p = *reinterpret_cast<T**>(mImpl + 0x04);
            return p[idx];
        }
        /* 錯誤恢復：返回 nullptr 參考（REF 行為：直接存取 mImpl[0]= ptr 的 0 地址）*/
        return *reinterpret_cast<T*>(mImpl + 0x00);
    }

    inline unsigned int getNodes() const {
        return *reinterpret_cast<const uint32_t*>(mImpl + 0x00);
    }

private:
    char mImpl[0x0c];  /* 物件總長 0x10: vptr(4) + mImpl(12) */
};

/* PtrLockListDel — 繼承 PtrLockList，覆寫 removeNode 以刪除 data 物件
 * REF 析構順序：先呼叫 removeAll（觸發 removeNode 刪除 data），再呼叫 ~PtrLockList */
template<typename T>
class PtrLockListDel : public PtrLockList<T> {
public:
    inline PtrLockListDel() = default;

    __attribute__((noinline)) virtual ~PtrLockListDel() {
        /* 匹配 REF ~PtrLockListDel：先 removeAll（刪除 data），然後 tail-call ~PtrLockList */
        this->removeAll();
    }

    /* 覆寫 removeNode：刪除 data 物件後再從 inner list 移除 node
     * 匹配 REF PtrLockListDel::removeNode：(*node->data->vtable[1])(node->data) */
    __attribute__((noinline)) virtual void removeNode(typename PtrList<T>::Node* n) override {
        if (!n) return;
        if (n->data) {
            int** vtbl = *reinterpret_cast<int***>(n->data);
            reinterpret_cast<void(*)(void*)>(vtbl[1])(n->data);
            n->data = nullptr;
        }
        this->mList.removeNode(n);
    }
};

} /* namespace cacao_pal */
#endif /* __cplusplus */
