// types.h — vendor.somc.hardware.camera.cacao@3.0 型別 stub
// 依照 libcacao_process_ctrl_gateway.so 反編譯結果重建
// 實際 HIDL 傳輸由 vendor.somc.hardware.camera.cacao@3.0.so 提供
//
// 2026-08-17 用 Ghidra headless（8091, so_32/vendor.somc.hardware.camera.cacao@3.0.so）
// 反編譯 BnHwCacao::onTransact 與 dynsym 校正：
//   - enum 真實名稱是 CameraIndex（非 CameraId）、ControlMode（非 ModeData）
//
// 2026-08-18 進一步反編譯 writeEmbeddedToParcel(CacaoCaps/VideoStabilizationCaps/
// SuperSlowCaps/SupportedInfo const&, ...) 確認 CacaoCaps 真實是巢狀具名子結構
// （VideoStabilizationCaps + SuperSlowCaps），不是先前那份扁平 sizes0..formats3
// 陣列猜測。用組合語言核對過 hidl_vec<T> 在此 ABI 上 buffer 在 vec+0、count 在
// vec+8（hidl_pointer 為跨 32/64-bit 相容固定占 8 bytes），故只要用真正的
// android::hardware::hidl_vec<T> 型別，位元組佈局就會自動吻合，不需要手動兜 offset。
// JpegSetting/VideoHFRCaps 存在於 dynsym 但目前找不到任何呼叫端把它們接進
// getCaps()/getCapsV3_1() 的鏈路，暫不處理。V3_1::CacaoCaps 的擴充欄位
// （base V3_0::CacaoCaps 之後、+0xD0 與 +0xF8 各一個約 40 bytes 的子結構）
// 卡在一個 ARM/Thumb interworking veneer + PLT 間接跳轉，還沒解出真正型別。

#pragma once
#include <stdint.h>
#include <hidl/HidlSupport.h>

namespace vendor {
namespace somc {
namespace hardware {
namespace camera {
namespace cacao {
namespace V3_0 {

/* ErrCode 列舉（從反編譯 callback 中觀察到的用法） */
enum class ErrCode : int32_t {
    OK           = 0,
    NO_ERROR     = 0,
    ERR_GENERAL  = -1,
};

/* CameraIndex 列舉（getCaps 參數；dynsym 確認真實名稱為 CameraIndex） */
enum class CameraIndex : int32_t {
    CAMERA_ID_0 = 0,
    CAMERA_ID_1 = 1,
};

/* ConfigId 列舉（getConfig 參數；dynsym 確認存在，實際值未反查） */
enum class ConfigId : int32_t {
    UNKNOWN = 0,
};

/* DataSpace（setColorSpaceForHandle 參數；dynsym 確認存在，實際值未反查，
 * 很可能對應 android::hardware::graphics::common 的 Dataspace，待驗證） */
enum class DataSpace : int32_t {
    UNKNOWN = 0,
};

/* ImageSize — hidl_vec<ImageSize> 在 CacaoCaps 中使用 */
struct ImageSize {
    uint32_t width;
    uint32_t height;
};

/* SupportedInfo — 2026-08-18 反編譯 writeEmbeddedToParcel(SupportedInfo const&, ...)
 * 確認：兩個 hidl_vec 欄位，中間/前面各留白（不需要 embedded marshalling 的
 * 純量欄位，語意未知，用 _reserved 佔位——欄位順序與型別對 wire 正確就夠，
 * 名稱不影響序列化）。VideoHFRCaps 的 writeEmbeddedToParcel 是完全相同的
 * 函式位址，應是 .hal 裡對同一結構的 typedef，故沿用同一份定義。 */
struct SupportedInfo {
    uint32_t _reserved0[2];                                              // +0x00 (8 bytes)
    android::hardware::hidl_vec<ImageSize> sizes;                        // +0x08
    android::hardware::hidl_vec<uint32_t>  formats;                      // +0x18
};                                                                        // sizeof = 0x28 (40)
using VideoHFRCaps = SupportedInfo;

/* SuperSlowCaps — 反編譯 writeEmbeddedToParcel(SuperSlowCaps const&, ...) 確認
 * 三個 hidl_vec 到 +0x40 為止。尾端 _reserved1[4] 的大小（16 bytes，非單靠
 * writeEmbeddedToParcel 能看出來）是從 V3_1::CacaoCaps 交叉驗證：反編譯
 * V3_1::writeEmbeddedToParcel(CacaoCaps) 直接呼叫 base V3_0::CacaoCaps 的
 * writer 後，緊接著在絕對 offset +0xD0 開始寫自己的擴充欄位（見 3.1/ICacao.h），
 * 也就是編譯器認定 sizeof(V3_0::CacaoCaps)=0xD0，回推 SuperSlowCaps=0x50。
 * 舊版 app-facing flattenCacaoCapsV30 攤平格式只用了尾端 4 個保留字中的
 * 前兩個（field_c0/field_c4），另外兩個目前沒有已知的 app-facing 對應。 */
struct SuperSlowCaps {
    uint32_t _reserved0[4];                                              // +0x00 (16 bytes)
    android::hardware::hidl_vec<ImageSize> sizes;                        // +0x10
    android::hardware::hidl_vec<uint32_t>  formats0;                     // +0x20
    android::hardware::hidl_vec<uint32_t>  formats1;                     // +0x30
    uint32_t _reserved1[4];                                              // +0x40 (16 bytes)
};                                                                        // sizeof = 0x50 (80)

/* VideoStabilizationCaps — 反編譯 writeEmbeddedToParcel(VideoStabilizationCaps
 * const&, ...) 確認四個 hidl_vec 欄位；總大小 0x60 是從 CacaoCaps 內嵌它之後
 * 緊接著下一個欄位（sizes @+0x60）反推確認，不是猜測。 */
struct VideoStabilizationCaps {
    uint32_t _reserved0[4];                                              // +0x00 (16 bytes)
    android::hardware::hidl_vec<ImageSize> sizes0;                       // +0x10
    android::hardware::hidl_vec<uint32_t>  formats0;                     // +0x20
    uint32_t _reserved1[2];                                              // +0x30 (8 bytes)
    android::hardware::hidl_vec<ImageSize> sizes1;                       // +0x38
    android::hardware::hidl_vec<uint32_t>  formats1;                     // +0x48
    uint32_t _reserved2[2];                                              // +0x58 (8 bytes，補滿到 0x60)
};                                                                        // sizeof = 0x60 (96，CacaoCaps context 確認)

/* CacaoCaps V3.0 — getCaps 回傳的結構。
 * 2026-08-18 反編譯 writeEmbeddedToParcel(CacaoCaps const&, ...) 完全推翻先前
 * 「一堆 sizes0..formats3 扁平陣列」的猜測：CacaoCaps 其實是巢狀具名子結構——
 * 開頭直接內嵌 VideoStabilizationCaps（reinterpret_cast this 呼叫，代表它是
 * CacaoCaps 的第一個成員），接著兩個 hidl_vec，最後內嵌 SuperSlowCaps。
 * SuperSlowCaps 尾端大小用 V3_1::CacaoCaps 的擴充欄位絕對 offset 交叉驗證
 * 確認（見上方註解），sizeof(CacaoCaps)=0xD0 是編譯器實際採用的值，不是估算。 */
struct CacaoCaps {
    VideoStabilizationCaps videoStabilization;                           // +0x00 (0x60)
    android::hardware::hidl_vec<ImageSize> sizes;                        // +0x60
    android::hardware::hidl_vec<uint32_t>  formats;                      // +0x70
    SuperSlowCaps superSlow;                                             // +0x80 (0x50)
};                                                                        // sizeof = 0xD0 (208)

/* 處理型別（ProcessType），從 convert() 輸入觀察 */
enum class ProcessType : uint32_t {
    STILL         = 0,
    PREPAREVIDXXX = 1,
    VIDEOCAP_DONE = 8,
    PREPARE_VIDEO = 10,
    START_VIDEO   = 11,
    STOP_VIDEO    = 12,
    TRIGGER_SUPER = 13,
};

/* Config 結構 — 2026-08-17 用 Ghidra 反編譯 BpHwCacao::_hidl_setConfig
 * （writeBuffer(parcel, &config, 8, …)）與 _hidl_getConfig
 * （readBuffer(parcel, 8, …)）確認：整個結構固定 8 bytes，兩個 uint32_t，
 * 不是先前猜測的 0x104-byte「configId + 256-byte data」。
 * 對照 ProcessCtrlGateway::setConfig 送出的 {mBuffer=0, mSize=dpType} 二字，
 * 第二個欄位就是 dpType 本身，非指標。 */
struct Config {
    uint32_t configId;
    uint32_t value;
};

/* ControlMode — start() HIDL 方法的參數結構（dynsym 確認真實名稱為
 * ControlMode，非先前猜測的 ModeData）。9 個 uint32_t（36 bytes），
 * 從 onStart 反組譯確認，對應 ProcessCtrlMode 的 field_08..field_28 */
struct ControlMode {
    uint32_t modeType;          // mode->getType()
    int32_t  camIdx;            // field_0c
    uint32_t maxStreamWidth;    // field_10
    uint32_t maxStreamHeight;   // field_14
    uint32_t captureWidth;      // field_18
    uint32_t captureHeight;     // field_1c
    uint32_t maxPrdCaptureNum;  // field_20
    uint32_t field_24;          // field_24
    uint32_t field_28;          // field_28
};

/* ImageBufInfo（從 REF copyProcessCtrlParam 彙編確認佈局）
 * REF asm(1b9c8): str w→[info+0], str h→[info+4], handle.setTo→[info+8], colorFmt→[info+0x18]
 * sizeof(hidl_handle)=16 bytes（HIDL_ALIGNED 8，含 mHandle ptr + mOwnsHandle + padding）
 * sizeof(ImageBufInfo)=0x20(32) bytes（REF copyFrom 的 element stride = 0x20）
 */
struct ImageBufInfo {
    uint32_t                       width;    // +0x00
    uint32_t                       height;   // +0x04
    android::hardware::hidl_handle handle;   // +0x08 (16 bytes)
    uint32_t                       colorFmt; // +0x18
    uint32_t                       padding;  // +0x1c
};

/* Result — 從 handleProgress/handleResult 的 memcpy(dst, src, 0x48) 確認大小 0x48
 * 第一個欄位（+0x00）是 opaque handle（gateway 端 ResultItem 指標，用於 callback 回傳時匹配）
 * cald 原封不動地回傳整個 Result buffer，gateway 在 onHandleResultInternal 中 memcpy 回 ResultItem
 * 欄位 +0x18 = processType（convert() 後的值），其他欄位由 cald 填入 */
struct Result {
    uint8_t  data[0x48];
};

/* Event — handleEvent 參數（大小短，約 0x10） */
struct Event {
    uint32_t eventType;
    uint32_t reserved[3];
};

/* Param — process()/processAsync() 的參數結構。
 * 2026-08-18 反編譯 writeEmbeddedToParcel(Param const&, ...) 確認四個內嵌
 * 欄位：hidl_string@+0x30、hidl_handle@+0x78、hidl_handle@+0x98、
 * hidl_vec<ImageBufInfo>@+0xb8（element stride <<5＝×32，正好對上已確認的
 * sizeof(ImageBufInfo)=0x20；函式內對這個 vec 的逐一走訪還直接印證了
 * ImageBufInfo::handle 真的在元素 +8——與現有 ImageBufInfo 定義一致，不用改）。
 * 總大小 0xC8，與舊筆記「Ghidra memclr(param_3, 200)」完全吻合，只是這次
 * 找出了 200 bytes 裡實際的欄位邊界，而不是整塊當作純量佔位。
 * 欄位之間的空隙（語意未知的純量）用 _reservedN 佔位，不影響 wire 佈局。 */
struct Param {
    uint32_t processType;                        // +0x00
    uint32_t _reserved0[11];                      // +0x04 (44 bytes)
    android::hardware::hidl_string name;          // +0x30 (16 bytes)
    uint32_t _reserved1[14];                      // +0x40 (56 bytes)
    android::hardware::hidl_handle handle0;       // +0x78 (16 bytes)
    uint32_t _reserved2[4];                       // +0x88 (16 bytes)
    android::hardware::hidl_handle handle1;       // +0x98 (16 bytes)
    uint32_t _reserved3[4];                       // +0xa8 (16 bytes)
    android::hardware::hidl_vec<ImageBufInfo> bufInfos; // +0xb8 (16 bytes)
};                                                 // sizeof = 0xC8 (200)

} // namespace V3_0
} // namespace cacao
} // namespace camera
} // namespace hardware
} // namespace somc
} // namespace vendor
