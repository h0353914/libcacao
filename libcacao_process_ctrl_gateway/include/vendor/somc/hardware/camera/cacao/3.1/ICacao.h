// ICacao.h — vendor.somc.hardware.camera.cacao@3.1 主介面 stub
// V3.1 繼承 V3.0，從 onInit 中的 castFrom 呼叫推斷

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

    /* V3.1 新增方法 vtable[0x58]: 未知，占 slot */
    virtual Return<V3_0::ErrCode> _pad_v31_method() = 0;

    /* V3.1 getCaps vtable[0x5c]: 回傳 V3.1 CacaoCaps */
    virtual Return<void> getCaps(V3_0::CameraId camId,
        std::function<void(const CacaoCaps&, V3_0::ErrCode)> cb) = 0;
};

} // namespace V3_1
} // namespace cacao
} // namespace camera
} // namespace hardware
} // namespace somc
} // namespace vendor
