// ICacao.h — vendor.somc.hardware.camera.cacao@3.1 主介面 stub
// V3.1 繼承 V3.0，從 onInit 中的 castFrom 呼叫推斷
//
// 2026-08-17 用 Ghidra headless（8092, so_32/vendor.somc.hardware.camera.cacao@3.1.so）
// 反編譯 V3_1::BnHwCacao::onTransact 確認：code 1-10 直接呼叫 V3_0::BnHwCacao 的
// _hidl_* helper（等同沿用 V3.0 全部 10 個方法，順序不變），V3.1 只在 code 11
// 新增唯一一個方法 getCapsV3_1。先前猜測「多一個 padding 方法」是錯的。
//
// 2026-08-18 反編譯 V3_1::writeEmbeddedToParcel(CacaoCaps const&, ...) 確認：
// 呼叫 base V3_0::CacaoCaps 的 writer 後，緊接著在絕對 offset +0xD0 開始寫
// 自己的擴充欄位（回推 sizeof(V3_0::CacaoCaps)=0xD0，見 3.0/types.h）。
// 一開始手動算 PLT relocation index 算錯（誤以為呼叫端是
// _hidl_setColorSpaceForHandle），改用 Ghidra 的 get_function_callees
// 讓工具自己解析 ARM/Thumb interworking veneer + PLT 間接跳轉後，
// 確認 +0xD0 和 +0xF8 兩次呼叫（間隔 0x28=40 bytes）都是呼叫
// V3_0::writeEmbeddedToParcel(SupportedInfo const&, ...)——
// 也就是說 V3.1 的擴充欄位就是兩個 V3_0::SupportedInfo 實例，
// 不是先前猜測的 sizes4/formats4/sizes5/formats5 四個獨立 hidl_vec。

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

/* V3.1 CacaoCaps — 組合 V3_0::CacaoCaps（sizeof=0xD0），自己在 +0xD0/+0xF8
 * 各加一個 V3_0::SupportedInfo（Ghidra get_function_callees 確認呼叫端）。
 * hidl-gen 產生的 V3_0::CacaoCaps 是 final（HIDL struct 本來就不支援真正
 * 的繼承），所以這裡改用組合而非 C++ inheritance，wire layout 相同。 */
struct CacaoCaps {
    V3_0::CacaoCaps base;                                                // +0x00 (0xd0)
    V3_0::SupportedInfo ext0;                                            // +0xd0 (0x28)
    V3_0::SupportedInfo ext1;                                            // +0xf8 (0x28)
};                                                                        // sizeof = 0x120 (288)

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
