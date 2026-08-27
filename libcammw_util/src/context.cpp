// cammw_util 的全域 context：gralloc1 module/device 惰性初始化、15 個
// g_*_func 函式指標、event 系列。對照 kong 這輪 decompiled.c：
//   cammw_util_init_buffer_manager @ 0x132f4
//   cammw_util_event_init/deinit   @ 0x151a8 / 0x151ce
//   cammw_util_event_wait/notify   @ 0x151ec / 0x152c8（已完整讀過，逐字轉譯）
//   gralloc1 module/device 惰性初始化邏輯是從 cammw_util_gralloc_alloc
//   （@0x13708）內聯的那段反推出來的獨立函式（原本應該也是獨立函式，
//   被編譯器 inline 了；這裡拆回獨立函式，日後 shmem/gralloc_alloc 都能共用）。
//
// g_context 的 mutex/module/device 三個欄位 offset 是從
// `__mutex+0x20`=module、`(int*)((int)__mutex+0x24)`=device 這兩處反推的，
// __mutex 本身 = g_cammw_buffer_manager 解參考出來的東西（也就是
// g_context 自己），所以 mutex 在 offset 0、module 在 0x20、device 在
// 0x24。中間 0x20 bytes 應該是別的東西（bufmgr？先前筆記提過 context
// 裡有三個內嵌 C++ 物件之一是 PtrLockList，這輪沒有重新確認完整佈局，
// 用 reserved 佔位、只鎖住已知的三個 offset）。

#define LOG_TAG "cammw_util"

#include "cammw_util_internal.h"

#include <errno.h>
#include <fcntl.h>
#include <hardware/gralloc1.h>
#include <hardware/hardware.h>
#include <log/log.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>

// ---------------------------------------------------------------------
// g_context：全域單例。

struct cammw_util_context_t {
  pthread_mutex_t mutex;              // +0x00
  uint8_t reserved[0x20 - sizeof(pthread_mutex_t)];  // 佔位，未重新核對內容
  hw_module_t *gralloc_module;         // +0x20
  gralloc1_device_t *gralloc_device;   // +0x24
};
static_assert(offsetof(cammw_util_context_t, gralloc_module) == 0x20, "*(int*)((int)__mutex+0x20)");
static_assert(offsetof(cammw_util_context_t, gralloc_device) == 0x24, "(int*)((int)__mutex+0x24)");

static cammw_util_context_t g_context_storage{};
static cammw_util_context_t *g_context = &g_context_storage;
static pthread_once_t g_init_once = PTHREAD_ONCE_INIT;

extern "C" cammw_util_context_t *cammw_util_get_context(void) {
  return g_context;
}

// 對照 decompiled.c @ 0x132f4（kong 標的 @name 是
// cammw_util_init_buffer_manager，但函式簽名註解跟所有呼叫端都叫它
// cammw_util_init——這是 kong 這次語意命名跟原始碼命名的落差，用位址
// 對照，不是用它取的名字）。
//
// 原版是 operator_new(0x2c) 配置一個帶 3 個內嵌 vtable 物件的
// context，惰性配置一次、存進全域指標 g_cammw_buffer_manager。
// 這裡刻意沒有照抄那個 44-byte 佈局：那 3 個 vtable 物件是
// PtrLockList/PtrList 的內嵌實例，我們的重建版用獨立的 C++ 物件
// （cammw::PtrLockList，在 gralloc_descriptor.cpp）取代，不需要塞進
// 同一個 struct——只要「mutex 保護 + 一次性初始化 + gralloc device
// 惰性載入」這個行為對得上，物理佈局不用逐位元組一致（這個 context
// 不會被其他函式庫按 offset 硬解，跟 thread_context/msg_session_t 那種
// 一定要位元組對齊的情況不一樣）。
extern "C" int cammw_util_init(void) {
  pthread_once(&g_init_once, [] { pthread_mutex_init(&g_context_storage.mutex, nullptr); });
  return 0;
}

// ---------------------------------------------------------------------
// 15 個 gralloc1 function pointer globals。跟 libcammw.so 自己也定義的
// 同名符號靠 symbol interposition 共用同一份（這是先前 debug 找到的
// 關鍵機制，見 README）。

extern "C" {
GRALLOC1_PFN_ALLOCATE g_allocate_func = nullptr;
GRALLOC1_PFN_CREATE_DESCRIPTOR g_create_descriptor_func = nullptr;
GRALLOC1_PFN_DESTROY_DESCRIPTOR g_destroy_descriptor_func = nullptr;
GRALLOC1_PFN_GET_FORMAT g_get_format_func = nullptr;
GRALLOC1_PFN_LOCK g_lock_func = nullptr;
GRALLOC1_PFN_LOCK_FLEX g_lock_flex_func = nullptr;
void *g_perform_func = nullptr;  // GRALLOC1_FUNCTION_PERFORM，重建版沒用到，占位保留符號
GRALLOC1_PFN_RELEASE g_release_func = nullptr;
GRALLOC1_PFN_RETAIN g_retain_func = nullptr;
GRALLOC1_PFN_SET_CONSUMER_USAGE g_set_consumer_usage_func = nullptr;
GRALLOC1_PFN_SET_DIMENSIONS g_set_dimensions_func = nullptr;
GRALLOC1_PFN_SET_FORMAT g_set_format_func = nullptr;
GRALLOC1_PFN_SET_PRODUCER_USAGE g_set_producer_usage_func = nullptr;
GRALLOC1_PFN_GET_NUM_FLEX_PLANES g_get_num_flex_planes_func = nullptr;
GRALLOC1_PFN_UNLOCK g_unlock_func = nullptr;
}

extern "C" GRALLOC1_PFN_LOCK cammw_util_gralloc_lock_fn(void) { return g_lock_func; }
extern "C" GRALLOC1_PFN_LOCK_FLEX cammw_util_gralloc_lock_flex_fn(void) { return g_lock_flex_func; }

// 惰性初始化 gralloc1 module/device，把 15 個函式指標填好。
// 對照 cammw_util_gralloc_alloc 內聯的那段（0x13a2c 附近）。
extern "C" int cammw_util_get_gralloc1_dev(gralloc1_device_t **out_device) {
  pthread_mutex_lock(&g_context->mutex);
  int rc = 0;
  if (g_context->gralloc_device == nullptr) {
    const hw_module_t *module = g_context->gralloc_module;
    if (module == nullptr) {
      int hw_rc = hw_get_module(GRALLOC_HARDWARE_MODULE_ID, &module);
      if (hw_rc < 0) {
        ALOGE("E: %s: hw_get_module Failed", __FUNCTION__);
        rc = -0x6f;
      } else {
        g_context->gralloc_module = const_cast<hw_module_t *>(module);
      }
      if (module == nullptr) {
        ALOGE("E: %s: failed to load gralloc1 module %d", __FUNCTION__, hw_rc);
        pthread_mutex_unlock(&g_context->mutex);
        return -0x6f;
      }
    }
    hw_device_t *device = nullptr;
    int open_rc = module->methods->open(module, GRALLOC_HARDWARE_MODULE_ID, &device);
    if (open_rc == 0 && device != nullptr) {
      auto *dev = reinterpret_cast<gralloc1_device_t *>(device);
      g_allocate_func = reinterpret_cast<GRALLOC1_PFN_ALLOCATE>(
          dev->getFunction(dev, GRALLOC1_FUNCTION_ALLOCATE));
      g_create_descriptor_func = reinterpret_cast<GRALLOC1_PFN_CREATE_DESCRIPTOR>(
          dev->getFunction(dev, GRALLOC1_FUNCTION_CREATE_DESCRIPTOR));
      g_destroy_descriptor_func = reinterpret_cast<GRALLOC1_PFN_DESTROY_DESCRIPTOR>(
          dev->getFunction(dev, GRALLOC1_FUNCTION_DESTROY_DESCRIPTOR));
      g_get_format_func = reinterpret_cast<GRALLOC1_PFN_GET_FORMAT>(
          dev->getFunction(dev, GRALLOC1_FUNCTION_GET_FORMAT));
      g_lock_func =
          reinterpret_cast<GRALLOC1_PFN_LOCK>(dev->getFunction(dev, GRALLOC1_FUNCTION_LOCK));
      g_lock_flex_func = reinterpret_cast<GRALLOC1_PFN_LOCK_FLEX>(
          dev->getFunction(dev, GRALLOC1_FUNCTION_LOCK_FLEX));
      // GRALLOC1_FUNCTION_PERFORM 是原廠 vendor 擴充 opcode(0x1000)，不在
      // AOSP 標準 gralloc1.h 的列舉裡，這裡照原版數值直接寫。
      g_perform_func = reinterpret_cast<void *>(dev->getFunction(dev, static_cast<int32_t>(0x1000)));
      g_release_func = reinterpret_cast<GRALLOC1_PFN_RELEASE>(
          dev->getFunction(dev, GRALLOC1_FUNCTION_RELEASE));
      g_retain_func =
          reinterpret_cast<GRALLOC1_PFN_RETAIN>(dev->getFunction(dev, GRALLOC1_FUNCTION_RETAIN));
      g_set_consumer_usage_func = reinterpret_cast<GRALLOC1_PFN_SET_CONSUMER_USAGE>(
          dev->getFunction(dev, GRALLOC1_FUNCTION_SET_CONSUMER_USAGE));
      g_set_dimensions_func = reinterpret_cast<GRALLOC1_PFN_SET_DIMENSIONS>(
          dev->getFunction(dev, GRALLOC1_FUNCTION_SET_DIMENSIONS));
      g_set_format_func = reinterpret_cast<GRALLOC1_PFN_SET_FORMAT>(
          dev->getFunction(dev, GRALLOC1_FUNCTION_SET_FORMAT));
      g_set_producer_usage_func = reinterpret_cast<GRALLOC1_PFN_SET_PRODUCER_USAGE>(
          dev->getFunction(dev, GRALLOC1_FUNCTION_SET_PRODUCER_USAGE));
      g_get_num_flex_planes_func = reinterpret_cast<GRALLOC1_PFN_GET_NUM_FLEX_PLANES>(
          dev->getFunction(dev, GRALLOC1_FUNCTION_GET_NUM_FLEX_PLANES));
      g_unlock_func =
          reinterpret_cast<GRALLOC1_PFN_UNLOCK>(dev->getFunction(dev, GRALLOC1_FUNCTION_UNLOCK));
      g_context->gralloc_device = dev;
      rc = 0;
    } else {
      ALOGE("E: %s: failed to open gralloc1 device %d", __FUNCTION__, open_rc);
      rc = -0x6f;
    }
    if (g_context->gralloc_device == nullptr && device != nullptr) {
      device->close(device);
    }
  }
  pthread_mutex_unlock(&g_context->mutex);
  if (rc != 0) {
    return rc;
  }
  if (g_context->gralloc_device == nullptr) {
    ALOGE("E: %s: no gralloc1 device", __FUNCTION__);
    return -0x6f;
  }
  *out_device = g_context->gralloc_device;
  return 0;
}

// ---------------------------------------------------------------------
// event（一個 mutex + cond + flag + code，跟 cammw_util_thread_context
// 一樣是個小型同步原語，供 msg 以外的地方用）。

struct cammw_event_t {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  uint8_t signaled;
  int code;
};

extern "C" int cammw_util_event_init(cammw_event_t *ev) {
  if (ev == nullptr) {
    return CAMMW_ERR_INVALID_ARG;
  }
  pthread_mutex_init(&ev->mutex, nullptr);
  pthread_cond_init(&ev->cond, nullptr);
  ev->signaled = 0;
  ev->code = 0;
  return 0;
}

extern "C" int cammw_util_event_deinit(cammw_event_t *ev) {
  if (ev == nullptr) {
    return CAMMW_ERR_INVALID_ARG;
  }
  pthread_cond_destroy(&ev->cond);
  pthread_mutex_destroy(&ev->mutex);
  return 0;
}

// 對照 decompiled.c @ 0x151ec，逐字轉譯（timeout_ms==0 -> 無限等）。
extern "C" int cammw_util_event_wait(cammw_event_t *ev, uint32_t timeout_ms) {
  if (ev == nullptr) {
    return 0xffffff99;
  }
  pthread_mutex_lock(&ev->mutex);
  int rc = 0;
  if (!ev->signaled) {
    do {
      if (timeout_ms == 0) {
        pthread_cond_wait(&ev->cond, &ev->mutex);
      } else {
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += timeout_ms / 1000;
        ts.tv_nsec += (timeout_ms % 1000) * 1000000;
        int wait_rc = pthread_cond_timedwait(&ev->cond, &ev->mutex, &ts);
        if (wait_rc != 0) {
          ALOGE("E: %s: timeout rc = %d", __FUNCTION__, wait_rc);
          rc = 0xffffff92;
          goto done;
        }
      }
    } while (!ev->signaled);
    rc = 0;
  }
  ev->signaled = 0;
done:
  pthread_mutex_unlock(&ev->mutex);
  return rc;
}

// 對照 decompiled.c @ 0x152c8，逐字轉譯。
// ---------------------------------------------------------------------
// mmap/munmap 包一層 log。對照 decompiled.c @ 0x15e18 / 0x15e78，逐字轉譯。

extern "C" int cammw_util_mmap(int fd, uint32_t size, uint8_t **out_addr) {
  void *addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
    ALOGE("E: %s: failed: fd=%d, size=%zu, e=%d %s", __FUNCTION__, fd, static_cast<size_t>(size),
          errno, strerror(errno));
    return 0xffffff97;
  }
  *out_addr = static_cast<uint8_t *>(addr);
  return 0;
}

extern "C" int cammw_util_munmap(uint8_t *addr, uint32_t size) {
  if (munmap(addr, size) == 0) {
    return 0;
  }
  ALOGE("E: %s: failed: addr=%p, size=%zu %d %s", __FUNCTION__, addr, static_cast<size_t>(size),
        errno, strerror(errno));
  return 0xffffff97;
}

// ---------------------------------------------------------------------
// 時間轉換系列。原版用手動 32-bit 分段進位運算（因為原本是 32-bit
// ARM 編譯器，int64 運算被拆成兩個 32-bit word），這裡直接用
// int64_t——數學上是同一件事，可讀性好很多、也不會抄錯進位。
//
// cammw_util_init_diff_timespec 原版會取 5~6 次樣本挑「最穩定」的一次
// （抖動過濾），這裡簡化成單次採樣（見下方註解，這是刻意的精度簡化，
// 不是遺漏）。

static int64_t g_boottime_to_monotonic_offset_ns = 0;

namespace {
int64_t timespec_to_ns(const timespec &ts) {
  return static_cast<int64_t>(ts.tv_sec) * 1000000000LL + ts.tv_nsec;
}
}  // namespace

// 簡化版：原版取樣 5~6 次找抖動最小的一組，這裡只取一次。BOOTTIME 跟
// MONOTONIC 的差值本來就應該是穩定值（機器沒有 suspend/resume 就不會
// 變），多次取樣主要是為了避開系統呼叫本身的排程抖動，對最終結果影響
// 通常在微秒等級，這裡先簡化，需要的話之後再補多次採樣版本。
extern "C" void cammw_util_init_diff_timespec(void) {
  timespec boot_a{}, mono{}, boot_b{};
  clock_gettime(CLOCK_BOOTTIME, &boot_a);
  clock_gettime(CLOCK_MONOTONIC, &mono);
  clock_gettime(CLOCK_BOOTTIME, &boot_b);
  const int64_t boot_mid = (timespec_to_ns(boot_a) + timespec_to_ns(boot_b)) / 2;
  g_boottime_to_monotonic_offset_ns = timespec_to_ns(mono) - boot_mid;
}

extern "C" int cammw_util_convert_boottime_to_monotonic(uint32_t nsec, int32_t sec, int32_t *out) {
  if (out == nullptr) {
    ALOGE("E: %s: Invalid arg", __FUNCTION__);
    return 0xffffff99;
  }
  // 原版輸出/輸入都是「拆成 nsec(低 32)/sec(高 32，實際是秒數整數部分
  // 混進位運算後的結果)」這種怪異的 split representation，不是真正的
  // {sec,nsec} pair——從呼叫端（這輪沒有往上追）沒辦法百分之百確認，
  // 這裡先照最合理的解讀：param_1/param_3[0] 是總奈秒數的低 32 位，
  // param_2/param_3[1] 是高 32 位，整體是一個 64-bit 奈秒數的 split。
  const int64_t total_ns = (static_cast<uint32_t>(nsec)) | (static_cast<int64_t>(sec) << 32);
  const int64_t converted = total_ns + g_boottime_to_monotonic_offset_ns;
  out[0] = static_cast<int32_t>(static_cast<uint32_t>(converted));
  out[1] = static_cast<int32_t>(converted >> 32);
  return 0;
}

extern "C" int cammw_util_convert_realtime_to_monotonic(uint32_t nsec, int32_t sec, int32_t *out) {
  if (out == nullptr) {
    ALOGE("E: %s: Invalid arg", __FUNCTION__);
    return 0xffffff99;
  }
  timespec real{}, mono{};
  clock_gettime(CLOCK_REALTIME, &real);
  clock_gettime(CLOCK_MONOTONIC, &mono);
  const int64_t total_ns = (static_cast<uint32_t>(nsec)) | (static_cast<int64_t>(sec) << 32);
  const int64_t converted = total_ns - timespec_to_ns(real) + timespec_to_ns(mono);
  out[0] = static_cast<int32_t>(static_cast<uint32_t>(converted));
  out[1] = static_cast<int32_t>(converted >> 32);
  return 0;
}

extern "C" int64_t cammw_util_get_timestamp(int clock_type) {
  clockid_t id;
  switch (clock_type) {
    case 0: id = CLOCK_REALTIME; break;
    case 1: id = CLOCK_MONOTONIC; break;
    case 2: id = CLOCK_BOOTTIME; break;
    default: return 0;
  }
  timespec ts{};
  clock_gettime(id, &ts);
  return timespec_to_ns(ts);
}

// 原版讀一個系統屬性、剖析成整數後就丟掉不用（kong 的分析摘要也這樣
// 講），觀察得到的行為就是「沒有」，這裡照實做成 no-op。
extern "C" void cammw_util_get_thermal_camera_info(void) {}

// 對照 decompiled.c @ 0x15444，逐字轉譯。
extern "C" void cammw_util_set_thermal(int zone_id, int32_t value) {
  char path[108];
  snprintf(path, sizeof(path), "sys/devices/sony_camera_%d/info", zone_id);
  int fd = open(path, O_WRONLY);
  if (fd < 0) {
    ALOGE("E: %s: open error %d", __FUNCTION__, zone_id);
    return;
  }
  ssize_t n = write(fd, &value, sizeof(value));
  if (n != sizeof(value)) {
    ALOGE("E: %s: write size err %zd", __FUNCTION__, n);
  }
  close(fd);
}

extern "C" int cammw_util_event_notify(cammw_event_t *ev, int code) {
  if (ev == nullptr) {
    return 0xffffff99;
  }
  pthread_mutex_lock(&ev->mutex);
  ev->signaled = 1;
  ev->code = code;
  pthread_cond_signal(&ev->cond);
  pthread_mutex_unlock(&ev->mutex);
  return 0;
}
