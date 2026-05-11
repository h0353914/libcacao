/*
 * android/binder_enums.h — 相容 stub
 *
 * 提供 VNDK v30 AIDL generated headers 所需的 ndk::enum_range 等模板。
 * API 28 NDK sysroot 中不包含此 header，此 stub 僅複製功能定義。
 *
 * Soong build 時使用 #include_next 轉發到 AOSP 真實 header。
 */
#ifdef LIBCACAO_SOONG_BUILD
#pragma once
#include_next <android/binder_enums.h>
#else
#pragma once

#include <iterator>
#include <type_traits>

namespace ndk {

namespace internal {
template <typename T>
struct invalid_type;

template <typename EnumType,
          typename = typename std::enable_if<std::is_enum<EnumType>::value>::type>
constexpr invalid_type<EnumType> enum_values;
}  // namespace internal

template <typename EnumType,
          typename = typename std::enable_if<std::is_enum<EnumType>::value>::type>
struct enum_range {
    constexpr auto begin() const { return std::begin(internal::enum_values<EnumType>); }
    constexpr auto end() const { return std::end(internal::enum_values<EnumType>); }
};

}  // namespace ndk
#endif
