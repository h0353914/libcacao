/*
 * aeabi_ldivmod.S — ARM EABI __aeabi_ldivmod (有號 64-bit 除法)
 *
 * 原始 libcacao_process_ctrl_gateway.so (GCC 編譯) 靜態包含來自 libgcc.a 的
 * __aeabi_ldivmod。本檔案提供等效實作，委派給 __aeabi_uldivmod 處理無號部分，
 * 除數為 0 時依 ARM EABI 規範呼叫 __aeabi_ldiv0（本 .so 內定義，回傳 0）。
 *
 * 介面 (ARM EABI):
 *   Input:  r0:r1 = numerator  (lo:hi)
 *           r2:r3 = denominator (lo:hi)
 *   Output: r0:r1 = quotient   (lo:hi)
 *           r2:r3 = remainder  (lo:hi)
 *
 * 64-bit negation in Thumb-2 (no RSC immediate):
 *   rsbs rd_lo, rm_lo, #0   @ lo = 0 - lo, sets carry
 *   mvn  rd_hi, rm_hi       @ hi = ~hi (no flag change)
 *   adcs rd_hi, rd_hi, #0   @ hi += carry → ~hi + carry = correct 2's complement
 */

    .syntax unified
    .thumb
    .text
    .align 2

    .global __aeabi_ldivmod
    .hidden __aeabi_ldivmod
    .type   __aeabi_ldivmod, %function
    .thumb_func

__aeabi_ldivmod:
    @ 檢查除數是否為 0：d_lo | d_hi
    orr     r12, r2, r3
    cmp     r12, #0
    beq     .Lldivmod_zero

    push    {r4, r5, r6, r7, lr}

    @ 儲存符號資訊
    @ r4 = n_hi（決定餘數符號）
    @ r5 = n_hi ^ d_hi（決定商符號）
    mov     r4, r1
    eor     r5, r1, r3

    @ 若被除數為負，取絕對值
    tst     r1, #0x80000000
    beq     .Lldivmod_pos_n
    rsbs    r0, r0, #0
    mvn     r1, r1
    adcs    r1, r1, #0

.Lldivmod_pos_n:
    @ 若除數為負，取絕對值
    tst     r3, #0x80000000
    beq     .Lldivmod_pos_d
    rsbs    r2, r2, #0
    mvn     r3, r3
    adcs    r3, r3, #0

.Lldivmod_pos_d:
    @ 呼叫無號除法：__aeabi_uldivmod(|n|, |d|)
    bl      __aeabi_uldivmod
    @ r0:r1 = quotient，r2:r3 = remainder

    @ 根據原始符號調整商的正負
    tst     r5, #0x80000000
    beq     .Lldivmod_quot_done
    rsbs    r0, r0, #0
    mvn     r1, r1
    adcs    r1, r1, #0

.Lldivmod_quot_done:
    @ 根據被除數符號調整餘數正負
    tst     r4, #0x80000000
    beq     .Lldivmod_done
    rsbs    r2, r2, #0
    mvn     r3, r3
    adcs    r3, r3, #0

.Lldivmod_done:
    pop     {r4, r5, r6, r7, pc}

.Lldivmod_zero:
    @ 除數為 0，依 ARM EABI 規範呼叫 __aeabi_ldiv0
    @ 本 .so 內的 __aeabi_ldiv0 回傳 0，不拋出 SIGFPE
    push    {lr}
    movs    r0, #0
    movs    r1, #0
    bl      __aeabi_ldiv0
    movs    r0, #0
    movs    r1, #0
    movs    r2, #0
    movs    r3, #0
    pop     {pc}

    .size __aeabi_ldivmod, . - __aeabi_ldivmod
