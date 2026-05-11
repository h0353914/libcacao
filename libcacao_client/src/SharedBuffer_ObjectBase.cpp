// SharedBuffer_ObjectBase.cpp
// cacao::ObjectBase 與 cacao::SharedBuffer 實作
// 反編譯來源: src/libcacao_client.cpp 5082-5205 行（Ghidra pseudocode）

#define LOG_TAG "libcacao_client"
#include <cacao/cacao_pal.h>
#include <log/log.h>

// PAL 記憶體函式（由 libcacao_pal.so 提供）
extern "C" void* PAL_MemAllocEx(int pool, size_t size, const char* file, int line, int flags);
extern "C" void  PAL_MemFreeEx(void* ptr, int flags);

namespace cacao {

// ──────────────────────────────────────────────────────────
// ObjectBase
// ──────────────────────────────────────────────────────────

// 建構子（vtable 由 C++ compiler 自動設定，不需手動操作）
ObjectBase::ObjectBase()
{
}

// 解構子
ObjectBase::~ObjectBase()
{
}

// dump — 基底類別無操作（line 5082-5084 in ref）
void ObjectBase::dump(unsigned int /*flags*/)
{
}

// operator new — PAL_MemAllocEx(pool=0, size, file, line=0x39, flags=0)
// 對應 Ghidra: pvVar1 = PAL_MemAllocEx(0, this, DAT+file_str, 0x39, 0)
void* ObjectBase::operator new(size_t size)
{
    return PAL_MemAllocEx(0, size, __FILE__, 0x39, 0);
}

// operator delete — PAL_MemFreeEx(ptr, 0)
void ObjectBase::operator delete(void* ptr)
{
    PAL_MemFreeEx(ptr, 0);
}

// ──────────────────────────────────────────────────────────
// SharedBuffer
// sizeof = 8: { int32_t refCount@0, pthread_mutex_t mutex@4 }
// ──────────────────────────────────────────────────────────

// 建構子: refCount=0, mutex_init
// Ghidra 5189-5196: *(int*)this = 0; pthread_mutex_init(&mutex, nullptr)
SharedBuffer::SharedBuffer()
    : refCount(0)
{
    pthread_mutex_init(&mutex, nullptr);
}

// 解構子: mutex_destroy
// Ghidra 5200-5204: pthread_mutex_destroy(&(this+4))
SharedBuffer::~SharedBuffer()
{
    pthread_mutex_destroy(&mutex);
}

// create — 以 PAL_MemAllocEx 分配，初始化，incRefCnt
// Ghidra 5116-5129:
//   this = operator_new(8)
//   *(int*)this = 0
//   pthread_mutex_init(this+4, 0)
//   incRefCnt(this)
SharedBuffer* SharedBuffer::create()
{
    SharedBuffer* sb = new SharedBuffer();
    sb->incRefCnt();
    return sb;
}

// incRefCnt — lock, ++refCount, unlock
// Ghidra 5131-5140
void SharedBuffer::incRefCnt()
{
    pthread_mutex_lock(&mutex);
    refCount++;
    pthread_mutex_unlock(&mutex);
}

// decRefCnt — lock, if>0 --refCount, unlock, return new count
// Ghidra 5171-5184
int SharedBuffer::decRefCnt()
{
    pthread_mutex_lock(&mutex);
    int cnt = refCount;
    if (cnt > 0) {
        cnt--;
        refCount = cnt;
    }
    pthread_mutex_unlock(&mutex);
    return cnt;
}

// share — if(buf) incRefCnt, return buf
// Ghidra 5143-5152
SharedBuffer* SharedBuffer::share(SharedBuffer* buf)
{
    if (buf) {
        buf->incRefCnt();
    }
    return buf;
}

// release — if(buf && decRefCnt()==0): mutex_destroy, delete, return 1
//           else return 0
// Ghidra 5155-5167
int SharedBuffer::release(SharedBuffer* buf)
{
    if (buf && buf->decRefCnt() == 0) {
        pthread_mutex_destroy(&buf->mutex);
        // 直接釋放記憶體，不走 ~SharedBuffer（避免重複 mutex_destroy）
        PAL_MemFreeEx(buf, 0);
        return 1;
    }
    return 0;
}

} // namespace cacao
