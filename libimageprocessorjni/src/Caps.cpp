//
// Caps.cpp — cacao::ProcessCtrlResult 建構子/序列化 + cacao::Caps 序列化 + nativeGetCaps 實作
// 依照 so_32 Ghidra 反編譯結果重建
// ProcessCtrlResult ctor 此處定義，使 vtable 隨本 .so 打包（與原始 .so 行為相同）
//

#include <string.h>
#include <android/log.h>
#include <jni.h>
#include <stdlib.h>

#include <imageprocessor/BypassCamera.h>
#include <cacao/CacaoClient.h>
#include <cacao/ProcessCtrlGatewayBase.h>
#include <gui/IGraphicBufferProducer.h>

// cacao::Caps 完整定義
// 繼承 ObjectBase，並提供 getSerializedSize/serialize/deserialize 虛函數
// 讓 vtable 和 Cacao::getCaps 中的 raw vtable 呼叫匹配
// ObjectBase vtable: [0]D1 [1]D0 [2]dump [3]getClassName
// Caps 新增: [4]getSerializedSize [5]serialize [6]deserialize
#pragma GCC visibility push(hidden)
namespace cacao {
class Caps : public ObjectBase {
public:
    Caps() { memset(data, 0, sizeof(data)); }
    virtual ~Caps() {}
    virtual uint32_t getSerializedSize() const { return 0x262c; }
    virtual int serialize(ISerialize::SerializedData* sd) {
        if (!sd || !sd->data) return -0x67;
        if (sd->offset + 0x262cU > sd->size) return -0x67;
        memcpy((uint8_t*)sd->data + sd->offset, data, 0x262c);
        sd->offset += 0x262c;
        return 0;
    }
    virtual int deserialize(ISerialize::SerializedData* sd) {
        if (!sd || !sd->data) return -0x67;
        if (sd->offset + 0x262cU > sd->size) return -0x67;
        memcpy(data, (uint8_t*)sd->data + sd->offset, 0x262c);
        sd->offset += 0x262c;
        return 0;
    }
    uint8_t data[0x2630];
};
} // namespace cacao
#pragma GCC visibility pop

#define LOG_TAG "libimageprocessorjni_real"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

// ─────────────────────────────────────────────────────
// ProcessCtrlResult 實作
// 此 TU 定義建構子，使 vtable 打包進本 .so
// 來自 so_32 Ghidra @ 0xc188
// ─────────────────────────────────────────────────────
#pragma GCC visibility push(hidden)
namespace cacao {

// SerializedData helper macros (matches header: size@+0x184, data@+0x188, offset@+0x18c)
#define SD_SIZE(sd)   ((sd)->size)
#define SD_BUF(sd)    (reinterpret_cast<uint8_t*>((sd)->data))
#define SD_OFF(sd)    ((sd)->offset)

// ─────────────────────────────────────────────────────
// ProcessCtrlMode 實作
// vtable 在本 TU 生成（weak symbol，與 prebuilt 行為相同）
// ─────────────────────────────────────────────────────

ProcessCtrlMode::ProcessCtrlMode()
    : field_08(0), field_0c(-1),
      field_10(0), field_14(0), field_18(0), field_1c(0),
      field_20(0), field_24(0), field_28(0)
{}

ProcessCtrlMode::~ProcessCtrlMode() {}

void ProcessCtrlMode::dump(unsigned int /*flags*/) {}

uint32_t ProcessCtrlMode::getType() const { return field_08; }

uint32_t ProcessCtrlMode::getSerializedSize() const { return 0x24; }

int ProcessCtrlMode::serialize(ISerialize::SerializedData* data) const {
    if (!data || !data->data) return -0x67;
    // base: field_08 (4 bytes)
    if (SD_OFF(data) + 4u > SD_SIZE(data)) return -0x67;
    *reinterpret_cast<uint32_t*>(SD_BUF(data) + SD_OFF(data)) = field_08;
    SD_OFF(data) += 4;
    // mode fields: field_0c..field_28 (8 × 4 = 32 bytes)
    if (SD_OFF(data) + 32u > SD_SIZE(data)) return -0x67;
    uint8_t* dst = SD_BUF(data) + SD_OFF(data);
    *reinterpret_cast<uint32_t*>(dst + 0)  = static_cast<uint32_t>(field_0c);
    *reinterpret_cast<uint32_t*>(dst + 4)  = field_10;
    *reinterpret_cast<uint32_t*>(dst + 8)  = field_14;
    *reinterpret_cast<uint32_t*>(dst + 12) = field_18;
    *reinterpret_cast<uint32_t*>(dst + 16) = field_1c;
    *reinterpret_cast<uint32_t*>(dst + 20) = field_20;
    *reinterpret_cast<uint32_t*>(dst + 24) = field_24;
    *reinterpret_cast<uint32_t*>(dst + 28) = field_28;
    SD_OFF(data) += 32;
    return 0;
}

int ProcessCtrlMode::deserialize(ISerialize::SerializedData* data) {
    if (!data || !data->data) return -0x67;
    if (SD_OFF(data) + 4u > SD_SIZE(data)) return -0x67;
    field_08 = *reinterpret_cast<uint32_t*>(SD_BUF(data) + SD_OFF(data));
    SD_OFF(data) += 4;
    if (SD_OFF(data) + 32u > SD_SIZE(data)) return -0x67;
    uint8_t* src = SD_BUF(data) + SD_OFF(data);
    field_0c = static_cast<int32_t>(*reinterpret_cast<uint32_t*>(src + 0));
    field_10 = *reinterpret_cast<uint32_t*>(src + 4);
    field_14 = *reinterpret_cast<uint32_t*>(src + 8);
    field_18 = *reinterpret_cast<uint32_t*>(src + 12);
    field_1c = *reinterpret_cast<uint32_t*>(src + 16);
    field_20 = *reinterpret_cast<uint32_t*>(src + 20);
    field_24 = *reinterpret_cast<uint32_t*>(src + 24);
    field_28 = *reinterpret_cast<uint32_t*>(src + 28);
    SD_OFF(data) += 32;
    return 0;
}

// ─────────────────────────────────────────────────────
// ProcessCtrlResult 實作
// 此 TU 定義建構子，使 vtable 打包進本 .so
// ─────────────────────────────────────────────────────

ProcessCtrlResult::ProcessCtrlResult()
    : field_08(0), field_0c(-1), field_10(0),
      field_14(0), field_18(0), field_1c(0), field_20(0xFFFFFFFF),
      field_24(0), field_28(0), field_2c(0xFFFF0000), field_30(0xFF)
{
    _pad[0] = _pad[1] = _pad[2] = 0;
}

ProcessCtrlResult::~ProcessCtrlResult() {}

/* ProcessCtrlResult::serialize — 對應 so_32 REF 0x1cbbc
 * Phase 1: 寫 field_08 (result) + field_0c (progress) = 8B
 * Phase 2: 寫 field_14..field_30+pad = 0x20B (跳過 field_10 = userData)
 * 64-bit 修正：使用 named members 直接存取，避免 32-bit 硬編碼偏移量
 * Ghidra 確認：錯誤碼為 0xffffff99 = -0x67 */
int ProcessCtrlResult::serialize(ISerialize::SerializedData* data) const {
    if (!data || !data->data) return -0x67;
    // Phase 1: 8 bytes (field_08, field_0c)
    if (SD_OFF(data) + 8u > SD_SIZE(data)) return -0x67;
    memcpy(SD_BUF(data) + SD_OFF(data), &field_08, 8);
    SD_OFF(data) += 8;
    // Phase 2: 0x20 bytes (field_14..field_30+pad, skipping field_10=userData)
    if (SD_OFF(data) + 0x20u > SD_SIZE(data)) return -0x67;
    memcpy(SD_BUF(data) + SD_OFF(data), &field_14, 0x20);
    SD_OFF(data) += 0x20;
    return 0;
}

int ProcessCtrlResult::deserialize(ISerialize::SerializedData* data) {
    if (!data || !data->data) return -0x67;
    if (SD_OFF(data) + 8u > SD_SIZE(data)) return -0x67;
    memcpy(&field_08, SD_BUF(data) + SD_OFF(data), 8);
    SD_OFF(data) += 8;
    if (SD_OFF(data) + 0x20u > SD_SIZE(data)) return -0x67;
    memcpy(&field_14, SD_BUF(data) + SD_OFF(data), 0x20);
    SD_OFF(data) += 0x20;
    return 0;
}

// ─────────────────────────────────────────────────────
// ProcessCtrlParam 實作
// ─────────────────────────────────────────────────────

ProcessCtrlParam::ProcessCtrlParam() {
    memset(_data, 0, sizeof(_data));
}

ProcessParamBase* ProcessCtrlParam::clone() const {
    void* mem = ObjectBase::operator new(0x160);
    if (!mem) return nullptr;
    ProcessCtrlParam* c = ::new(mem) ProcessCtrlParam();
    memcpy(c->_data, _data, sizeof(_data));
    return c;
}

/* ProcessCtrlParam::copy/serialize/deserialize
 * 64-bit 修正：使用 _data 成員直接存取，避免 this+0x08 在 64-bit 讀到 vtable 指針
 * Ghidra 確認：錯誤碼為 0xffffff99 = -0x67 */
void ProcessCtrlParam::copy(const ProcessParamBase* src) {
    if (!src) return;
    const ProcessCtrlParam* s = static_cast<const ProcessCtrlParam*>(src);
    *reinterpret_cast<uint32_t*>(_data) = *reinterpret_cast<const uint32_t*>(s->_data);
    memcpy(_data + 8, s->_data + 8, 0x150);
}

uint32_t ProcessCtrlParam::getSerializedSize() const { return 0x158; }

int ProcessCtrlParam::serialize(ISerialize::SerializedData* data) const {
    if (!data || !data->data || SD_OFF(data) + 0x158u > SD_SIZE(data)) return -0x67;
    uint8_t* dst = SD_BUF(data) + SD_OFF(data);
    *reinterpret_cast<uint32_t*>(dst)     = *reinterpret_cast<const uint32_t*>(_data);  // type field
    *reinterpret_cast<uint32_t*>(dst + 4) = 0;
    memcpy(dst + 8, _data + 8, 0x150);
    SD_OFF(data) += 0x158;
    return 0;
}

int ProcessCtrlParam::deserialize(ISerialize::SerializedData* data) {
    if (!data || !data->data || SD_OFF(data) + 0x158u > SD_SIZE(data)) return -0x67;
    uint8_t* src = SD_BUF(data) + SD_OFF(data);
    *reinterpret_cast<uint32_t*>(_data) = *reinterpret_cast<uint32_t*>(src);            // type field
    memcpy(_data + 8, src + 8, 0x150);
    SD_OFF(data) += 0x158;
    return 0;
}

// ─────────────────────────────────────────────────────
// ProcessCtrlDynamicParameterConfig serialize/deserialize
// ─────────────────────────────────────────────────────

int ProcessCtrlDynamicParameterConfig::serialize(ISerialize::SerializedData* data) const {
    if (!data || !data->data || SD_OFF(data) + 4u > SD_SIZE(data)) return -0x67;
    *reinterpret_cast<uint32_t*>(SD_BUF(data) + SD_OFF(data)) = configValue;
    SD_OFF(data) += 4;
    return 0;
}

int ProcessCtrlDynamicParameterConfig::deserialize(ISerialize::SerializedData* data) {
    if (!data || !data->data || SD_OFF(data) + 4u > SD_SIZE(data)) return -0x67;
    configValue = *reinterpret_cast<uint32_t*>(SD_BUF(data) + SD_OFF(data));
    SD_OFF(data) += 4;
    return 0;
}

// ─────────────────────────────────────────────────────
// ProcessCtrlVideoRecParam 實作
// ─────────────────────────────────────────────────────

ProcessCtrlVideoRecParam::ProcessCtrlVideoRecParam() {
    memset(_data, 0, sizeof(_data));
    memset(_extra, 0, sizeof(_extra));
    // type = VIDEO_REC (0x0e)
    *reinterpret_cast<uint32_t*>(_data) = 0x0e;
}

ProcessCtrlVideoRecParam::~ProcessCtrlVideoRecParam() {
    // REF so_64 @ 0xe6fc: ldr x8,[x19,#0x168] then cbz/bl decStrong(id=this+0x168)
    // 64-bit: ~sp() on _extra[0..1] (this+0x168); sp is always null (never set by caller)
    using SP = android::sp<android::IGraphicBufferProducer>;
    reinterpret_cast<SP*>(_extra)->~SP();
}

ProcessParamBase* ProcessCtrlVideoRecParam::clone() const {
    void* mem = ObjectBase::operator new(0x170);
    if (!mem) return nullptr;
    ProcessCtrlVideoRecParam* c = ::new(mem) ProcessCtrlVideoRecParam();
    if (c) c->copy(this);
    return c;
}

/* ProcessCtrlVideoRecParam::copy/serialize/deserialize
 * 64-bit 布局 (_extra 在 this+0x168):
 *   [0,1] = sp<GBP>.m_ptr (8B) @ 0x168 — 從不設定，始終 null
 *   [2]   = dataspace @ 0x170     (BypassCameraVideo.cpp 設定)
 *   [3]   = frameRate  @ 0x174    (BypassCameraVideo.cpp 設定)
 * 32-bit 布局 (_extra 在 this+0x160):
 *   [0]   = sp<GBP>.m_ptr (4B) @ 0x160
 *   [1]   = dataspace @ 0x164
 *   [2]   = frameRate  @ 0x168 */
void ProcessCtrlVideoRecParam::copy(const ProcessParamBase* src) {
    if (!src) return;
    const ProcessCtrlVideoRecParam* s = static_cast<const ProcessCtrlVideoRecParam*>(src);
    *reinterpret_cast<uint32_t*>(_data) = *reinterpret_cast<const uint32_t*>(s->_data);
    memcpy(_data + 8, s->_data + 8, 0x150);
    // 複製 _extra（sp.m_ptr + dataspace/frameRate）
    // 注：64-bit sp.m_ptr 始終 null，對於 null sp 的 incStrong 與 memcpy 等僷
    memcpy(_extra, s->_extra, sizeof(_extra));
}

uint32_t ProcessCtrlVideoRecParam::getSerializedSize() const { return 0x160; }

int ProcessCtrlVideoRecParam::serialize(ISerialize::SerializedData* data) const {
    if (!data || !data->data || SD_OFF(data) + 0x158u > SD_SIZE(data)) return -0x67;
    uint8_t* dst = SD_BUF(data) + SD_OFF(data);
    *reinterpret_cast<uint32_t*>(dst)     = *reinterpret_cast<const uint32_t*>(_data);  // type field
    *reinterpret_cast<uint32_t*>(dst + 4) = 0;
    memcpy(dst + 8, _data + 8, 0x150);
    SD_OFF(data) += 0x158;
    // VideoRecParam 額外序列化：_extra[2] (dataspace) 和 _extra[3] (frameRate)
    if (SD_OFF(data) + 8u > SD_SIZE(data)) return -0x67;
    dst = SD_BUF(data) + SD_OFF(data);
    *reinterpret_cast<uint32_t*>(dst)     = _extra[2];  // dataspace
    *reinterpret_cast<uint32_t*>(dst + 4) = _extra[3];  // frameRate
    SD_OFF(data) += 8;
    return 0;
}

int ProcessCtrlVideoRecParam::deserialize(ISerialize::SerializedData* data) {
    if (!data || !data->data || SD_OFF(data) + 0x158u > SD_SIZE(data)) return -0x67;
    uint8_t* src = SD_BUF(data) + SD_OFF(data);
    *reinterpret_cast<uint32_t*>(_data) = *reinterpret_cast<uint32_t*>(src);            // type field
    memcpy(_data + 8, src + 8, 0x150);
    SD_OFF(data) += 0x158;
    // VideoRecParam 額外反序列化：_extra[2] (dataspace) 和 _extra[3] (frameRate)
    if (SD_OFF(data) + 8u > SD_SIZE(data)) return -0x67;
    src = SD_BUF(data) + SD_OFF(data);
    _extra[2] = *reinterpret_cast<uint32_t*>(src);        // dataspace
    _extra[3] = *reinterpret_cast<uint32_t*>(src + 4);    // frameRate
    SD_OFF(data) += 8;
    return 0;
}

} // namespace cacao
#pragma GCC visibility pop

// ─────────────────────────────────────────────────────
// ProcessCtrlResult 建立輔助（設定 userData = ctx 指標）
// ─────────────────────────────────────────────────────
__attribute__((visibility("hidden")))
cacao::ProcessCtrlResult* createResultWithCtx(imageprocessor::BypassCameraContext* ctx) {
    cacao::ProcessCtrlResult* r = new cacao::ProcessCtrlResult();
    if (r) {
        // getUserData() 回傳 field_10 作為指針
        // field_10 是 uintptr_t（大小隨平台，支援 64-bit）
        r->field_10 = (uintptr_t)ctx;
    }
    return r;
}

// ─────────────────────────────────────────────────────
// BypassCamera_getCaps — nativeGetCaps 實作
// 來自 so_32 @ 0x000179a0
// 呼叫 ProcessCtrlCapsFactory::getCaps，然後透過 JNI 反射填入 Java Capability 物件
//
// Caps 物件佈局（32-bit size = 0x2634，data 從 this+8 開始）：
//   由於 64-bit vtable 佔 8 bytes（ObjectBase 單繼承），data 從 this+8
//   Caps::data[0x2630] 對齊到 32-bit 偏移量：
//     32-bit this+N  ⟹  data[N - 8]
//
// 欄位映射（32-bit offset → data offset → Java setter）：
//   +0x08  → data[0x000] = videoStabilizationMode (uint32)
//   +0x0c  → data[0x004] = steadyShotCount (uint32)
//   +0x10  → data[0x008] = steadyShot[].w (stride 8)
//   +0x14  → data[0x00c] = steadyShot[].h (stride 8)
//   +0x410 → data[0x408] = steadyShot[].flags (stride 4)
//   +0x610 → data[0x608] = intelligentActiveCount (uint32)
//   +0x614 → data[0x60c] = intelligentActive[].w (stride 8)
//   +0x618 → data[0x610] = intelligentActive[].h (stride 8)
//   +0xa14 → data[0xa0c] = intelligentActive[].flags (stride 4)
//   +0xc14 → data[0xc0c] = highFrameRateCount (uint32)
//   +0xc18 → data[0xc10] = highFrameRate[].w (stride 8)
//   +0xc1c → data[0xc14] = highFrameRate[].h (stride 8)
//   +0x1018→ data[0x1010]= highFrameRate[].flags (stride 4)
//   +0x1218→ data[0x1210]= superSlowMode (uint32)
//   +0x121c→ data[0x1214]= superSlowSupportedCount (uint32)
//   +0x1220→ data[0x1218]= superSlowSupported[].w (stride 8)
//   +0x1224→ data[0x121c]= superSlowSupported[].h (stride 8)
//   +0x1620→ data[0x1618]= superSlowSupported[].flags (stride 4)
//   +0x1820→ data[0x1818]= superSlowFrameNum[] (stride 4)
//   +0x1a20→ data[0x1a18]= climaxRecognitionMode (uint32)
//   +0x1a24→ data[0x1a1c]= burstMode (uint32)
//   +0x1a28→ data[0x1a20]= videoHdrMode (uint32)
// ─────────────────────────────────────────────────────

// data 中的偏移量（32-bit this offset - 8）
#define CAPS_OFF_VIDEO_STAB          0x000
#define CAPS_OFF_STEADYSHOT_COUNT    0x004
#define CAPS_OFF_STEADYSHOT_WH       0x008   // stride 8 (w,h pairs)
#define CAPS_OFF_STEADYSHOT_FLAGS    0x408   // stride 4
#define CAPS_OFF_IA_COUNT            0x608
#define CAPS_OFF_IA_WH               0x60c   // stride 8
#define CAPS_OFF_IA_FLAGS            0xa0c   // stride 4
#define CAPS_OFF_HFR_COUNT           0xc0c
#define CAPS_OFF_HFR_WH              0xc10   // stride 8
#define CAPS_OFF_HFR_FLAGS           0x1010  // stride 4
#define CAPS_OFF_SUPER_SLOW_MODE     0x1210
#define CAPS_OFF_SS_COUNT            0x1214
#define CAPS_OFF_SS_WH               0x1218  // stride 8
#define CAPS_OFF_SS_FLAGS            0x1618  // stride 4
#define CAPS_OFF_SS_FRAMENUM         0x1818  // stride 4
#define CAPS_OFF_CLIMAX              0x1a18
#define CAPS_OFF_BURST               0x1a1c
#define CAPS_OFF_VIDEO_HDR           0x1a20

static inline uint32_t capsU32(const cacao::Caps* caps, uint32_t off) {
    return *reinterpret_cast<const uint32_t*>(caps->data + off);
}

__attribute__((visibility("hidden")))
int BypassCamera_getCaps(JNIEnv* env, jint cameraIdx, jobject capsObj) {
    if (!capsObj) return -1;

    // 1. 配置 Caps 物件
    cacao::Caps* caps = new cacao::Caps();

    // 2. 呼叫 ProcessCtrlCapsFactory::getCaps 填入 caps 數據
    cacao::ProcessCtrlCaps::CameraIndex camIdx;
    camIdx.index = (int)cameraIdx;
    int ret = cacao::ProcessCtrlCapsFactory::getCaps(camIdx, caps);
    if (ret < 0) {
        ALOGE("BypassCamera_getCaps: getCaps failed ret=%d", ret);
        delete caps;
        return ret;
    }

    // 3. 取得 Java Capability class 和方法 ID
    jclass cls = env->GetObjectClass(capsObj);
    if (!cls) { delete caps; return -1; }
    if (env->ExceptionCheck()) { env->ExceptionClear(); delete caps; return -1; }

    jmethodID mSetVideoStab    = env->GetMethodID(cls, "setVideoStabilizationMode", "(I)V");
    jmethodID mSetSuperSlow    = env->GetMethodID(cls, "setSuperSlowMode", "(I)V");
    jmethodID mSetClimax       = env->GetMethodID(cls, "setClimaxRecognitionMode", "(I)V");
    jmethodID mSetBurst        = env->GetMethodID(cls, "setBurstMode", "(I)V");
    jmethodID mSetVideoHdr     = env->GetMethodID(cls, "setVideoHdrMode", "(I)V");
    jmethodID mAddHFR          = env->GetMethodID(cls, "addHighFrameRateSupportedInfo", "(III)V");
    jmethodID mAddSteady       = env->GetMethodID(cls, "addSteadyShotSupportedInfo", "(III)V");
    jmethodID mAddIA           = env->GetMethodID(cls, "addIntelligentActiveSupportedInfo", "(III)V");
    jmethodID mAddSSInfo       = env->GetMethodID(cls, "addSuperSlowSupportedInfo", "(III)V");
    jmethodID mAddSSFrameNum   = env->GetMethodID(cls, "addSuperSlowFrameNum", "(I)V");

    if (env->ExceptionCheck()) { env->ExceptionClear(); }

    // 4. 呼叫 Java setter（依照 so_32 @ 0x179a0 的順序）
    env->CallVoidMethod(capsObj, mSetVideoStab,
        (jint)capsU32(caps, CAPS_OFF_VIDEO_STAB));
    env->CallVoidMethod(capsObj, mSetSuperSlow,
        (jint)capsU32(caps, CAPS_OFF_SUPER_SLOW_MODE));
    env->CallVoidMethod(capsObj, mSetClimax,
        (jint)capsU32(caps, CAPS_OFF_CLIMAX));
    env->CallVoidMethod(capsObj, mSetBurst,
        (jint)capsU32(caps, CAPS_OFF_BURST));

    // videoHdrMode: 只有非零才呼叫（so_32 有 null check）
    if (mSetVideoHdr) {
        env->CallVoidMethod(capsObj, mSetVideoHdr,
            (jint)capsU32(caps, CAPS_OFF_VIDEO_HDR));
    }

    // Loop 1: addHighFrameRateSupportedInfo
    uint32_t hfrCount = capsU32(caps, CAPS_OFF_HFR_COUNT);
    for (uint32_t i = 0; i < hfrCount; i++) {
        uint32_t w     = *reinterpret_cast<const uint32_t*>(caps->data + CAPS_OFF_HFR_WH + i * 8);
        uint32_t h     = *reinterpret_cast<const uint32_t*>(caps->data + CAPS_OFF_HFR_WH + i * 8 + 4);
        uint32_t flags = *reinterpret_cast<const uint32_t*>(caps->data + CAPS_OFF_HFR_FLAGS + i * 4);
        env->CallVoidMethod(capsObj, mAddHFR, (jint)w, (jint)h, (jint)flags);
    }

    // Loop 2: addSteadyShotSupportedInfo
    uint32_t steadyCount = capsU32(caps, CAPS_OFF_STEADYSHOT_COUNT);
    for (uint32_t i = 0; i < steadyCount; i++) {
        uint32_t w     = *reinterpret_cast<const uint32_t*>(caps->data + CAPS_OFF_STEADYSHOT_WH + i * 8);
        uint32_t h     = *reinterpret_cast<const uint32_t*>(caps->data + CAPS_OFF_STEADYSHOT_WH + i * 8 + 4);
        uint32_t flags = *reinterpret_cast<const uint32_t*>(caps->data + CAPS_OFF_STEADYSHOT_FLAGS + i * 4);
        env->CallVoidMethod(capsObj, mAddSteady, (jint)w, (jint)h, (jint)flags);
    }

    // Loop 3: addIntelligentActiveSupportedInfo
    uint32_t iaCount = capsU32(caps, CAPS_OFF_IA_COUNT);
    for (uint32_t i = 0; i < iaCount; i++) {
        uint32_t w     = *reinterpret_cast<const uint32_t*>(caps->data + CAPS_OFF_IA_WH + i * 8);
        uint32_t h     = *reinterpret_cast<const uint32_t*>(caps->data + CAPS_OFF_IA_WH + i * 8 + 4);
        uint32_t flags = *reinterpret_cast<const uint32_t*>(caps->data + CAPS_OFF_IA_FLAGS + i * 4);
        env->CallVoidMethod(capsObj, mAddIA, (jint)w, (jint)h, (jint)flags);
    }

    // Loop 4: addSuperSlowSupportedInfo + addSuperSlowFrameNum
    uint32_t ssCount = capsU32(caps, CAPS_OFF_SS_COUNT);
    for (uint32_t i = 0; i < ssCount; i++) {
        uint32_t w     = *reinterpret_cast<const uint32_t*>(caps->data + CAPS_OFF_SS_WH + i * 8);
        uint32_t h     = *reinterpret_cast<const uint32_t*>(caps->data + CAPS_OFF_SS_WH + i * 8 + 4);
        uint32_t flags = *reinterpret_cast<const uint32_t*>(caps->data + CAPS_OFF_SS_FLAGS + i * 4);
        env->CallVoidMethod(capsObj, mAddSSInfo, (jint)w, (jint)h, (jint)flags);
        uint32_t frameNum = *reinterpret_cast<const uint32_t*>(caps->data + CAPS_OFF_SS_FRAMENUM + i * 4);
        env->CallVoidMethod(capsObj, mAddSSFrameNum, (jint)frameNum);
    }

    env->DeleteLocalRef(cls);
    delete caps;
    return ret;
}
