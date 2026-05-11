// ICacaoProcessCallback.cpp — android::ICacaoProcessCallback Binder 介面實作
// 包含: ICacaoProcessCallback, BpCacaoProcessCallback, BnCacaoProcessCallback::onTransact
// 依照 libcacao_client.so (ARM 32-bit) Ghidra 反編譯結果 100% 重建
// Interface descriptor: "com.sonymobile.cacao.ICacaoProcessCallback"
//
// BnCacaoProcessCallback::onTransact transaction codes:
//   code 1 → notifyProgress(sp<IMemory>, SerializedData) (one-way)
//   code 2 → notifyResult(sp<IMemory>, SerializedData, Vector<ImageBuf*>*) (one-way)

#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/IMemory.h>
#include <binder/Parcel.h>
#include <utils/Vector.h>
#include <log/log.h>
#include <unistd.h>
#include <string.h>
#include <cutils/native_handle.h>

#include <cacao/ICacaoProcessCallback.h>
#include <cacao/binder_sd_helpers.h>

#define LOG_TAG "ICacaoProcessCallback"

namespace android {

// ══════════════════════════════════════════════════════════════
// ICacaoProcessCallback 基礎介面實作
// ══════════════════════════════════════════════════════════════

const String16 ICacaoProcessCallback::descriptor(
    "com.sonymobile.cacao.ICacaoProcessCallback");

ICacaoProcessCallback::ICacaoProcessCallback() {}
ICacaoProcessCallback::~ICacaoProcessCallback() {}

const String16& ICacaoProcessCallback::getInterfaceDescriptor() const {
    return descriptor;
}

// ══════════════════════════════════════════════════════════════
// BpCacaoProcessCallback — Binder Proxy (先宣告，供 asInterface 使用)
// ══════════════════════════════════════════════════════════════

class BpCacaoProcessCallback : public BpInterface<ICacaoProcessCallback> {
public:
    explicit BpCacaoProcessCallback(const sp<IBinder>& impl)
        : BpInterface<ICacaoProcessCallback>(impl)
    {}

    virtual ~BpCacaoProcessCallback() {}

    // code 1: notifyProgress(sp<IMemory>, SerializedData)
    virtual void notifyProgress(const sp<IMemory>& mem,
                                cacao::ISerialize::SerializedData sd) override {
        Parcel data, reply;
        data.writeInterfaceToken(ICacaoProcessCallback::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(mem));
        write_sd(data, sd);
        remote()->transact(1, data, &reply, 0);
    }

    // code 2: notifyResult(sp<IMemory>, SerializedData, Vector<ImageBuf*>*)
    // Ghidra 64-bit@0x14cdc 確認：Bp 端只寫 IMemory + SerializedData
    // ImageBuf vector 不通過 Binder 傳輸，transact 後本地清理
    virtual void notifyResult(const sp<IMemory>& mem,
                              cacao::ISerialize::SerializedData sd,
                              android::Vector<cacao::ImageBuf*>* bufs) override {
        Parcel data, reply;
        data.writeInterfaceToken(ICacaoProcessCallback::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(mem));
        write_sd(data, sd);
        remote()->transact(2, data, &reply, 0);

        // transact 後清理 ImageBuf vector
        // Ghidra 64-bit 確認：getNative → native_handle_close → native_handle_delete → D0 dtor
        if (bufs != nullptr && bufs->size() > 0) {
            for (size_t i = 0; i < bufs->size(); i++) {
                cacao::ImageBuf* buf = (*bufs)[i];
                if (buf != nullptr) {
                    void* nh_raw = buf->getNative();
                    if (nh_raw != nullptr) {
                        native_handle_close(static_cast<const native_handle_t*>(nh_raw));
                        native_handle_delete(static_cast<native_handle_t*>(nh_raw));
                    }
                    delete buf;
                }
            }
        }
    }
};

sp<ICacaoProcessCallback> ICacaoProcessCallback::asInterface(const sp<IBinder>& obj) {
    sp<ICacaoProcessCallback> intr;
    if (obj != nullptr) {
        intr = static_cast<ICacaoProcessCallback*>(
            obj->queryLocalInterface(ICacaoProcessCallback::descriptor).get());
        if (intr == nullptr) {
            intr = new BpCacaoProcessCallback(obj);
        }
    }
    return intr;
}

// ══════════════════════════════════════════════════════════════
// BnCacaoProcessCallback::onTransact — Binder Stub 分派器
// CacaoProcessCallback (CacaoClient 內嵌) 的接收端
// ══════════════════════════════════════════════════════════════

status_t BnCacaoProcessCallback::onTransact(uint32_t code,
                                             const Parcel& data,
                                             Parcel* reply,
                                             uint32_t flags)
{
    switch (code) {
    case 1: {
        // notifyProgress(sp<IMemory>, SerializedData) — one-way
        // 原始 .so: checkInterface → readStrongBinder(IMemory) → readFDs → reconstruct SD
        //          → call virtual notifyProgress
        if (!data.checkInterface(this)) return -1;
        sp<IMemory> mem = interface_cast<IMemory>(data.readStrongBinder());
        cacao::ISerialize::SerializedData sd;
        reconstruct_sd(data, mem, sd);
        notifyProgress(mem, sd);
        return NO_ERROR;
    }
    case 2: {
        // notifyResult(sp<IMemory>, SerializedData, Vector<ImageBuf*>*) — one-way
        // Ghidra 64-bit@0x14684 確認：Bn 端只讀 IMemory + SerializedData
        // 第三參數 (ImageBuf vector) 傳 nullptr — ImageBuf 不通過 Binder 傳輸
        if (!data.checkInterface(this)) return -1;
        sp<IMemory> mem = interface_cast<IMemory>(data.readStrongBinder());
        cacao::ISerialize::SerializedData sd;
        reconstruct_sd(data, mem, sd);

        notifyResult(mem, sd, nullptr);
        return NO_ERROR;
    }
    default:
        return BBinder::onTransact(code, data, reply, flags);
    }
}

} // namespace android
