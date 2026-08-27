// cammw_util 訊息層（msg / msg_server）用到的結構重建
//
// 這次逐行對照 kong 這輪重新產生的 ../kong-output/decompiled.c，不是憑
// 記憶抄——涵蓋以下函式全部讀過一遍：
//   cammw_util_msg_cli_open           @ 0x127f4
//   cammw_util_msg_create_session     @ 0x12904
//   cammw_util_msg_srv_open           @ 0x12b18
//   cammw_util_msg_server_thread      @ 0x12c68
//   cammw_util_msg_srv_create_session @ 0x12dc4
//   cammw_util_msg_send_internal      @ 0x12fa8
//   cammw_util_msg_sendrecv           @ 0x130ec
//   cammw_util_msg_wait               @ 0x1314c
//   cammw_util_msg_session_thread     @ 0x1566c
//   cammw_util_msg_signal             @ 0x159d0
//   cammw_util_msg_send               @ 0x12f70
//   cammw_util_msg_delete_session     @ 0x12ace
//   cammw_util_msg_srv_close          @ 0x12d60
//   cammw_util_msg_srv_delete_session @ 0x12f18
//
// 有意思的發現：decompiler 把 cammw_util_msg_session_t 推斷成「1 byte」
// 的型別，純粹是因為程式碼裡混用了 byte 級（param_1[0x74]）跟 word 級
// （param_1 + 0x1a，word-index，實際 = byte 0x68）的存取。這裡不管
// decompiler 怎麼標型別，一律換算回絕對 byte offset，用 static_assert
// 鎖死。

#pragma once

#include <stdint.h>
#include <pthread.h>

#include "cammw_thread.h"

// ---------------------------------------------------------------------
// cammw_util_msg_t —— 上層呼叫端傳給 msg_send_internal 的描述子。
// 逐位元組核對過（send_internal 用 `*param_4`=size、`param_4[1]`=payload
// 指標、`param_4[2]`=fd_count、`param_4+3`起=fds[]；msg_wait/msg_signal
// 用 `param_3[1]`=payload 指標 也對得上）。
struct cammw_util_msg_t {
  uint32_t size;      // +0x00
  void *payload;       // +0x04
  uint32_t fd_count;   // +0x08  (0..10)
  int fds[10];          // +0x0c
};
static_assert(sizeof(cammw_util_msg_t) == 0x34, "size+payload+fd_count+fds[10]");

// wire 上實際送出的 16-byte header（第一個 iovec），第二個 iovec 才是
// payload 本體。四個欄位跟 send_internal / session_thread 收發兩邊都對得上：
//   word0 = flags（0=notify 1=request 2=startup_done通知 3=reply）
//   word1 = fd_count（只有 flags 是 0/1 時才帶，2/3 固定送 0）
//   word2 = payload size
//   word3 = result code（在 flags==3 的回覆訊息才有意義，其餘送 0）
struct cammw_msg_wire_header {
  uint32_t flags;
  uint32_t fd_count;
  uint32_t payload_size;
  uint32_t result;
};
static_assert(sizeof(cammw_msg_wire_header) == 0x10, "sendmsg 的第一個 iovec 固定 16 bytes");

enum cammw_msg_flags : uint32_t {
  kCammwMsgNotify = 0,       // 送出去不等回覆
  kCammwMsgRequest = 1,      // 送出去等回覆（session_thread 收到後呼叫 callback 並自動回 3）
  kCammwMsgStartupDone = 2,  // session_thread 一啟動就送給對面，msg_create_session 等這個
  kCammwMsgReply = 3,        // 對 Request 的回覆
};

// 前向宣告：cammw_msg_server_context 的 session_table 要用到，但
// cammw_util_msg_session_t 本身定義在後面（它也沒有反過來需要
// cammw_msg_server_context，純粹是宣告順序問題）。
struct cammw_util_msg_session_t;

// ---------------------------------------------------------------------
// cammw_msg_server_context —— msg_srv_open 建立、msg_server_thread 使用、
// msg_srv_create_session/srv_close/srv_delete_session 操作的伺服端物件。
// calloc(1, 0xf4) —— 244 bytes，跟這裡的 sizeof 對上。
struct cammw_msg_server_context {
  int fd;                              // +0x00  listen() 成功後填，初值 -1
  cammw_thread_context accept_thread;  // +0x04  accept loop 執行緒
  pthread_mutex_t session_table_mutex; // +0x68
  int session_count;                   // +0x6c
  cammw_util_msg_session_t *session_table[4];  // +0x70 .. +0x80
  // socket 路徑（srv_close 用來 unlink()）。大小抓 acStack_90 那個
  // 108-byte 緩衝一致的量；确切在 srv_open 裡的寫入點這輪沒有重新核對到
  // 每一行，但 unlink((char*)(param_1+0x20)) 這行是逐行核對過的，
  // 0x20*4=0x80 沒錯，open() 端要把解析出來的路徑複製進來。
  char socket_path[0xec - 0x80];       // +0x80 .. +0xec
  void *connection_callback_context;   // +0xec
  // connection_callback(int accepted_fd, void *context)
  void (*connection_callback)(int, void *);  // +0xf0
};
static_assert(sizeof(cammw_msg_server_context) == 0xf4, "calloc(1, 0xf4)");
static_assert(offsetof(cammw_msg_server_context, session_table_mutex) == 0x68, "param_1+0x1a (word) = 0x68");
static_assert(offsetof(cammw_msg_server_context, session_count) == 0x6c, "param_1[0x1b]");
static_assert(offsetof(cammw_msg_server_context, session_table) == 0x70, "param_1+0x1c (word) = 0x70");
static_assert(offsetof(cammw_msg_server_context, connection_callback_context) == 0xec, "strd r6,r5,[r4,#0xec]");
static_assert(offsetof(cammw_msg_server_context, connection_callback) == 0xf0, "ldrd r1,r2,[r8,#0xec]/blx r2");

// ---------------------------------------------------------------------
// cammw_util_msg_session_t —— 一個已連線 session 的狀態。
// calloc(1, 0x18c4)，實際用到的最後一個已知欄位在 +0x1894，中間留白到
// 0x18c4 對齊原本的配置大小（多出來的 0x2c bytes 這輪沒有繼續往下查）。
struct cammw_util_msg_session_t {
  int fd;                              // +0x00
  cammw_thread_context recv_thread;    // +0x04  這個 session 自己的收訊執行緒
  pthread_mutex_t send_mutex;          // +0x68  msg_send/msg_sendrecv 用
  pthread_mutex_t wait_mutex;          // +0x6c  msg_wait/msg_signal/msg_sendrecv 用
  pthread_cond_t wait_cond;            // +0x70
  uint8_t disconnected;                // +0x74  EOF 時設 1（msg_wait 迴圈的離開條件之一）
  uint8_t has_result;                  // +0x75  收到 2/3 類回覆時設 1
  uint8_t reserved_76[2];              // +0x76  對齊
  uint32_t result_a;                   // +0x78  response_selector==0 的結果碼
  uint32_t result_b;                   // +0x7c  response_selector==1 的結果碼
  uint32_t recv_size;                  // +0x80  msg_wait/msg_signal 存取的「已收到大小」
  uint8_t recv_payload[0xc00];         // +0x84  .. +0xc84  request/reply 走這條
  uint8_t recv_payload_big[0xc00];     // +0xc84 .. +0x1884  session_thread 收 notify/request 走這條
  void *user_ctx;                      // +0x1884
  // message_callback(session, msg_t*, user_ctx) -> flags（0/1/2/3，決定
  // session_thread 怎麼處理：0=已處理完不用回、1=回傳值當結果碼自動回 3）
  uint32_t (*message_callback)(void *session, cammw_util_msg_t *msg, void *ctx);  // +0x1888
  void (*disconnect_callback)(void *session, void *ctx);                          // +0x188c
  uint32_t private_data_size;          // +0x1890
  void *private_data;                  // +0x1894  create_session 收到 (size,ptr) 就 calloc+memcpy 一份
  uint8_t reserved_tail[0x18c4 - 0x1898];  // 補到原本 calloc 的大小
};
static_assert(sizeof(cammw_util_msg_session_t) == 0x18c4, "calloc(1, 0x18c4)");
static_assert(offsetof(cammw_util_msg_session_t, send_mutex) == 0x68, "msg_send: param_1+0x1a(word)");
static_assert(offsetof(cammw_util_msg_session_t, wait_mutex) == 0x6c, "msg_wait/sendrecv: param_1+0x6c");
static_assert(offsetof(cammw_util_msg_session_t, wait_cond) == 0x70, "pthread_cond_wait 對象");
static_assert(offsetof(cammw_util_msg_session_t, disconnected) == 0x74, "EOF: *(undefined1*)(param_1+0x74)=1");
static_assert(offsetof(cammw_util_msg_session_t, has_result) == 0x75, "param_1[0x75]=1");
static_assert(offsetof(cammw_util_msg_session_t, result_a) == 0x78, "response_selector==0");
static_assert(offsetof(cammw_util_msg_session_t, result_b) == 0x7c, "response_selector==1");
static_assert(offsetof(cammw_util_msg_session_t, recv_size) == 0x80, "*(uint*)(param_1+0x80)");
static_assert(offsetof(cammw_util_msg_session_t, recv_payload) == 0x84, "__aeabi_memcpy(..., param_1+0x84, ...)");
static_assert(offsetof(cammw_util_msg_session_t, recv_payload_big) == 0xc84, "local_12c = param_1+0xc84");
static_assert(offsetof(cammw_util_msg_session_t, user_ctx) == 0x1884, "*puVar5, puVar5=param_1+0x1884");
static_assert(offsetof(cammw_util_msg_session_t, message_callback) == 0x1888, "(**(code**)(param_1+0x1888))(...)");
static_assert(offsetof(cammw_util_msg_session_t, disconnect_callback) == 0x188c, "(**(code**)(param_1+0x188c))(...)");
static_assert(offsetof(cammw_util_msg_session_t, private_data_size) == 0x1890, "param_7==0||param_8==0 檢查對象");
static_assert(offsetof(cammw_util_msg_session_t, private_data) == 0x1894, "free(*(void**)(param_1+0x1894))");
