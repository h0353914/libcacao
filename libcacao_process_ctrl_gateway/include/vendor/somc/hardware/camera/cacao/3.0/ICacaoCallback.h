// ICacaoCallback.h — vendor.somc.hardware.camera.cacao@3.0 callback 介面 stub
// 依照 Ghidra 反編譯 ProcessCtrlGateway::Callback 類別重建
// 實際 BnHw/BpHw 由 vendor.somc.hardware.camera.cacao@3.0.so 提供

#pragma once
#include <hidl/HidlSupport.h>
#include <hidl/MQDescriptor.h>
#include <android/hidl/base/1.0/IBase.h>
#include <vendor/somc/hardware/camera/cacao/3.0/types.h>

namespace vendor {
namespace somc {
namespace hardware {
namespace camera {
namespace cacao {
namespace V3_0 {

/* ICacaoCallback（2026-08-17 用 Ghidra headless 反編譯 BnHwCacaoCallback::onTransact
 * 直接確認的 transaction code，取代先前反了的猜測）：
 *   code 1 = handleEvent    → _hidl_handleEvent
 *   code 2 = handleProgress → _hidl_handleProgress
 *   code 3 = handleResult   → _hidl_handleResult
 *
 * 訊息碼：
 *   handleEvent   = PAL_MSG_SUPER_SLOW 0x01000009
 *   handleProgress = PAL_MSG_PROGRESS  0x01000007
 *   handleResult  = PAL_MSG_RESULT     0x01000008
 *
 * 注意：toBinder<ICacaoCallback>() 需要 interfaceDescriptor() 回傳正確字串，
 * 否則 HIDL 框架會用 BnHwBase 包裝（而非 BnHwCacaoCallback），導致 UNKNOWN_TRANSACTION。
 */
class ICacaoCallback : public ::android::hidl::base::V1_0::IBase {
public:
    static constexpr const char* descriptor =
        "vendor.somc.hardware.camera.cacao@3.0::ICacaoCallback";

    virtual ~ICacaoCallback() = default;

    /* 覆蓋 IBase::interfaceDescriptor()，讓 HIDL 框架能建立正確的 BnHwCacaoCallback */
    ::android::hardware::Return<void> interfaceDescriptor(
        std::function<void(const ::android::hardware::hidl_string&)> _hidl_cb) override {
        _hidl_cb(descriptor);
        return ::android::hardware::Void();
    }

    /* 覆蓋 IBase::interfaceChain()，讓 HIDL 框架識別繼承鏈 */
    ::android::hardware::Return<void> interfaceChain(
        std::function<void(const ::android::hardware::hidl_vec<::android::hardware::hidl_string>&)> _hidl_cb) override {
        ::android::hardware::hidl_vec<::android::hardware::hidl_string> chain = {
            descriptor,
            ::android::hidl::base::V1_0::IBase::descriptor
        };
        _hidl_cb(chain);
        return ::android::hardware::Void();
    }

    /* HIDL 業務方法（宣告順序必須與 .hal 檔案一致，以確保 vtable slot 正確）
     * .hal 順序：handleEvent, handleProgress, handleResult */
    virtual ::android::hardware::Return<void>
        handleEvent(const Event& ev) = 0;

    virtual ::android::hardware::Return<void>
        handleProgress(const Result& res) = 0;

    virtual ::android::hardware::Return<void>
        handleResult(const Result& res) = 0;
};

} // namespace V3_0
} // namespace cacao
} // namespace camera
} // namespace hardware
} // namespace somc
} // namespace vendor
