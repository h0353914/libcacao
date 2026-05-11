/*
 * aeabi_helpers.c — ARM EABI & GNU 64-bit 算術輔助函式
 *
 * 原始 libcacao_process_ctrl_gateway.so (GCC 編譯) 靜態包含這些 libgcc 函式。
 * 本檔案提供等效實作，供版本腳本匯出，與原始 .so 保持符號相容性。
 *
 * 注意：
 *   - __aeabi_uldivmod 在連結後已以局部符號存在，版本腳本可直接匯出；
 *     但為安全起見此處也提供定義（注意跳過重複定義問題）。
 *   - __divdi3 / __udivdi3 使用 / 運算子，Clang ARM32 會呼叫 __aeabi_ldivmod /
 *     __aeabi_uldivmod (不遞迴)。
 *   - 移位函式以 __attribute__((noinline, used)) 確保符號存在。
 */

#include <stdint.h>

typedef long long           s64;
typedef unsigned long long  u64;

/* ── 移位函式 ── */

__attribute__((noinline, used))
s64 __aeabi_llsl(s64 val, int n)
{
    return val << n;
}

__attribute__((noinline, used))
u64 __aeabi_llsr(u64 val, int n)
{
    return val >> n;
}

__attribute__((noinline, used))
s64 __ashldi3(s64 val, int n)
{
    return val << n;
}

__attribute__((noinline, used))
u64 __lshrdi3(u64 val, int n)
{
    return val >> n;
}

/* ── 除法函式（Clang ARM32 會使用 __aeabi_ldivmod / __aeabi_uldivmod，不遞迴） ── */

__attribute__((noinline, used))
s64 __divdi3(s64 a, s64 b)
{
    return a / b;
}

__attribute__((noinline, used))
u64 __udivdi3(u64 a, u64 b)
{
    return a / b;
}

/* ── GNU 輔助函式（GCC 介面：返回商，將餘數存入 *rem） ── */

__attribute__((noinline, used))
s64 __gnu_ldivmod_helper(s64 n, s64 d, s64 *rem)
{
    *rem = n % d;
    return n / d;
}

__attribute__((noinline, used))
u64 __gnu_uldivmod_helper(u64 n, u64 d, u64 *rem)
{
    *rem = n % d;
    return n / d;
}
