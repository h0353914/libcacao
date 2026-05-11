// ProcessFactory.cpp — cacao::ProcessFactory 與 cacao::ProcessCtrlCapsFactory 實作
//
// 100% 參照原始 libcacao_client_real.so 邏輯（Ghidra + capstone + lief 分析）：
//
// 1. createEvent(uint, SerializedData*) → return nullptr
//    binary: 00011929  movs r0, #0 ; bx lr
//
// 2. createProcess()
//    binary: 00011925  b.w 0x141cc → ARM thunk → PLT[0xcc88]
//    PLT 目標：GOT[0x18fd8] → _ZN7android5Cacao6createEv
//    實作：return android::Cacao::create()
//
// 3. getJpegBufferSize(ImageSize)
//    binary: 0001192d  b.w 0x141dc → ARM thunk → PLT[0xcc94]
//    PLT 目標：GOT[0x18fdc] → _ZN7android5Cacao17getJpegBufferSizeEN5cacao9ImageSizeE
//    注意：android::Cacao::getJpegBufferSize 透過靜態 mService 存取，不使用 this
//
// 4. ProcessCtrlCapsFactory::getCaps(CameraIndex const&, Caps*)
//    binary: 00011931  b.w 0x141ec → ARM thunk → PLT[0xcca0]
//    PLT 目標：GOT[0x18fe0] → _ZN7android5Cacao7getCapsERKN5cacao15ProcessCtrlCaps11CameraIndexEPNS1_4CapsE
//    注意：android::Cacao::getCaps 透過靜態 mService 存取，不使用 this

#include <cacao/CacaoClient.h>
#include <cacao/ProcessCtrlGatewayBase.h>
#include <cacao/ICacaoService.h>

namespace cacao {

/* ── ProcessFactory ─────────────────────────────────────────── */

/* createEvent — 原始 binary 直接回傳 0（nullptr） */
ProcessEventBase* ProcessFactory::createEvent(uint32_t /*type*/,
                                               ISerialize::SerializedData* /*sd*/) {
    return nullptr;
}

/* createProcess — 轉呼叫 android::Cacao::create()（static factory） */
android::Cacao* ProcessFactory::createProcess() {
    return android::Cacao::create();
}

/* getJpegBufferSize — 轉呼叫 android::Cacao::getJpegBufferSize(size)
 * android::Cacao::getJpegBufferSize 只存取靜態 mService，不依賴 this。
 * 透過 reinterpret_cast 複製原始 binary 的 tail-call 行為。 */
int ProcessFactory::getJpegBufferSize(ImageSize size) {
    return android::Cacao::getJpegBufferSize(size);
}

/* ── ProcessCtrlCapsFactory ─────────────────────────────────── */

/* getCaps — 轉呼叫 android::Cacao::getCaps(camIdx, caps)
 * android::Cacao::getCaps 只存取靜態 mService，不依賴 this。 */
int ProcessCtrlCapsFactory::getCaps(const ProcessCtrlCaps::CameraIndex& camIdx,
                                     Caps* caps) {
    return android::Cacao::getCaps(camIdx, caps);
}

} // namespace cacao
