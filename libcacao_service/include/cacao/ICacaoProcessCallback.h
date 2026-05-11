// ICacaoProcessCallback.h — android::ICacaoProcessCallback Binder 回呼介面宣告
// 由 libcacao_client_real.so 提供實際的 ctor/dtor/descriptor

#pragma once
#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/IMemory.h>
#include <binder/Parcel.h>
#include <utils/Vector.h>
#include <cacao/ProcessCtrlGatewayBase.h>

namespace android {

class ICacaoProcessCallback : public IInterface {
public:
    ICacaoProcessCallback();
    virtual ~ICacaoProcessCallback();

    static const String16 descriptor;
    virtual const String16& getInterfaceDescriptor() const;

    // vtable slot 4 — transaction 1
    virtual void notifyProgress(const sp<IMemory>& mem,
                                cacao::ISerialize::SerializedData sd) = 0;
    // vtable slot 5 — transaction 2
    virtual void notifyResult(const sp<IMemory>& mem,
                              cacao::ISerialize::SerializedData sd,
                              android::Vector<cacao::ImageBuf*>* bufs) = 0;

    static sp<ICacaoProcessCallback> asInterface(const sp<IBinder>& obj);
};

class BnCacaoProcessCallback : public BnInterface<ICacaoProcessCallback> {
public:
    virtual status_t onTransact(uint32_t code, const Parcel& data,
                                Parcel* reply, uint32_t flags) override;
};

} // namespace android