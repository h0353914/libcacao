// 共用的錯誤碼跟少數幾個跨檔案宣告。原版的錯誤碼是一堆寫死的十六進位
// 負數字面值（例如 -0x67、-0x6a、-0x6f...），這裡先只挑幾個最常見、
// 語意最確定的取名字，其餘保留數字字面值——強行幫每一個都取名字，
// 名字很可能猜錯語意，數字至少不會騙人。
//
// 已知常見碼（從各函式的用法反推，不是原始碼裡就有這些名字）：
//   -0x67 (=-103) invalid argument
//   -0x6a (=-106) unsupported
//   -0x6f (=-111) internal/backend failure（hw_get_module、gralloc 呼叫失敗）
//   -0x91..-0x9a  I/O / 記憶體相關失敗（sendmsg/recvmsg/mmap/malloc...）

#pragma once

#include <stdint.h>

constexpr int CAMMW_ERR_INVALID_ARG = -0x67;
constexpr int CAMMW_ERR_UNSUPPORTED = -0x6a;
constexpr int CAMMW_ERR_FAILED = -0x6f;
constexpr int CAMMW_ERR_ALREADY_RUNNING = -0x6b;

struct cammw_util_context_t;
extern "C" cammw_util_context_t *cammw_util_get_context(void);
extern "C" int cammw_util_init(void);

// bufmgr/import_buf/free_buf/alloc_buf 用的 24-byte「簡單」影像描述子。
// 用 word 陣列表示（跟 cammw_buf_t 一樣的理由：這顆結構被好幾個檔案
// 用不同角度存取，寧可 1:1 對應反編譯的 word-index 存取，不猜語意名字）。
//   w[0]=base/mapped_addr, w[1]=size, w[2]=fd, w[3]=fd_metadata/descriptor,
//   w[4]=type(0=ashmem 1=gralloc 2=unsupported 3=heap), w[5] 低位元組=cleanup_flag
struct cammw_util_image_buffer_t {
  int32_t w[6];
};
static_assert(sizeof(cammw_util_image_buffer_t) == 24, "bufmgr_open 每筆前進 6 個 word");
