// ICacaoService.cpp — android::ICacaoService Binder 介面實作
// 包含: ICacaoService, BpCacaoService, BnCacaoService::onTransact
// 依照 libcacao_client.so (ARM 32-bit) Ghidra 反編譯結果 100% 重建
// Interface descriptor: "com.sonymobile.cacao.ICacaoService"
//
// BnCacaoService::onTransact transaction codes:
//   code 1 → connect (vtable+0x10)
//   code 2 → getServicePid (vtable+0x14)
//   code 3 → getCaps (vtable+0x18)
//   code 4 → getJpegBufferSize (vtable+0x1c)
//
// SerializedData Binder 序列化協議:
//   傳送方: writeStrongBinder(IMemory), writeInt32(fdCount), writeFileDescriptor(fd[i]) × fdCount
//   接收方: readStrongBinder→IMemory, 讀 fdCount 個 fd 填入 sd._pad, 從 IMemory 設定 sd.size/data

#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/IMemory.h>
#include <binder/Parcel.h>
#include <log/log.h>
#include <unistd.h>
#include <string.h>

#include <cacao/ICacaoService.h>
#include <cacao/ICacao.h>
#include <cacao/ICacaoClient.h>
#include <cacao/binder_sd_helpers.h>

#define LOG_TAG "ICacaoService"

namespace android {

// ══════════════════════════════════════════════════════════════
// ICacaoService 基礎介面實作
// ══════════════════════════════════════════════════════════════

const String16 ICacaoService::descriptor("com.sonymobile.cacao.ICacaoService");

ICacaoService::ICacaoService() {}
ICacaoService::~ICacaoService() {}

const String16& ICacaoService::getInterfaceDescriptor() const {
    return descriptor;
}

// ══════════════════════════════════════════════════════════════
// BpCacaoService — Binder Proxy（客戶端使用，位於遠端 process 呼叫時）
// ══════════════════════════════════════════════════════════════

class BpCacaoService : public BpInterface<ICacaoService> {
public:
    explicit BpCacaoService(const sp<IBinder>& impl)
        : BpInterface<ICacaoService>(impl)
    {}

    virtual ~BpCacaoService() {}

    // code 1: connect(sp<ICacaoClient>) → sp<ICacao>
    virtual sp<ICacao> connect(const sp<ICacaoClient>& client) override {
        Parcel data, reply;
        data.writeInterfaceToken(ICacaoService::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(client));
        remote()->transact(1, data, &reply, 0);
        sp<ICacao> result = ICacao::asInterface(reply.readStrongBinder());
        return result;
    }

    // code 2: getServicePid() → int
    virtual int getServicePid() override {
        Parcel data, reply;
        data.writeInterfaceToken(ICacaoService::getInterfaceDescriptor());
        remote()->transact(2, data, &reply, 0);
        return reply.readInt32();
    }

    // code 3: getCaps(CameraIndex, sp<IMemory>, SerializedData) → int
    virtual int getCaps(const cacao::ProcessCtrlCaps::CameraIndex& camIdx,
                        const sp<IMemory>& mem,
                        cacao::ISerialize::SerializedData sd) override {
        Parcel data, reply;
        data.writeInterfaceToken(ICacaoService::getInterfaceDescriptor());
        data.writeInt32(camIdx.index);
        data.writeStrongBinder(IInterface::asBinder(mem));
        write_sd(data, sd);
        remote()->transact(3, data, &reply, 0);
        return reply.readInt32();
    }

    // code 4: getJpegBufferSize(ImageSize) → int
    virtual int getJpegBufferSize(cacao::ImageSize size) override {
        Parcel data, reply;
        data.writeInterfaceToken(ICacaoService::getInterfaceDescriptor());
        data.writeInt32(size.width);
        data.writeInt32(size.height);
        remote()->transact(4, data, &reply, 0);
        return reply.readInt32();
    }
};

// IMPLEMENT_META_INTERFACE equivalent:
// asInterface 手動實作（依照 Ghidra 反編譯邏輯）
sp<ICacaoService> ICacaoService::asInterface(const sp<IBinder>& obj) {
    sp<ICacaoService> intr;
    if (obj != nullptr) {
        intr = static_cast<ICacaoService*>(
            obj->queryLocalInterface(ICacaoService::descriptor).get());
        if (intr == nullptr) {
            intr = new BpCacaoService(obj);
        }
    }
    return intr;
}

// ══════════════════════════════════════════════════════════════
// BnCacaoService::onTransact — Binder Stub 分派器
// 在 CacaoService (libcacao_service_real.so) 中，此 stub 被 CacaoService::onTransact 呼叫
// 具體虛擬方法由 libcacao_service_real.so 的 CacaoService 實作
// ══════════════════════════════════════════════════════════════

status_t BnCacaoService::onTransact(uint32_t code,
                                     const Parcel& data,
                                     Parcel* reply,
                                     uint32_t flags)
{
    switch (code) {
    case 1: {
        // connect(sp<ICacaoClient>) → sp<ICacao>
        // 原始 .so: checkInterface → readStrongBinder → ICacaoClient::asInterface
        //            → connect(client) → writeStrongBinder(result)
        if (!data.checkInterface(this)) return -1;
        sp<ICacaoClient> client =
            ICacaoClient::asInterface(data.readStrongBinder());
        sp<ICacao> cacao = connect(client);
        reply->writeStrongBinder(IInterface::asBinder(cacao));
        return NO_ERROR;
    }
    case 2: {
        // getServicePid() → int
        if (!data.checkInterface(this)) return -1;
        int pid = getServicePid();
        reply->writeInt32(pid);
        return NO_ERROR;
    }
    case 3: {
        // getCaps(CameraIndex const&, sp<IMemory>, SerializedData) → int
        // 原始 .so: checkInterface → readInt32(camIdx) → readStrongBinder(IMemory)
        //            → reconstruct SD from FDs + IMemory → getCaps → writeInt32
        if (!data.checkInterface(this)) return -1;
        cacao::ProcessCtrlCaps::CameraIndex camIdx;
        camIdx.index = data.readInt32();
        sp<IMemory> mem = interface_cast<IMemory>(data.readStrongBinder());
        cacao::ISerialize::SerializedData sd;
        reconstruct_sd(data, mem, sd);
        int ret = getCaps(camIdx, mem, sd);
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    case 4: {
        // getJpegBufferSize(ImageSize) → int
        // 原始 .so: checkInterface → readInt32 × 2 → getJpegBufferSize → writeInt32
        if (!data.checkInterface(this)) return -1;
        cacao::ImageSize size;
        size.width  = data.readInt32();
        size.height = data.readInt32();
        int ret = getJpegBufferSize(size);
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    default:
        return BBinder::onTransact(code, data, reply, flags);
    }
}

} // namespace android
