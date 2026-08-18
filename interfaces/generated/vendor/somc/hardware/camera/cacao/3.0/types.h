#ifndef HIDL_GENERATED_VENDOR_SOMC_HARDWARE_CAMERA_CACAO_V3_0_TYPES_H
#define HIDL_GENERATED_VENDOR_SOMC_HARDWARE_CAMERA_CACAO_V3_0_TYPES_H

#include <hidl/HidlSupport.h>
#include <hidl/MQDescriptor.h>
#include <utils/NativeHandle.h>
#include <utils/misc.h>

namespace vendor {
namespace somc {
namespace hardware {
namespace camera {
namespace cacao {
namespace V3_0 {

// Forward declaration for forward reference support:
enum class ErrCode : int32_t;
enum class CameraIndex : int32_t;
enum class ConfigId : int32_t;
enum class DataSpace : int32_t;
struct ImageSize;
struct Config;
struct ControlMode;
struct ImageBufInfo;
struct Result;
struct Event;
struct Param;
struct SupportedInfo;
struct SuperSlowCaps;
struct VideoStabilizationCaps;
struct CacaoCaps;

/*
 * [確認] 從 callback 分支邏輯觀察到的用法；OK=0 已核對，其餘值未窮舉，
 * 但列舉底層一律是 int32_t，未列出的實際值一樣能正確序列化。
 */
enum class ErrCode : int32_t {
    OK = 0,
    ERR_GENERAL = -1 /* -1 */,
};

/*
 * [確認] getCaps()/getCapsV3_1() 的相機索引參數，dynsym 確認真實名稱
 * 是 CameraIndex（不是先前猜的 CameraId）。
 */
enum class CameraIndex : int32_t {
    CAMERA_ID_0 = 0,
    CAMERA_ID_1 = 1,
};

/*
 * [待查] getConfig()/setConfig() 的設定 ID，dynsym 確認存在、
 * ProcessCtrlGateway.cpp 只實際用到 0x65 這個值，完整列舉未知。
 */
enum class ConfigId : int32_t {
    UNKNOWN = 0,
};

/*
 * [待查] setColorSpaceForHandle() 的參數，dynsym 確認存在，完整列舉未知，
 * 很可能對應 android.hardware.graphics.common 的 Dataspace，待驗證。
 */
enum class DataSpace : int32_t {
    UNKNOWN = 0,
};

/*
 * [確認] hidl_vec<ImageSize> 在 CacaoCaps 系列結構中反覆出現。
 */
struct ImageSize final {
    uint32_t width __attribute__ ((aligned(4)));
    uint32_t height __attribute__ ((aligned(4)));
};

static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ImageSize, width) == 0, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ImageSize, height) == 4, "wrong offset");
static_assert(sizeof(::vendor::somc::hardware::camera::cacao::V3_0::ImageSize) == 8, "wrong size");
static_assert(__alignof(::vendor::somc::hardware::camera::cacao::V3_0::ImageSize) == 4, "wrong alignment");

/*
 * [確認] setConfig()/getConfig() 的資料結構。反編譯 BpHwCacao::_hidl_setConfig
 * 的 writeBuffer(parcel, &config, 8, …) 與 _hidl_getConfig 的
 * readBuffer(parcel, 8, …) 確認整個結構固定 8 bytes，兩個 uint32_t，
 * 不是先前猜測的「configId + 256-byte data」。第二欄語意對照
 * ProcessCtrlGateway::setConfig 送出的 {configId=0, value=dpType}
 * （Climax 慢動作參數）用法。
 */
struct Config final {
    uint32_t configId __attribute__ ((aligned(4)));
    uint32_t value __attribute__ ((aligned(4)));
};

static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::Config, configId) == 0, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::Config, value) == 4, "wrong offset");
static_assert(sizeof(::vendor::somc::hardware::camera::cacao::V3_0::Config) == 8, "wrong size");
static_assert(__alignof(::vendor::somc::hardware::camera::cacao::V3_0::Config) == 4, "wrong alignment");

/*
 * [確認] start() 的參數結構。反組譯 onStart 確認 9 個 uint32_t（36 bytes），
 * 對應 ProcessCtrlMode 的 field_08..field_28；後兩個欄位語意未知。
 */
struct ControlMode final {
    uint32_t modeType __attribute__ ((aligned(4)));
    int32_t camIdx __attribute__ ((aligned(4)));
    uint32_t maxStreamWidth __attribute__ ((aligned(4)));
    uint32_t maxStreamHeight __attribute__ ((aligned(4)));
    uint32_t captureWidth __attribute__ ((aligned(4)));
    uint32_t captureHeight __attribute__ ((aligned(4)));
    uint32_t maxPrdCaptureNum __attribute__ ((aligned(4)));
    uint32_t reserved0 __attribute__ ((aligned(4)));
    uint32_t reserved1 __attribute__ ((aligned(4)));
};

static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ControlMode, modeType) == 0, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ControlMode, camIdx) == 4, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ControlMode, maxStreamWidth) == 8, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ControlMode, maxStreamHeight) == 12, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ControlMode, captureWidth) == 16, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ControlMode, captureHeight) == 20, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ControlMode, maxPrdCaptureNum) == 24, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ControlMode, reserved0) == 28, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ControlMode, reserved1) == 32, "wrong offset");
static_assert(sizeof(::vendor::somc::hardware::camera::cacao::V3_0::ControlMode) == 36, "wrong size");
static_assert(__alignof(::vendor::somc::hardware::camera::cacao::V3_0::ControlMode) == 4, "wrong alignment");

/*
 * [確認] process()/processAsync() 的 Param 裡 hidl_vec<ImageBufInfo> 的元素
 * 型別。反編譯 writeEmbeddedToParcel(ImageBufInfo) 確認 hidl_handle 在 +8，
 * 與既有 ProcessCtrlGateway.cpp 的 REF asm 註解（str w→+0, str h→+4,
 * handle.setTo→+8, colorFmt→+0x18）完全吻合。
 */
struct ImageBufInfo final {
    uint32_t width __attribute__ ((aligned(4)));
    uint32_t height __attribute__ ((aligned(4)));
    ::android::hardware::hidl_handle imgHandle __attribute__ ((aligned(8)));
    uint32_t colorFmt __attribute__ ((aligned(4)));
    uint32_t reserved0 __attribute__ ((aligned(4)));
};

static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ImageBufInfo, width) == 0, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ImageBufInfo, height) == 4, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ImageBufInfo, imgHandle) == 8, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ImageBufInfo, colorFmt) == 24, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::ImageBufInfo, reserved0) == 28, "wrong offset");
static_assert(sizeof(::vendor::somc::hardware::camera::cacao::V3_0::ImageBufInfo) == 32, "wrong size");
static_assert(__alignof(::vendor::somc::hardware::camera::cacao::V3_0::ImageBufInfo) == 8, "wrong alignment");

/*
 * [確認] ICacaoCallback::handleProgress/handleResult 的資料結構。
 * 從 memcpy(dst, src, 0x48) 確認大小固定 72 bytes；反編譯
 * writeEmbeddedToParcel 系列函式沒有任何一個是拿 Result 當參數，
 * 代表整塊都是純量欄位（無內嵌 hidl_vec/hidl_handle/hidl_string），
 * 用不透明 byte array 表示對 wire 格式安全。
 */
struct Result final {
    ::android::hardware::hidl_array<uint8_t, 72> data __attribute__ ((aligned(1)));
};

static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::Result, data) == 0, "wrong offset");
static_assert(sizeof(::vendor::somc::hardware::camera::cacao::V3_0::Result) == 72, "wrong size");
static_assert(__alignof(::vendor::somc::hardware::camera::cacao::V3_0::Result) == 1, "wrong alignment");

/*
 * [確認] ICacaoCallback::handleEvent 的資料結構，大小約 16 bytes。
 */
struct Event final {
    uint32_t eventType __attribute__ ((aligned(4)));
    ::android::hardware::hidl_array<uint32_t, 3> reserved __attribute__ ((aligned(4)));
};

static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::Event, eventType) == 0, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::Event, reserved) == 4, "wrong offset");
static_assert(sizeof(::vendor::somc::hardware::camera::cacao::V3_0::Event) == 16, "wrong size");
static_assert(__alignof(::vendor::somc::hardware::camera::cacao::V3_0::Event) == 4, "wrong alignment");

/*
 * [確認] process()/processAsync() 的參數結構。反編譯
 * writeEmbeddedToParcel(Param const&, ...) 確認四個內嵌欄位：
 * hidl_string@+0x30、hidl_handle@+0x78、hidl_handle@+0x98、
 * hidl_vec<ImageBufInfo>@+0xb8（element stride ×32，對上已確認的
 * sizeof(ImageBufInfo)=32；函式內對這個 vec 的逐一走訪還直接印證了
 * ImageBufInfo.imgHandle 真的在元素 +8）。
 *
 * 雙重驗證：ProcessCtrlGateway.cpp 裡 ResultItem 建構子/解構子的
 * placement-new offset（this+8 為內嵌 Param，+0x38/+0x80/+0xa0/+0xc0
 * 相對 Param 起點正好是 +0x30/+0x78/+0x98/+0xb8）與 processAsyncInternal()
 * 的既有詳細註解都跟這裡的反編譯結果完全吻合。
 * 總大小 0xC8 (200 bytes)，與舊筆記「Ghidra memclr(param_3, 200)」一致。
 */
struct Param final {
    uint32_t processType __attribute__ ((aligned(4)));
    ::android::hardware::hidl_array<uint32_t, 11> reserved0 __attribute__ ((aligned(4)));
    ::android::hardware::hidl_string name __attribute__ ((aligned(8)));
    ::android::hardware::hidl_array<uint32_t, 14> reserved1 __attribute__ ((aligned(4)));
    ::android::hardware::hidl_handle handle0 __attribute__ ((aligned(8)));
    ::android::hardware::hidl_array<uint32_t, 4> reserved2 __attribute__ ((aligned(4)));
    ::android::hardware::hidl_handle handle1 __attribute__ ((aligned(8)));
    ::android::hardware::hidl_array<uint32_t, 3> reserved3 __attribute__ ((aligned(4)));
    uint32_t bufCount __attribute__ ((aligned(4)));
    ::android::hardware::hidl_vec<::vendor::somc::hardware::camera::cacao::V3_0::ImageBufInfo> bufInfos __attribute__ ((aligned(8)));
};

static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::Param, processType) == 0, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::Param, reserved0) == 4, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::Param, name) == 48, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::Param, reserved1) == 64, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::Param, handle0) == 120, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::Param, reserved2) == 136, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::Param, handle1) == 152, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::Param, reserved3) == 168, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::Param, bufCount) == 180, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::Param, bufInfos) == 184, "wrong offset");
static_assert(sizeof(::vendor::somc::hardware::camera::cacao::V3_0::Param) == 200, "wrong size");
static_assert(__alignof(::vendor::somc::hardware::camera::cacao::V3_0::Param) == 8, "wrong alignment");

/*
 * [確認] CacaoCaps 巢狀子結構之一。反編譯
 * writeEmbeddedToParcel(SupportedInfo const&, ...) 確認兩個 hidl_vec 欄位。
 * VideoHFRCaps 的 writeEmbeddedToParcel 是完全相同的函式位址，
 * 應是對同一結構的 typedef。
 */
struct SupportedInfo final {
    ::android::hardware::hidl_array<uint32_t, 2> reserved0 __attribute__ ((aligned(4)));
    ::android::hardware::hidl_vec<::vendor::somc::hardware::camera::cacao::V3_0::ImageSize> sizes __attribute__ ((aligned(8)));
    ::android::hardware::hidl_vec<uint32_t> formats __attribute__ ((aligned(8)));
};

static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo, reserved0) == 0, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo, sizes) == 8, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo, formats) == 24, "wrong offset");
static_assert(sizeof(::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo) == 40, "wrong size");
static_assert(__alignof(::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo) == 8, "wrong alignment");

typedef ::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo VideoHFRCaps;

/*
 * [確認] CacaoCaps 巢狀子結構之二。反編譯
 * writeEmbeddedToParcel(SuperSlowCaps const&, ...) 確認三個 hidl_vec 到
 * +0x40 為止；尾端 4 個保留字（+0x40~+0x4c）是從 V3_1::CacaoCaps 的擴充
 * 欄位絕對 offset (+0xD0) 交叉驗證確認的（回推 sizeof(V3_0::CacaoCaps)
 * =0xD0），不是估算。
 */
struct SuperSlowCaps final {
    ::android::hardware::hidl_array<uint32_t, 4> reserved0 __attribute__ ((aligned(4)));
    ::android::hardware::hidl_vec<::vendor::somc::hardware::camera::cacao::V3_0::ImageSize> sizes __attribute__ ((aligned(8)));
    ::android::hardware::hidl_vec<uint32_t> formats0 __attribute__ ((aligned(8)));
    ::android::hardware::hidl_vec<uint32_t> formats1 __attribute__ ((aligned(8)));
    ::android::hardware::hidl_array<uint32_t, 4> reserved1 __attribute__ ((aligned(4)));
};

static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps, reserved0) == 0, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps, sizes) == 16, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps, formats0) == 32, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps, formats1) == 48, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps, reserved1) == 64, "wrong offset");
static_assert(sizeof(::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps) == 80, "wrong size");
static_assert(__alignof(::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps) == 8, "wrong alignment");

/*
 * [確認] CacaoCaps 巢狀子結構之三。反編譯
 * writeEmbeddedToParcel(VideoStabilizationCaps const&, ...) 確認四個
 * hidl_vec 欄位；總大小 0x60 (96 bytes) 是從 CacaoCaps 內嵌它之後緊接著
 * 下一個欄位（sizes @+0x60）反推確認的，不是猜測。
 */
struct VideoStabilizationCaps final {
    ::android::hardware::hidl_array<uint32_t, 4> reserved0 __attribute__ ((aligned(4)));
    ::android::hardware::hidl_vec<::vendor::somc::hardware::camera::cacao::V3_0::ImageSize> sizes0 __attribute__ ((aligned(8)));
    ::android::hardware::hidl_vec<uint32_t> formats0 __attribute__ ((aligned(8)));
    ::android::hardware::hidl_array<uint32_t, 2> reserved1 __attribute__ ((aligned(4)));
    ::android::hardware::hidl_vec<::vendor::somc::hardware::camera::cacao::V3_0::ImageSize> sizes1 __attribute__ ((aligned(8)));
    ::android::hardware::hidl_vec<uint32_t> formats1 __attribute__ ((aligned(8)));
    ::android::hardware::hidl_array<uint32_t, 2> reserved2 __attribute__ ((aligned(4)));
};

static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps, reserved0) == 0, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps, sizes0) == 16, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps, formats0) == 32, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps, reserved1) == 48, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps, sizes1) == 56, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps, formats1) == 72, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps, reserved2) == 88, "wrong offset");
static_assert(sizeof(::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps) == 96, "wrong size");
static_assert(__alignof(::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps) == 8, "wrong alignment");

/*
 * [確認] getCaps() 回傳的相機能力結構。反編譯
 * writeEmbeddedToParcel(CacaoCaps const&, ...) 完全推翻先前「一堆
 * sizes0..formats3 扁平陣列」的猜測：CacaoCaps 其實是巢狀具名子結構——
 * 開頭直接內嵌 VideoStabilizationCaps（reinterpret_cast this 呼叫，
 * 代表它是 CacaoCaps 的第一個成員），接著兩個 hidl_vec，最後內嵌
 * SuperSlowCaps。sizeof=0xD0 (208 bytes) 是編譯器實際採用的值，
 * 用 V3_1::CacaoCaps 的擴充欄位絕對 offset 交叉驗證確認過。
 */
struct CacaoCaps final {
    ::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps videoStabilization __attribute__ ((aligned(8)));
    ::android::hardware::hidl_vec<::vendor::somc::hardware::camera::cacao::V3_0::ImageSize> sizes __attribute__ ((aligned(8)));
    ::android::hardware::hidl_vec<uint32_t> formats __attribute__ ((aligned(8)));
    ::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps superSlow __attribute__ ((aligned(8)));
};

static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps, videoStabilization) == 0, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps, sizes) == 96, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps, formats) == 112, "wrong offset");
static_assert(offsetof(::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps, superSlow) == 128, "wrong offset");
static_assert(sizeof(::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps) == 208, "wrong size");
static_assert(__alignof(::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps) == 8, "wrong alignment");

//
// type declarations for package
//

template<typename>
static inline std::string toString(int32_t o);
static inline std::string toString(::vendor::somc::hardware::camera::cacao::V3_0::ErrCode o);
static inline void PrintTo(::vendor::somc::hardware::camera::cacao::V3_0::ErrCode o, ::std::ostream* os);
constexpr int32_t operator|(const ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) | static_cast<int32_t>(rhs));
}
constexpr int32_t operator|(const int32_t lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode rhs) {
    return static_cast<int32_t>(lhs | static_cast<int32_t>(rhs));
}
constexpr int32_t operator|(const ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode lhs, const int32_t rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) | rhs);
}
constexpr int32_t operator&(const ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) & static_cast<int32_t>(rhs));
}
constexpr int32_t operator&(const int32_t lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode rhs) {
    return static_cast<int32_t>(lhs & static_cast<int32_t>(rhs));
}
constexpr int32_t operator&(const ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode lhs, const int32_t rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) & rhs);
}
constexpr int32_t &operator|=(int32_t& v, const ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode e) {
    v |= static_cast<int32_t>(e);
    return v;
}
constexpr int32_t &operator&=(int32_t& v, const ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode e) {
    v &= static_cast<int32_t>(e);
    return v;
}

template<typename>
static inline std::string toString(int32_t o);
static inline std::string toString(::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex o);
static inline void PrintTo(::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex o, ::std::ostream* os);
constexpr int32_t operator|(const ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) | static_cast<int32_t>(rhs));
}
constexpr int32_t operator|(const int32_t lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex rhs) {
    return static_cast<int32_t>(lhs | static_cast<int32_t>(rhs));
}
constexpr int32_t operator|(const ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex lhs, const int32_t rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) | rhs);
}
constexpr int32_t operator&(const ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) & static_cast<int32_t>(rhs));
}
constexpr int32_t operator&(const int32_t lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex rhs) {
    return static_cast<int32_t>(lhs & static_cast<int32_t>(rhs));
}
constexpr int32_t operator&(const ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex lhs, const int32_t rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) & rhs);
}
constexpr int32_t &operator|=(int32_t& v, const ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex e) {
    v |= static_cast<int32_t>(e);
    return v;
}
constexpr int32_t &operator&=(int32_t& v, const ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex e) {
    v &= static_cast<int32_t>(e);
    return v;
}

template<typename>
static inline std::string toString(int32_t o);
static inline std::string toString(::vendor::somc::hardware::camera::cacao::V3_0::ConfigId o);
static inline void PrintTo(::vendor::somc::hardware::camera::cacao::V3_0::ConfigId o, ::std::ostream* os);
constexpr int32_t operator|(const ::vendor::somc::hardware::camera::cacao::V3_0::ConfigId lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ConfigId rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) | static_cast<int32_t>(rhs));
}
constexpr int32_t operator|(const int32_t lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ConfigId rhs) {
    return static_cast<int32_t>(lhs | static_cast<int32_t>(rhs));
}
constexpr int32_t operator|(const ::vendor::somc::hardware::camera::cacao::V3_0::ConfigId lhs, const int32_t rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) | rhs);
}
constexpr int32_t operator&(const ::vendor::somc::hardware::camera::cacao::V3_0::ConfigId lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ConfigId rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) & static_cast<int32_t>(rhs));
}
constexpr int32_t operator&(const int32_t lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ConfigId rhs) {
    return static_cast<int32_t>(lhs & static_cast<int32_t>(rhs));
}
constexpr int32_t operator&(const ::vendor::somc::hardware::camera::cacao::V3_0::ConfigId lhs, const int32_t rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) & rhs);
}
constexpr int32_t &operator|=(int32_t& v, const ::vendor::somc::hardware::camera::cacao::V3_0::ConfigId e) {
    v |= static_cast<int32_t>(e);
    return v;
}
constexpr int32_t &operator&=(int32_t& v, const ::vendor::somc::hardware::camera::cacao::V3_0::ConfigId e) {
    v &= static_cast<int32_t>(e);
    return v;
}

template<typename>
static inline std::string toString(int32_t o);
static inline std::string toString(::vendor::somc::hardware::camera::cacao::V3_0::DataSpace o);
static inline void PrintTo(::vendor::somc::hardware::camera::cacao::V3_0::DataSpace o, ::std::ostream* os);
constexpr int32_t operator|(const ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) | static_cast<int32_t>(rhs));
}
constexpr int32_t operator|(const int32_t lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace rhs) {
    return static_cast<int32_t>(lhs | static_cast<int32_t>(rhs));
}
constexpr int32_t operator|(const ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace lhs, const int32_t rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) | rhs);
}
constexpr int32_t operator&(const ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) & static_cast<int32_t>(rhs));
}
constexpr int32_t operator&(const int32_t lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace rhs) {
    return static_cast<int32_t>(lhs & static_cast<int32_t>(rhs));
}
constexpr int32_t operator&(const ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace lhs, const int32_t rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) & rhs);
}
constexpr int32_t &operator|=(int32_t& v, const ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace e) {
    v |= static_cast<int32_t>(e);
    return v;
}
constexpr int32_t &operator&=(int32_t& v, const ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace e) {
    v &= static_cast<int32_t>(e);
    return v;
}

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::ImageSize& o);
static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::ImageSize& o, ::std::ostream*);
static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::ImageSize& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ImageSize& rhs);
static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::ImageSize& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ImageSize& rhs);

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::Config& o);
static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::Config& o, ::std::ostream*);
static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::Config& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::Config& rhs);
static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::Config& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::Config& rhs);

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::ControlMode& o);
static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::ControlMode& o, ::std::ostream*);
static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::ControlMode& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ControlMode& rhs);
static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::ControlMode& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ControlMode& rhs);

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::ImageBufInfo& o);
static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::ImageBufInfo& o, ::std::ostream*);
// operator== and operator!= are not generated for ImageBufInfo

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::Result& o);
static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::Result& o, ::std::ostream*);
static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::Result& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::Result& rhs);
static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::Result& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::Result& rhs);

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::Event& o);
static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::Event& o, ::std::ostream*);
static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::Event& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::Event& rhs);
static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::Event& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::Event& rhs);

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::Param& o);
static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::Param& o, ::std::ostream*);
// operator== and operator!= are not generated for Param

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo& o);
static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo& o, ::std::ostream*);
static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo& rhs);
static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo& rhs);

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps& o);
static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps& o, ::std::ostream*);
static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps& rhs);
static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps& rhs);

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps& o);
static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps& o, ::std::ostream*);
static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps& rhs);
static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps& rhs);

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps& o);
static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps& o, ::std::ostream*);
static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps& rhs);
static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps& rhs);

//
// type header definitions for package
//

template<>
inline std::string toString<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode>(int32_t o) {
    using ::android::hardware::details::toHexString;
    std::string os;
    ::android::hardware::hidl_bitfield<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> flipped = 0;
    bool first = true;
    if ((o & ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode::OK) == static_cast<int32_t>(::vendor::somc::hardware::camera::cacao::V3_0::ErrCode::OK)) {
        os += (first ? "" : " | ");
        os += "OK";
        first = false;
        flipped |= ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode::OK;
    }
    if ((o & ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode::ERR_GENERAL) == static_cast<int32_t>(::vendor::somc::hardware::camera::cacao::V3_0::ErrCode::ERR_GENERAL)) {
        os += (first ? "" : " | ");
        os += "ERR_GENERAL";
        first = false;
        flipped |= ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode::ERR_GENERAL;
    }
    if (o != flipped) {
        os += (first ? "" : " | ");
        os += toHexString(o & (~flipped));
    }os += " (";
    os += toHexString(o);
    os += ")";
    return os;
}

static inline std::string toString(::vendor::somc::hardware::camera::cacao::V3_0::ErrCode o) {
    using ::android::hardware::details::toHexString;
    if (o == ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode::OK) {
        return "OK";
    }
    if (o == ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode::ERR_GENERAL) {
        return "ERR_GENERAL";
    }
    std::string os;
    os += toHexString(static_cast<int32_t>(o));
    return os;
}

static inline void PrintTo(::vendor::somc::hardware::camera::cacao::V3_0::ErrCode o, ::std::ostream* os) {
    *os << toString(o);
}

template<>
inline std::string toString<::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex>(int32_t o) {
    using ::android::hardware::details::toHexString;
    std::string os;
    ::android::hardware::hidl_bitfield<::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex> flipped = 0;
    bool first = true;
    if ((o & ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex::CAMERA_ID_0) == static_cast<int32_t>(::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex::CAMERA_ID_0)) {
        os += (first ? "" : " | ");
        os += "CAMERA_ID_0";
        first = false;
        flipped |= ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex::CAMERA_ID_0;
    }
    if ((o & ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex::CAMERA_ID_1) == static_cast<int32_t>(::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex::CAMERA_ID_1)) {
        os += (first ? "" : " | ");
        os += "CAMERA_ID_1";
        first = false;
        flipped |= ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex::CAMERA_ID_1;
    }
    if (o != flipped) {
        os += (first ? "" : " | ");
        os += toHexString(o & (~flipped));
    }os += " (";
    os += toHexString(o);
    os += ")";
    return os;
}

static inline std::string toString(::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex o) {
    using ::android::hardware::details::toHexString;
    if (o == ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex::CAMERA_ID_0) {
        return "CAMERA_ID_0";
    }
    if (o == ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex::CAMERA_ID_1) {
        return "CAMERA_ID_1";
    }
    std::string os;
    os += toHexString(static_cast<int32_t>(o));
    return os;
}

static inline void PrintTo(::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex o, ::std::ostream* os) {
    *os << toString(o);
}

template<>
inline std::string toString<::vendor::somc::hardware::camera::cacao::V3_0::ConfigId>(int32_t o) {
    using ::android::hardware::details::toHexString;
    std::string os;
    ::android::hardware::hidl_bitfield<::vendor::somc::hardware::camera::cacao::V3_0::ConfigId> flipped = 0;
    bool first = true;
    if ((o & ::vendor::somc::hardware::camera::cacao::V3_0::ConfigId::UNKNOWN) == static_cast<int32_t>(::vendor::somc::hardware::camera::cacao::V3_0::ConfigId::UNKNOWN)) {
        os += (first ? "" : " | ");
        os += "UNKNOWN";
        first = false;
        flipped |= ::vendor::somc::hardware::camera::cacao::V3_0::ConfigId::UNKNOWN;
    }
    if (o != flipped) {
        os += (first ? "" : " | ");
        os += toHexString(o & (~flipped));
    }os += " (";
    os += toHexString(o);
    os += ")";
    return os;
}

static inline std::string toString(::vendor::somc::hardware::camera::cacao::V3_0::ConfigId o) {
    using ::android::hardware::details::toHexString;
    if (o == ::vendor::somc::hardware::camera::cacao::V3_0::ConfigId::UNKNOWN) {
        return "UNKNOWN";
    }
    std::string os;
    os += toHexString(static_cast<int32_t>(o));
    return os;
}

static inline void PrintTo(::vendor::somc::hardware::camera::cacao::V3_0::ConfigId o, ::std::ostream* os) {
    *os << toString(o);
}

template<>
inline std::string toString<::vendor::somc::hardware::camera::cacao::V3_0::DataSpace>(int32_t o) {
    using ::android::hardware::details::toHexString;
    std::string os;
    ::android::hardware::hidl_bitfield<::vendor::somc::hardware::camera::cacao::V3_0::DataSpace> flipped = 0;
    bool first = true;
    if ((o & ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace::UNKNOWN) == static_cast<int32_t>(::vendor::somc::hardware::camera::cacao::V3_0::DataSpace::UNKNOWN)) {
        os += (first ? "" : " | ");
        os += "UNKNOWN";
        first = false;
        flipped |= ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace::UNKNOWN;
    }
    if (o != flipped) {
        os += (first ? "" : " | ");
        os += toHexString(o & (~flipped));
    }os += " (";
    os += toHexString(o);
    os += ")";
    return os;
}

static inline std::string toString(::vendor::somc::hardware::camera::cacao::V3_0::DataSpace o) {
    using ::android::hardware::details::toHexString;
    if (o == ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace::UNKNOWN) {
        return "UNKNOWN";
    }
    std::string os;
    os += toHexString(static_cast<int32_t>(o));
    return os;
}

static inline void PrintTo(::vendor::somc::hardware::camera::cacao::V3_0::DataSpace o, ::std::ostream* os) {
    *os << toString(o);
}

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::ImageSize& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".width = ";
    os += ::android::hardware::toString(o.width);
    os += ", .height = ";
    os += ::android::hardware::toString(o.height);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::ImageSize& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::ImageSize& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ImageSize& rhs) {
    if (lhs.width != rhs.width) {
        return false;
    }
    if (lhs.height != rhs.height) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::ImageSize& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ImageSize& rhs){
    return !(lhs == rhs);
}

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::Config& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".configId = ";
    os += ::android::hardware::toString(o.configId);
    os += ", .value = ";
    os += ::android::hardware::toString(o.value);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::Config& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::Config& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::Config& rhs) {
    if (lhs.configId != rhs.configId) {
        return false;
    }
    if (lhs.value != rhs.value) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::Config& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::Config& rhs){
    return !(lhs == rhs);
}

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::ControlMode& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".modeType = ";
    os += ::android::hardware::toString(o.modeType);
    os += ", .camIdx = ";
    os += ::android::hardware::toString(o.camIdx);
    os += ", .maxStreamWidth = ";
    os += ::android::hardware::toString(o.maxStreamWidth);
    os += ", .maxStreamHeight = ";
    os += ::android::hardware::toString(o.maxStreamHeight);
    os += ", .captureWidth = ";
    os += ::android::hardware::toString(o.captureWidth);
    os += ", .captureHeight = ";
    os += ::android::hardware::toString(o.captureHeight);
    os += ", .maxPrdCaptureNum = ";
    os += ::android::hardware::toString(o.maxPrdCaptureNum);
    os += ", .reserved0 = ";
    os += ::android::hardware::toString(o.reserved0);
    os += ", .reserved1 = ";
    os += ::android::hardware::toString(o.reserved1);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::ControlMode& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::ControlMode& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ControlMode& rhs) {
    if (lhs.modeType != rhs.modeType) {
        return false;
    }
    if (lhs.camIdx != rhs.camIdx) {
        return false;
    }
    if (lhs.maxStreamWidth != rhs.maxStreamWidth) {
        return false;
    }
    if (lhs.maxStreamHeight != rhs.maxStreamHeight) {
        return false;
    }
    if (lhs.captureWidth != rhs.captureWidth) {
        return false;
    }
    if (lhs.captureHeight != rhs.captureHeight) {
        return false;
    }
    if (lhs.maxPrdCaptureNum != rhs.maxPrdCaptureNum) {
        return false;
    }
    if (lhs.reserved0 != rhs.reserved0) {
        return false;
    }
    if (lhs.reserved1 != rhs.reserved1) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::ControlMode& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::ControlMode& rhs){
    return !(lhs == rhs);
}

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::ImageBufInfo& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".width = ";
    os += ::android::hardware::toString(o.width);
    os += ", .height = ";
    os += ::android::hardware::toString(o.height);
    os += ", .imgHandle = ";
    os += ::android::hardware::toString(o.imgHandle);
    os += ", .colorFmt = ";
    os += ::android::hardware::toString(o.colorFmt);
    os += ", .reserved0 = ";
    os += ::android::hardware::toString(o.reserved0);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::ImageBufInfo& o, ::std::ostream* os) {
    *os << toString(o);
}

// operator== and operator!= are not generated for ImageBufInfo

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::Result& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".data = ";
    os += ::android::hardware::toString(o.data);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::Result& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::Result& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::Result& rhs) {
    if (lhs.data != rhs.data) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::Result& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::Result& rhs){
    return !(lhs == rhs);
}

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::Event& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".eventType = ";
    os += ::android::hardware::toString(o.eventType);
    os += ", .reserved = ";
    os += ::android::hardware::toString(o.reserved);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::Event& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::Event& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::Event& rhs) {
    if (lhs.eventType != rhs.eventType) {
        return false;
    }
    if (lhs.reserved != rhs.reserved) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::Event& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::Event& rhs){
    return !(lhs == rhs);
}

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::Param& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".processType = ";
    os += ::android::hardware::toString(o.processType);
    os += ", .reserved0 = ";
    os += ::android::hardware::toString(o.reserved0);
    os += ", .name = ";
    os += ::android::hardware::toString(o.name);
    os += ", .reserved1 = ";
    os += ::android::hardware::toString(o.reserved1);
    os += ", .handle0 = ";
    os += ::android::hardware::toString(o.handle0);
    os += ", .reserved2 = ";
    os += ::android::hardware::toString(o.reserved2);
    os += ", .handle1 = ";
    os += ::android::hardware::toString(o.handle1);
    os += ", .reserved3 = ";
    os += ::android::hardware::toString(o.reserved3);
    os += ", .bufCount = ";
    os += ::android::hardware::toString(o.bufCount);
    os += ", .bufInfos = ";
    os += ::android::hardware::toString(o.bufInfos);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::Param& o, ::std::ostream* os) {
    *os << toString(o);
}

// operator== and operator!= are not generated for Param

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".reserved0 = ";
    os += ::android::hardware::toString(o.reserved0);
    os += ", .sizes = ";
    os += ::android::hardware::toString(o.sizes);
    os += ", .formats = ";
    os += ::android::hardware::toString(o.formats);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo& rhs) {
    if (lhs.reserved0 != rhs.reserved0) {
        return false;
    }
    if (lhs.sizes != rhs.sizes) {
        return false;
    }
    if (lhs.formats != rhs.formats) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::SupportedInfo& rhs){
    return !(lhs == rhs);
}

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".reserved0 = ";
    os += ::android::hardware::toString(o.reserved0);
    os += ", .sizes = ";
    os += ::android::hardware::toString(o.sizes);
    os += ", .formats0 = ";
    os += ::android::hardware::toString(o.formats0);
    os += ", .formats1 = ";
    os += ::android::hardware::toString(o.formats1);
    os += ", .reserved1 = ";
    os += ::android::hardware::toString(o.reserved1);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps& rhs) {
    if (lhs.reserved0 != rhs.reserved0) {
        return false;
    }
    if (lhs.sizes != rhs.sizes) {
        return false;
    }
    if (lhs.formats0 != rhs.formats0) {
        return false;
    }
    if (lhs.formats1 != rhs.formats1) {
        return false;
    }
    if (lhs.reserved1 != rhs.reserved1) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::SuperSlowCaps& rhs){
    return !(lhs == rhs);
}

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".reserved0 = ";
    os += ::android::hardware::toString(o.reserved0);
    os += ", .sizes0 = ";
    os += ::android::hardware::toString(o.sizes0);
    os += ", .formats0 = ";
    os += ::android::hardware::toString(o.formats0);
    os += ", .reserved1 = ";
    os += ::android::hardware::toString(o.reserved1);
    os += ", .sizes1 = ";
    os += ::android::hardware::toString(o.sizes1);
    os += ", .formats1 = ";
    os += ::android::hardware::toString(o.formats1);
    os += ", .reserved2 = ";
    os += ::android::hardware::toString(o.reserved2);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps& rhs) {
    if (lhs.reserved0 != rhs.reserved0) {
        return false;
    }
    if (lhs.sizes0 != rhs.sizes0) {
        return false;
    }
    if (lhs.formats0 != rhs.formats0) {
        return false;
    }
    if (lhs.reserved1 != rhs.reserved1) {
        return false;
    }
    if (lhs.sizes1 != rhs.sizes1) {
        return false;
    }
    if (lhs.formats1 != rhs.formats1) {
        return false;
    }
    if (lhs.reserved2 != rhs.reserved2) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::VideoStabilizationCaps& rhs){
    return !(lhs == rhs);
}

static inline std::string toString(const ::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".videoStabilization = ";
    os += ::vendor::somc::hardware::camera::cacao::V3_0::toString(o.videoStabilization);
    os += ", .sizes = ";
    os += ::android::hardware::toString(o.sizes);
    os += ", .formats = ";
    os += ::android::hardware::toString(o.formats);
    os += ", .superSlow = ";
    os += ::vendor::somc::hardware::camera::cacao::V3_0::toString(o.superSlow);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps& rhs) {
    if (lhs.videoStabilization != rhs.videoStabilization) {
        return false;
    }
    if (lhs.sizes != rhs.sizes) {
        return false;
    }
    if (lhs.formats != rhs.formats) {
        return false;
    }
    if (lhs.superSlow != rhs.superSlow) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps& lhs, const ::vendor::somc::hardware::camera::cacao::V3_0::CacaoCaps& rhs){
    return !(lhs == rhs);
}


}  // namespace V3_0
}  // namespace cacao
}  // namespace camera
}  // namespace hardware
}  // namespace somc
}  // namespace vendor

//
// global type declarations for package
//

namespace android {
namespace hardware {
namespace details {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++17-extensions"
template<> inline constexpr std::array<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode, 2> hidl_enum_values<::vendor::somc::hardware::camera::cacao::V3_0::ErrCode> = {
    ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode::OK,
    ::vendor::somc::hardware::camera::cacao::V3_0::ErrCode::ERR_GENERAL,
};
#pragma clang diagnostic pop
}  // namespace details
}  // namespace hardware
}  // namespace android

namespace android {
namespace hardware {
namespace details {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++17-extensions"
template<> inline constexpr std::array<::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex, 2> hidl_enum_values<::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex> = {
    ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex::CAMERA_ID_0,
    ::vendor::somc::hardware::camera::cacao::V3_0::CameraIndex::CAMERA_ID_1,
};
#pragma clang diagnostic pop
}  // namespace details
}  // namespace hardware
}  // namespace android

namespace android {
namespace hardware {
namespace details {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++17-extensions"
template<> inline constexpr std::array<::vendor::somc::hardware::camera::cacao::V3_0::ConfigId, 1> hidl_enum_values<::vendor::somc::hardware::camera::cacao::V3_0::ConfigId> = {
    ::vendor::somc::hardware::camera::cacao::V3_0::ConfigId::UNKNOWN,
};
#pragma clang diagnostic pop
}  // namespace details
}  // namespace hardware
}  // namespace android

namespace android {
namespace hardware {
namespace details {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++17-extensions"
template<> inline constexpr std::array<::vendor::somc::hardware::camera::cacao::V3_0::DataSpace, 1> hidl_enum_values<::vendor::somc::hardware::camera::cacao::V3_0::DataSpace> = {
    ::vendor::somc::hardware::camera::cacao::V3_0::DataSpace::UNKNOWN,
};
#pragma clang diagnostic pop
}  // namespace details
}  // namespace hardware
}  // namespace android


#endif  // HIDL_GENERATED_VENDOR_SOMC_HARDWARE_CAMERA_CACAO_V3_0_TYPES_H
