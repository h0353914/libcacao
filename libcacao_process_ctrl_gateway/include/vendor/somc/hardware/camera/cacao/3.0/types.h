// types.h — vendor.somc.hardware.camera.cacao@3.0 型別 stub
// 依照 libcacao_process_ctrl_gateway.so 反編譯結果重建
// 實際 HIDL 傳輸由 vendor.somc.hardware.camera.cacao@3.0.so 提供

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

/* CameraId 列舉（getCaps 參數） */
enum class CameraId : int32_t {
    CAMERA_ID_0 = 0,
    CAMERA_ID_1 = 1,
};

/* ImageSize — hidl_vec<ImageSize> 在 CacaoCaps 中使用 */
struct ImageSize {
    uint32_t width;
    uint32_t height;
};

/* CacaoCaps V3.0 — getCaps 回傳的結構
 * 從 V3_1::CacaoCaps 拷貝建構子反編譯得到欄位佈局
 * 包含多個 hidl_vec 欄位（ImageSize 和 unsigned_int 配對）
 * V3.0 結構大小約 0xD4 bytes */
struct CacaoCaps {
    uint32_t field_00;                                                   // +0x00
    uint32_t field_04;                                                   // +0x04
    uint32_t field_08;                                                   // +0x08
    uint32_t _pad0c;                                                     // +0x0c
    android::hardware::hidl_vec<ImageSize> sizes0;                       // +0x10
    android::hardware::hidl_vec<uint32_t>  formats0;                     // +0x20
    uint32_t field_30;                                                   // +0x30
    uint32_t _pad34;                                                     // +0x34
    android::hardware::hidl_vec<ImageSize> sizes1;                       // +0x38
    android::hardware::hidl_vec<uint32_t>  formats1;                     // +0x48
    uint32_t field_58;                                                   // +0x58
    uint32_t _pad5c;                                                     // +0x5c
    android::hardware::hidl_vec<ImageSize> sizes2;                       // +0x60
    android::hardware::hidl_vec<uint32_t>  formats2;                     // +0x70
    uint32_t field_80;                                                   // +0x80
    uint32_t _pad84;                                                     // +0x84
    uint32_t field_88;                                                   // +0x88
    uint32_t _pad8c;                                                     // +0x8c
    android::hardware::hidl_vec<ImageSize> sizes3;                       // +0x90
    android::hardware::hidl_vec<uint32_t>  formats3;                     // +0xa0
    android::hardware::hidl_vec<uint32_t>  extra0;                       // +0xb0
    uint32_t field_c0;                                                   // +0xc0
    uint32_t field_c4;                                                   // +0xc4
    uint32_t field_c8;                                                   // +0xc8
    uint32_t field_cc;                                                   // +0xcc
    uint32_t field_d0;                                                   // +0xd0
};

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

/* Config 結構（setConfig 參數，大小從反編譯估算） */
struct Config {
    uint32_t configId;
    uint8_t  data[0x100];
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

/* ModeData — start() HIDL 方法的參數結構
 * 9 個 uint32_t（36 bytes），從 onStart 反組譯確認
 * 對應 ProcessCtrlMode 的 field_08..field_28
 */
struct ModeData {
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

/* Param — copyProcessCtrlParam 目標結構
 * 大小 0xC8 bytes（從 Ghidra memclr(param_3, 200) 推算）
 * processType 在 +0x00，内有 hidl_handle、hidl_vec<ImageBufInfo> 等欄位
 */
struct Param {
    uint32_t processType;   // +0x00
    uint8_t  _reserved[0xC4]; // +0x04 … 0xC7
};

} // namespace V3_0
} // namespace cacao
} // namespace camera
} // namespace hardware
} // namespace somc
} // namespace vendor
