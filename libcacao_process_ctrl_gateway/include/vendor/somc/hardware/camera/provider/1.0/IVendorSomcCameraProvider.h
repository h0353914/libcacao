// IVendorSomcCameraProvider.h — provider@1.0 stub
// 繼承自 ICameraProvider@2.4（繼承自 IBase）
// vtable layout 必須精確匹配實機上的 vendor.somc.hardware.camera.provider@1.0.so

#pragma once
#include <hidl/HidlSupport.h>
#include <android/hidl/base/1.0/IBase.h>
#include <android/hidl/manager/1.0/IServiceNotification.h>
#include <android/hardware/camera/common/1.0/types.h>
#include <vendor/somc/hardware/camera/cacao/3.0/ICacao.h>
#include <vendor/somc/hardware/camera/cacao/3.0/types.h>

namespace vendor {
namespace somc {
namespace hardware {
namespace camera {
namespace provider {
namespace V1_0 {

using android::sp;
using android::hardware::hidl_string;
using android::hardware::Return;
using ::android::hardware::camera::common::V1_0::Status;

class IVendorSomcCameraProvider
    : public ::android::hidl::base::V1_0::IBase {
public:
    virtual ~IVendorSomcCameraProvider() = default;

    static sp<IVendorSomcCameraProvider>
        getService(const std::string& name = "internal/0", bool getStub = false);

    /* 註冊服務通知（由 HIDL 生成碼提供，在 vendor .so 中匯出） */
    static bool registerForNotifications(
        const std::string& serviceName,
        const sp<::android::hidl::manager::V1_0::IServiceNotification>& notification);

    /* ICameraProvider@2.4 的 7 個虛函式（佔據正確 vtable slot） */
    virtual Return<void> _pad_setCallback(const sp<::android::hidl::base::V1_0::IBase>&) = 0;
    virtual Return<void> _pad_getVendorTags(std::function<void()>) = 0;
    virtual Return<void> _pad_getCameraIdList(std::function<void()>) = 0;
    virtual Return<void> _pad_isSetTorchModeSupported(std::function<void()>) = 0;
    virtual Return<void> _pad_getCameraDeviceInterface_V1_x(const hidl_string&, std::function<void()>) = 0;
    virtual Return<void> _pad_getCameraDeviceInterface_V3_x(const hidl_string&, std::function<void()>) = 0;
    virtual Return<void> _pad_getCameraExtensionDeviceInterface_V1_0(const hidl_string&, std::function<void()>) = 0;

    /* IVendorSomcCameraProvider 自身的 3 個虛函式 */
    virtual Return<void> getCacaoInterface_V1_0(std::function<void()>) = 0;
    virtual Return<void> getCacaoInterface_V2_0(std::function<void()>) = 0;

    /* vtable offset 0x58：getCacaoInterface_V3_0
     * callback: (Status, sp<ICacao> const&) — Status 是 camera.common::V1_0::Status */
    virtual Return<void>
        getCacaoInterface_V3_0(std::function<void(
            Status status,
            const sp<cacao::V3_0::ICacao>& service)> cb) = 0;
};

} // namespace V1_0
} // namespace provider
} // namespace camera
} // namespace hardware
} // namespace somc
} // namespace vendor
