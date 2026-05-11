// ICacaoService.h — android::ICacaoService 介面宣告
// 依照 libcacao_client.so 反編譯結果重建
// ICacaoService 的 ctor/dtor/descriptor 由 libcacao_client_real.so 提供
// BnCacaoService::onTransact 也由 libcacao_client_real.so 提供
// 本檔案僅提供 CacaoService 編譯所需的類型宣告

#pragma once
#include <binder/IInterface.h>
#include <binder/IMemory.h>
#include <cacao/ProcessCtrlGatewayBase.h>

namespace cacao {
namespace ProcessCtrlCaps {
    struct CameraIndex {
        int index;  // 0 或 1
    };
}
} // namespace cacao

namespace android {

class ICacao;
class ICacaoClient;

// ICacaoService 純虛介面
// BnCacaoService::onTransact 中的 4 個 transaction code:
//   code 1 → connect (vtable+0x10)
//   code 2 → getServicePid (vtable+0x14)
//   code 3 → getCaps (vtable+0x18)
//   code 4 → getJpegBufferSize (vtable+0x1c)
class ICacaoService : public IInterface {
public:
    ICacaoService();
    virtual ~ICacaoService();

    static sp<ICacaoService> asInterface(const sp<IBinder>& obj);
    static const String16 descriptor;
    virtual const String16& getInterfaceDescriptor() const;

    // vtable slot 4 (offset +0x10) — tx code 1
    virtual sp<ICacao> connect(const sp<ICacaoClient>& client) = 0;
    // vtable slot 5 (offset +0x14) — tx code 2
    virtual int getServicePid() = 0;
    // vtable slot 6 (offset +0x18) — tx code 3
    virtual int getCaps(const cacao::ProcessCtrlCaps::CameraIndex& camIdx,
                        const sp<IMemory>& mem,
                        cacao::ISerialize::SerializedData sd) = 0;
    // vtable slot 7 (offset +0x1c) — tx code 4
    virtual int getJpegBufferSize(cacao::ImageSize size) = 0;
};

// BnCacaoService — Binder native stub
// onTransact 由 libcacao_client_real.so 提供
class BnCacaoService : public BnInterface<ICacaoService> {
public:
    virtual status_t onTransact(uint32_t code, const Parcel& data,
                                Parcel* reply, uint32_t flags = 0);
};

} // namespace android