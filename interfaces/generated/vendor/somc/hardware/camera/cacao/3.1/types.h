#ifndef HIDL_GENERATED_VENDOR_SOMC_HARDWARE_CAMERA_CACAO_V3_1_TYPES_H
#define HIDL_GENERATED_VENDOR_SOMC_HARDWARE_CAMERA_CACAO_V3_1_TYPES_H

#include <vendor/somc/hardware/camera/cacao/3.0/types.h>

#include <hidl/HidlSupport.h>
#include <hidl/MQDescriptor.h>
#include <utils/NativeHandle.h>
#include <utils/misc.h>

namespace vendor {
namespace somc {
namespace hardware {
namespace camera {
namespace cacao {
namespace V3_1 {

// Forward declaration for forward reference support:
struct CacaoCaps;

/*
 * [確認] getCapsV3_1() 回傳的相機能力結構。反編譯
 * V3_1::writeEmbeddedToParcel(CacaoCaps const&, ...) 確認：先呼叫 base
 * V3_0::CacaoCaps 的 writer（offset 0，代表繼承關係），接著在絕對 offset
 * +0xD0 開始寫兩個擴充欄位。一開始手動算 PLT relocation index 對到
 * _hidl_setColorSpaceForHandle，是算錯的；改用 Ghidra 的
 * get_function_callees 讓工具自己解析 ARM/Thumb interworking veneer +
 * PLT 間接跳轉後，確認 +0xD0 和 +0xF8 兩次呼叫（間隔 0x28=40 bytes）
 * 都是呼叫 V3_0::writeEmbeddedToParcel(SupportedInfo const&, ...)——
 * 也就是說 V3.1 的擴充欄位就是兩個 V3_0::SupportedInfo 實例，不是先前
 * 猜測的 sizes4/formats4/sizes5/formats5 四個獨立 hidl_vec。
 * （HIDL struct 不支援 extends，用組合表示 base 欄位，wire 佈局等價。）
 *
 * 2026-08-18 修正：base（sizeof=0xC8，見 3.0/types.hal）結束於 +0xC8，
 * 但 ext0 從 +0xD0 開始，中間有 8 bytes 填充，先前漏掉了。
 * 直接證據：3.1 blob 的 BpHwCacao::_hidl_getCapsV3_1 裡 readBuffer 立即數
 * 是 0x120=288，而 0xF8(ext1) + 0x28(SupportedInfo) = 0x120 剛好吻合。
 */
struct CacaoCaps final {
    ::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps base __attribute__ ((aligned(8)));
    ::android::hardware::hidl_array<uint32_t, 2> reserved0 __attribute__ ((aligned(4)));
    ::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo ext0 __attribute__ ((aligned(8)));
    ::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo ext1 __attribute__ ((aligned(8)));
};

static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps, base) == 0, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps, reserved0) == 200, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps, ext0) == 208, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps, ext1) == 248, "wrong offset");
static_assert(sizeof(::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps) == 288, "wrong size");
static_assert(__alignof(::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps) == 8, "wrong alignment");

//
// type declarations for package
//

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps& o);
static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps& o, ::std::ostream*);
static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps& rhs);
static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps& rhs);

//
// type header definitions for package
//

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".base = ";
    os += ::vendor::somc::hardware::camera::cacao::V3_0::toString(o.base);
    os += ", .reserved0 = ";
    os += ::android::hardware::toString(o.reserved0);
    os += ", .ext0 = ";
    os += ::vendor::somc::hardware::camera::cacao::V3_0::toString(o.ext0);
    os += ", .ext1 = ";
    os += ::vendor::somc::hardware::camera::cacao::V3_0::toString(o.ext1);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps& rhs) {
    if (lhs.base != rhs.base) {
        return false;
    }
    if (lhs.reserved0 != rhs.reserved0) {
        return false;
    }
    if (lhs.ext0 != rhs.ext0) {
        return false;
    }
    if (lhs.ext1 != rhs.ext1) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_1::CacaoCaps& rhs){
    return !(lhs == rhs);
}


}  // namespace V3_1
}  // namespace cacao
}  // namespace camera
}  // namespace hardware
}  // namespace somc
}  // namespace vendor

//
// global type declarations for package
//


#endif  // HIDL_GENERATED_VENDOR_SOMC_HARDWARE_CAMERA_CACAO_V3_1_TYPES_H
