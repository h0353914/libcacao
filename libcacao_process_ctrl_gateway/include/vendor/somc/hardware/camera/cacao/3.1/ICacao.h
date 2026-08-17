// ICacao.h — vendor.somc.hardware.camera.cacao@3.1 主介面 stub
// V3.1 繼承 V3.0，從 onInit 中的 castFrom 呼叫推斷
//
// 2026-08-17 用 Ghidra headless（8092, so_32/vendor.somc.hardware.camera.cacao@3.1.so）
// 反編譯 V3_1::BnHwCacao::onTransact 確認：code 1-10 直接呼叫 V3_0::BnHwCacao 的
// _hidl_* helper（等同沿用 V3.0 全部 10 個方法，順序不變），V3.1 只在 code 11
// 新增唯一一個方法 getCapsV3_1。先前猜測「多一個 padding 方法」是錯的。

#pragma once
#include <vendor/somc/hardware/camera/cacao/3.0/ICacao.h>

namespace vendor {
namespace somc {
namespace hardware {
namespace camera {
namespace cacao {
namespace V3_1 {

using android::sp;
using android::hardware::Return;

/* V3.1 CacaoCaps — 比 V3.0 多出 D8~11F 的擴充欄位
 * 從 V3_1::CacaoCaps 拷貝建構子反編譯取得 */
struct CacaoCaps : public V3_0::CacaoCaps {
    uint32_t _pad_d4;                                                    // +0xd4
    android::hardware::hidl_vec<V3_0::ImageSize> sizes4;                 // +0xd8
    android::hardware::hidl_vec<uint32_t>  formats4;                     // +0xe8
    uint32_t field_f8;                                                   // +0xf8
    uint32_t _pad_fc;                                                    // +0xfc
    android::hardware::hidl_vec<V3_0::ImageSize> sizes5;                 // +0x100
    android::hardware::hidl_vec<uint32_t>  formats5;                     // +0x110
};

class ICacao : public V3_0::ICacao {
public:
    virtual ~ICacao() = default;

    /* V3.1 castFrom（stub，實際由 vendor.somc.hardware.camera.cacao@3.1.so 實作） */
    static sp<ICacao> castFrom(const sp<V3_0::ICacao>& service, bool pure = false);

    /* code 11 — V3.1 唯一新增方法，回傳 V3.1 CacaoCaps（Ghidra 確認方法名為
     * getCapsV3_1，不是覆寫 V3.0 的 getCaps） */
    virtual Return<void> getCapsV3_1(V3_0::CameraIndex camIdx,
        std::function<void(const CacaoCaps&, V3_0::ErrCode)> cb) = 0;
};

} // namespace V3_1
} // namespace cacao
} // namespace camera
} // namespace hardware
} // namespace somc
} // namespace vendor
