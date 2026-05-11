/*
 * android/binder_interface_utils.h — 最小 stub
 *
 * 讓 VNDK v30 AIDL generated headers 通過編譯。
 * 實際上 ProcessCtrlGateway 不直接使用 AIDL interface，
 * 只是 gui/Surface.h → ui/GraphicTypes.h 間接拉入。
 *
 * Soong build 時使用 #include_next 轉發到 AOSP 真實 header。
 */
#ifdef LIBCACAO_SOONG_BUILD
#pragma once
#include_next <android/binder_interface_utils.h>
#else
#pragma once

#include <cstdint>
#include <memory>
#include <string>

typedef int32_t binder_status_t;
struct AParcel;
#endif
