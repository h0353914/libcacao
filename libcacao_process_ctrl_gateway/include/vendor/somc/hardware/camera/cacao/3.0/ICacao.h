// ICacao.h — vendor.somc.hardware.camera.cacao@3.0 主介面 stub
// 依照 Ghidra 反編譯 processInternal / passRequest / onInit 還原
//
// 2026-08-17 用 Ghidra headless（8091, so_32/vendor.somc.hardware.camera.cacao@3.0.so）
// 反編譯 BnHwCacao::onTransact 的完整 switch，transaction code 確認如下
// （即 .hal 真實宣告順序，取代先前的猜測）：
//   1 setCallback, 2 start, 3 stop, 4 setConfig, 5 getConfig,
//   6 process, 7 processAsync, 8 setColorSpaceForHandle,
//   9 getCaps, 10 getJpegBufferSize
// 先前猜測的 configure(hidl_vec<uint8_t>)、returnBuffer 兩個方法實際不存在；
// setConfig/setColorSpaceForHandle 才是真的（來自 BpHwCacao dynsym 確認）。

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

    /* ── HIDL 方法（順序 = transaction code，Ghidra 反編譯確認）────── */

    /* code 1 */
    virtual Return<ErrCode> setCallback(const sp<ICacaoCallback>& callback) = 0;

    /* code 2 */
    virtual Return<ErrCode> start(const ControlMode& mode) = 0;

    /* code 3 */
    virtual Return<ErrCode> stop() = 0;

    /* code 4 — BpHwCacao::setConfig(Config const&) */
    virtual Return<ErrCode> setConfig(const Config& config) = 0;

    /* code 5 — BpHwCacao::getConfig(ConfigId, callback(Config const&, ErrCode)) */
    virtual Return<void> getConfig(ConfigId configId,
        std::function<void(const Config&, ErrCode)> cb) = 0;

    /* code 6 — BpHwCacao::process(Param const&) */
    virtual Return<ErrCode> process(const Param& param) = 0;

    /* code 7 — BpHwCacao::processAsync(Param const&, Result const&)
     *   param = 處理參數，result = opaque handle buffer (0x48 bytes) */
    virtual Return<ErrCode> processAsync(const Param& param,
                                       const Result& result) = 0;

    /* code 8 — BpHwCacao::setColorSpaceForHandle(hidl_handle const&, DataSpace)
     *   先前反編譯完全漏掉這個方法，靠 dynsym 才補上 */
    virtual Return<ErrCode> setColorSpaceForHandle(const hidl_handle& bufHandle,
                                       DataSpace dataSpace) = 0;

    /* code 9 — BpHwCacao::getCaps(CameraIndex, callback(CacaoCaps const&, ErrCode)) */
    virtual Return<void> getCaps(CameraIndex camIdx,
        std::function<void(const CacaoCaps&, ErrCode)> cb) = 0;

    /* code 10 — BpHwCacao::getJpegBufferSize(ImageSize const&, callback(uint32_t, ErrCode)) */
    virtual Return<void> getJpegBufferSize(const ImageSize& size,
        std::function<void(uint32_t jpegSize, ErrCode)> cb) = 0;
};

} // namespace V3_0
} // namespace cacao
} // namespace camera
} // namespace hardware
} // namespace somc
} // namespace vendor
