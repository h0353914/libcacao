// ICacaoClient.h — android::ICacaoClient Binder 回呼介面宣告
// 由 libcacao_client_real.so 提供實際的 ctor/dtor/descriptor
// CacaoService::Client 持有 sp<ICacaoClient> 用於回呼通知 Java 層

#pragma once
#include <binder/IInterface.h>
#include <binder/IMemory.h>
#include <cacao/ProcessCtrlGatewayBase.h>

namespace android {

// ICacaoClient — 相機客戶端回呼介面
// BnCacaoClient::onTransact 在 libcacao_client.so 中
// 虛函式順序從 BpCacaoClient proxy 推導
class ICacaoClient : public IInterface {
public:
    ICacaoClient();
    virtual ~ICacaoClient();

    static const String16 descriptor;
    virtual const String16& getInterfaceDescriptor() const;

    // vtable slot 4 — 通知事件
    virtual void notifyEvent(int eventType,
                             const sp<IMemory>& mem,
                             cacao::ISerialize::SerializedData sd) = 0;

    static sp<ICacaoClient> asInterface(const sp<IBinder>& obj);
};

class BnCacaoClient : public BnInterface<ICacaoClient> {
public:
    virtual status_t onTransact(uint32_t code, const Parcel& data,
                                Parcel* reply, uint32_t flags) override;
};

} // namespace android