// IVendorSomcCameraProvider.h — provider@1.0 stub
// 真正繼承自 android::hardware::camera::provider::V2_4::ICameraProvider
// （AOSP 標準介面，非先前猜測的 7 個 _pad_* 占位方法）
//
// 2026-08-17 用 Ghidra headless（8093, so_32/vendor.somc.hardware.camera.provider@1.0.so）
// 反編譯 BnHwVendorSomcCameraProvider::onTransact 確認完整 transaction code：
//   1-6  繼承自 ICameraProvider@2.4（setCallback, getVendorTags, getCameraIdList,
//        isSetTorchModeSupported, getCameraDeviceInterface_V1_x, _V3_x）
//   7    getCameraExtensionDeviceInterface_V1_0（自己的擴充，注意排在 Cacao 系列之前）
//   8    getCacaoInterface_V1_0
//   9    getCacaoInterface_V2_0
//   10   getCacaoInterface_V3_0
// dynsym 確認自己只新增 4 個方法，而非先前猜測的 3 個。

#pragma once
#include <hidl/HidlSupport.h>
#include <android/hidl/base/1.0/IBase.h>
#include <android/hidl/manager/1.0/IServiceNotification.h>
#include <android/hardware/camera/common/1.0/types.h>
#include <android/hardware/camera/provider/2.4/ICameraProvider.h>
#include <vendor/somc/hardware/camera/cacao/3.0/ICacao.h>
#include <vendor/somc/hardware/camera/cacao/3.0/types.h>

namespace vendor {
namespace somc {
namespace hardware {
namespace camera {

/* cacao V1_0/V2_0::ICacao 與 device V1_0::ICameraExtensionDevice 尚未在本專案
 * 反建構出完整介面（只有 V3_0/V3_1 的 ICacao 被 libcacao_service 實際使用）。
 * 這裡只 forward-declare，讓 getCacaoInterface_V1_0/V2_0/getCameraExtensionDeviceInterface_V1_0
 * 三個方法能佔住正確的 vtable slot（ABI 排列正確），呼叫端目前也只用得到
 * getCacaoInterface_V3_0，不需要這三個型別的完整定義。 */
namespace cacao {
namespace V1_0 { class ICacao; }
namespace V2_0 { class ICacao; }
} // namespace cacao
namespace device {
namespace V1_0 { class ICameraExtensionDevice; }
} // namespace device

namespace provider {
namespace V1_0 {

using android::sp;
using android::hardware::hidl_string;
using android::hardware::Return;
using ::android::hardware::camera::common::V1_0::Status;

class IVendorSomcCameraProvider
    : public ::android::hardware::camera::provider::V2_4::ICameraProvider {
public:
    virtual ~IVendorSomcCameraProvider() = default;

    static sp<IVendorSomcCameraProvider>
        getService(const std::string& name = "internal/0", bool getStub = false);

    /* 註冊服務通知（由 HIDL 生成碼提供，在 vendor .so 中匯出） */
    static bool registerForNotifications(
        const std::string& serviceName,
        const sp<::android::hidl::manager::V1_0::IServiceNotification>& notification);

    /* code 7 — 排在 Cacao 系列之前（Ghidra 確認的實際順序） */
    virtual Return<void> getCameraExtensionDeviceInterface_V1_0(
        const hidl_string& cameraDeviceName,
        std::function<void(Status status,
            const sp<device::V1_0::ICameraExtensionDevice>& device)> cb) = 0;

    /* code 8 */
    virtual Return<void> getCacaoInterface_V1_0(
        std::function<void(Status status,
            const sp<cacao::V1_0::ICacao>& service)> cb) = 0;

    /* code 9 */
    virtual Return<void> getCacaoInterface_V2_0(
        std::function<void(Status status,
            const sp<cacao::V2_0::ICacao>& service)> cb) = 0;

    /* code 10 */
    virtual Return<void> getCacaoInterface_V3_0(
        std::function<void(Status status,
            const sp<cacao::V3_0::ICacao>& service)> cb) = 0;
};

} // namespace V1_0
} // namespace provider
} // namespace camera
} // namespace hardware
} // namespace somc
} // namespace vendor
