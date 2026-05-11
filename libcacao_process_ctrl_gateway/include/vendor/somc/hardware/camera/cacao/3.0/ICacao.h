// ICacao.h — vendor.somc.hardware.camera.cacao@3.0 主介面 stub
// 依照 Ghidra 反編譯 processInternal / passRequest / onInit 還原
// VTable 偏移已依 memory note (gateway-hidl-callback-chain.md) 確認:
//   setCallback=0x34, start=0x38, stop=0x3c, process=0x48, processAsync=0x4c

#pragma once
#include <hidl/HidlSupport.h>
#include <android/hidl/base/1.0/IBase.h>
#include <vendor/somc/hardware/camera/cacao/3.0/types.h>
#include <vendor/somc/hardware/camera/cacao/3.0/ICacaoCallback.h>
#include <functional>

namespace vendor {
namespace somc {
namespace hardware {
namespace camera {
namespace cacao {
namespace V3_0 {

/* 直向型別宣告 */
using android::hardware::Return;
using android::hardware::hidl_string;
using android::hardware::hidl_handle;
using android::hardware::hidl_vec;
using android::sp;

class ICacao : public ::android::hidl::base::V1_0::IBase {
public:
    virtual ~ICacao() = default;

    /* 取得服務實例（回傳 nullptr stub，實際由 SO 提供） */
    static sp<ICacao> getService(const std::string& serviceName = "default");

    /* 嘗試轉型為 V3.1（如果服務支援的話） */
    static sp<ICacao> castFrom(const sp<ICacao>& service);

    /* ── HIDL 方法（vtable 偏移已驗證）────────────────────── */
    /* vtable[0x34 / 4 = idx 13] */
        virtual Return<ErrCode> setCallback(
        const sp<ICacaoCallback>& callback) = 0;

    /* vtable[0x38 / 4 = idx 14] */
        virtual Return<ErrCode> start(const ModeData& mode) = 0;

    /* vtable[0x3c / 4 = idx 15] */
        virtual Return<ErrCode> stop() = 0;

    /* vtable[0x40 / 4 = idx 16]: configure() */
        virtual Return<ErrCode> configure(const hidl_vec<uint8_t>& config) = 0;

    /* vtable[0x44 / 4 = idx 17]: getConfig()
     *   HIDL callback style: getConfig(config_in, callback(config_out, errCode))
     */
    virtual Return<void> getConfig(const hidl_vec<uint8_t>& configIn,
        std::function<void(const Config&, ErrCode)> cb) = 0;

    /* vtable[0x48 / 4 = idx 18]: process()
     *   BpHwCacao::process(const Param&) — 直接傳 Param
     */
    virtual Return<ErrCode> process(const Param& param) = 0;

    /* vtable[0x4c / 4 = idx 19]: processAsync()
     *   BpHwCacao::processAsync(const Param&, const Result&)
     *   param = 處理參數，result = opaque handle buffer (0x48 bytes)
     */
    virtual Return<ErrCode> processAsync(const Param& param,
                                       const Result& result) = 0;

    /* vtable[0x50 / 4 = idx 20]: returnBuffer() */
    virtual Return<ErrCode> returnBuffer(const hidl_handle& bufHandle,
                                       uint32_t colorFormat) = 0;

    /* vtable[0x54 / 4 = idx 21]: getCaps()
     *   getCaps(CameraId, callback(CacaoCaps const&, ErrCode)) */
    virtual Return<void> getCaps(CameraId camId,
        std::function<void(const CacaoCaps&, ErrCode)> cb) = 0;

    /* vtable[0x58 / 4 = idx 22]: getJpegBufferSize()
     *   getJpegBufferSize(ImageSize const&, callback(uint32_t jpegSize, ErrCode))
     *   確認自 BpHwCacao::getJpegBufferSize 符號：RK = const& */
    virtual Return<void> getJpegBufferSize(const ImageSize& size,
        std::function<void(uint32_t jpegSize, ErrCode)> cb) = 0;
};

} // namespace V3_0
} // namespace cacao
} // namespace camera
} // namespace hardware
} // namespace somc
} // namespace vendor
