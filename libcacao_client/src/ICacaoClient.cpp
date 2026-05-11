// ICacaoClient.cpp — android::ICacaoClient Binder 介面實作
// 包含: ICacaoClient, BpCacaoClient, BnCacaoClient::onTransact
// 依照 libcacao_client.so (ARM 32-bit) Ghidra 反編譯結果 100% 重建
// Interface descriptor: "com.sonymobile.cacao.ICacaoClient"
//
// BnCacaoClient::onTransact transaction codes:
//   code 1 → notifyEvent(int eventType, sp<IMemory>, SerializedData) (無 reply)

#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/IMemory.h>
#include <binder/Parcel.h>
#include <log/log.h>
#include <unistd.h>
#include <string.h>

#include <cacao/ICacaoClient.h>
#include <cacao/binder_sd_helpers.h>

#define LOG_TAG "ICacaoClient"

namespace android {

// ══════════════════════════════════════════════════════════════
// ICacaoClient 基礎介面實作
// ══════════════════════════════════════════════════════════════

const String16 ICacaoClient::descriptor("com.sonymobile.cacao.ICacaoClient");

ICacaoClient::ICacaoClient() {}
ICacaoClient::~ICacaoClient() {}

const String16& ICacaoClient::getInterfaceDescriptor() const {
    return descriptor;
}

// ══════════════════════════════════════════════════════════════
// BpCacaoClient — Binder Proxy (先宣告，供 asInterface 使用)
// ══════════════════════════════════════════════════════════════

class BpCacaoClient : public BpInterface<ICacaoClient> {
public:
    explicit BpCacaoClient(const sp<IBinder>& impl)
        : BpInterface<ICacaoClient>(impl)
    {}

    virtual ~BpCacaoClient() {}

    // code 1: notifyEvent(int eventType, sp<IMemory>, SerializedData) — 無 reply
    // 原始 .so: writeToken → writeInt32(eventType) → writeStrongBinder(mem) → write_sd
    //          → transact(1, IBinder::FLAG_ONEWAY)
    virtual void notifyEvent(int eventType,
                             const sp<IMemory>& mem,
                             cacao::ISerialize::SerializedData sd) override {
        Parcel data, reply;
        data.writeInterfaceToken(ICacaoClient::getInterfaceDescriptor());
        data.writeInt32(eventType);
        data.writeStrongBinder(IInterface::asBinder(mem));
        write_sd(data, sd);
        remote()->transact(1, data, &reply, IBinder::FLAG_ONEWAY);
    }
};

sp<ICacaoClient> ICacaoClient::asInterface(const sp<IBinder>& obj) {
    sp<ICacaoClient> intr;
    if (obj != nullptr) {
        intr = static_cast<ICacaoClient*>(
            obj->queryLocalInterface(ICacaoClient::descriptor).get());
        if (intr == nullptr) {
            intr = new BpCacaoClient(obj);
        }
    }
    return intr;
}

// ══════════════════════════════════════════════════════════════
// BnCacaoClient::onTransact — Binder Stub 分派器
// Client process 中，Java 層回呼的接收端
// ══════════════════════════════════════════════════════════════

status_t BnCacaoClient::onTransact(uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags)
{
    switch (code) {
    case 1: {
        // notifyEvent(int eventType, sp<IMemory>, SerializedData) — 無 reply
        // 原始 .so: checkInterface → readInt32(eventType) →
        //           readStrongBinder(IMemory) → readFDs → reconstruct SD → call virtual
        if (!data.checkInterface(this)) return -1;
        int eventType = data.readInt32();
        sp<IMemory> mem = interface_cast<IMemory>(data.readStrongBinder());
        cacao::ISerialize::SerializedData sd;
        reconstruct_sd(data, mem, sd);
        notifyEvent(eventType, mem, sd);
        return NO_ERROR;
    }
    default:
        return BBinder::onTransact(code, data, reply, flags);
    }
}

} // namespace android
