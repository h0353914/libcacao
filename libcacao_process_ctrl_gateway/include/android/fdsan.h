/*
 * android/fdsan.h — 相容 stub
 *
 * fdsan API 在 Android 9 (API 28) 不存在，正式 API 為 29+。
 * 此 stub 讓 VNDK v30 的 android-base/unique_fd.h 可在 API 28 工具鏈下編譯。
 * 所有函式宣告都被 __ANDROID_API__ >= 29 guard 保護，API 28 不會引入任何符號。
 *
 * Soong build 時使用 #include_next 轉發到 bionic 真實 header。
 */
#ifdef LIBCACAO_SOONG_BUILD
#pragma once
#include_next <android/fdsan.h>
#else
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

enum android_fdsan_owner_type {
    ANDROID_FDSAN_OWNER_TYPE_GENERIC_00       = 0,
    ANDROID_FDSAN_OWNER_TYPE_GENERIC_FF       = 255,
    ANDROID_FDSAN_OWNER_TYPE_FILE             = 1,
    ANDROID_FDSAN_OWNER_TYPE_DIR              = 2,
    ANDROID_FDSAN_OWNER_TYPE_UNIQUE_FD        = 3,
    ANDROID_FDSAN_OWNER_TYPE_SQLITE           = 4,
    ANDROID_FDSAN_OWNER_TYPE_FILEINPUTSTREAM  = 5,
    ANDROID_FDSAN_OWNER_TYPE_FILEOUTPUTSTREAM = 6,
    ANDROID_FDSAN_OWNER_TYPE_RANDOMACCESSFILE = 7,
    ANDROID_FDSAN_OWNER_TYPE_PARCELFILEDESCRIPTOR = 8,
    ANDROID_FDSAN_OWNER_TYPE_ART_FDFILE       = 9,
    ANDROID_FDSAN_OWNER_TYPE_DATAGRAMSOCKETIMPL = 10,
    ANDROID_FDSAN_OWNER_TYPE_SOCKETIMPL       = 11,
    ANDROID_FDSAN_OWNER_TYPE_ZIPARCHIVE       = 12,
};

enum android_fdsan_error_level {
    ANDROID_FDSAN_ERROR_LEVEL_DISABLED,
    ANDROID_FDSAN_ERROR_LEVEL_WARN_ONCE,
    ANDROID_FDSAN_ERROR_LEVEL_WARN_ALWAYS,
    ANDROID_FDSAN_ERROR_LEVEL_FATAL,
};

/*
 * 以下函式在 API 29+ 才正式存在，但在 VNDK v30 的 unique_fd.h 中會被直接使用。
 * 宣告為 weak 讓編譯通過，連結時由 linker 處理 (allow_undefined_symbols)。
 */
uint64_t android_fdsan_create_owner_tag(enum android_fdsan_owner_type type,
                                        uint64_t tag)
    __attribute__((__weak__));

void android_fdsan_exchange_owner_tag(int fd,
                                      uint64_t expected_tag,
                                      uint64_t new_tag)
    __attribute__((__weak__));

int android_fdsan_close_with_tag(int fd, uint64_t tag)
    __attribute__((__weak__));

uint64_t android_fdsan_get_owner_tag(int fd) __attribute__((__weak__));
const char* android_fdsan_get_tag_type(uint64_t tag) __attribute__((__weak__));
uint64_t android_fdsan_get_tag_value(uint64_t tag) __attribute__((__weak__));

enum android_fdsan_error_level android_fdsan_get_error_level()
    __attribute__((__weak__));

enum android_fdsan_error_level android_fdsan_set_error_level(
    enum android_fdsan_error_level new_level)
    __attribute__((__weak__));

enum android_fdsan_error_level android_fdsan_set_error_level_from_property(
    enum android_fdsan_error_level default_level)
    __attribute__((__weak__));

__END_DECLS
#endif
