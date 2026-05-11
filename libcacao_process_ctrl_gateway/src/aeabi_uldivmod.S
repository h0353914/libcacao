/*
 * aeabi_uldivmod.S — ARM EABI __aeabi_uldivmod 軟體實作
 *
 * 原始 libcacao_process_ctrl_gateway.so (GCC 編譯) 靜態包含來自 libgcc.a 的
 * __aeabi_uldivmod。本檔案以純 Thumb-2 軟體除法實作，不依賴外部 runtime library，
 * 避免與 --exclude-libs 衝突。
 *
 * 介面 (ARM EABI):
 *   Input:  r0:r1 = numerator  (lo:hi)
 *           r2:r3 = denominator (lo:hi)
 *   Output: r0:r1 = quotient   (lo:hi)
 *           r2:r3 = remainder  (lo:hi)
 *
 * 演算法：64 次 restoring binary long division
 */

    .syntax unified
    .thumb
    .text
    .align 2

    .global __aeabi_uldivmod
    .type   __aeabi_uldivmod, %function
    .thumb_func

__aeabi_uldivmod:
    @ 保存 callee-saved 暫存器
    push    {r4, r5, r6, r7, r8, r9, r10, r11, lr}

    @ 工作暫存器配置:
    @   r4:r5  = numerator   n  (lo:hi)
    @   r6:r7  = denominator d  (lo:hi)
    @   r8:r9  = quotient    q  (lo:hi), 初始 0
    @   r10:r11 = remainder  rem (lo:hi), 初始 0
    @   r12     = 迭代計數 (64)

    movs    r4,  r0          @ n_lo
    movs    r5,  r1          @ n_hi
    movs    r6,  r2          @ d_lo
    movs    r7,  r3          @ d_hi

    movs    r8,  #0          @ quot_lo = 0
    movs    r9,  #0          @ quot_hi = 0
    movs    r10, #0          @ rem_lo  = 0
    movs    r11, #0          @ rem_hi  = 0
    mov     r12, #64         @ 64 次迭代

.Ludivmod_loop:
    @ 將 rem 和 n 同時左移 1 位 (128-bit left shift，取 n 最高位進 rem)
    @ rem_hi = (rem_hi << 1) | (rem_lo >> 31)
    lsl     r11, r11, #1
    orr     r11, r11, r10, lsr #31

    @ rem_lo = (rem_lo << 1) | (n_hi >> 31)
    lsl     r10, r10, #1
    orr     r10, r10, r5, lsr #31

    @ n_hi = (n_hi << 1) | (n_lo >> 31)
    lsl     r5, r5, #1
    orr     r5, r5, r4, lsr #31

    @ n_lo = n_lo << 1
    lsl     r4, r4, #1

    @ quot 左移 1 位
    lsl     r9, r9, #1
    orr     r9, r9, r8, lsr #31
    lsl     r8, r8, #1

    @ 比較 rem (r10:r11) >= d (r6:r7)
    @ 先比較高位
    cmp     r11, r7
    bhi     .Ludivmod_sub    @ rem_hi > d_hi → 做減法
    bne     .Ludivmod_no_sub @ rem_hi < d_hi → 不做減法
    @ rem_hi == d_hi，比較低位
    cmp     r10, r6
    blo     .Ludivmod_no_sub @ rem_lo < d_lo → 不做減法

.Ludivmod_sub:
    @ rem -= d
    subs    r10, r10, r6
    sbc     r11, r11, r7
    @ 設置商的最低位
    orr     r8,  r8, #1

.Ludivmod_no_sub:
    subs    r12, r12, #1
    bne     .Ludivmod_loop

    @ 將結果放入返回暫存器
    movs    r0, r8           @ quot_lo
    movs    r1, r9           @ quot_hi
    movs    r2, r10          @ rem_lo
    movs    r3, r11          @ rem_hi

    pop     {r4, r5, r6, r7, r8, r9, r10, r11, pc}

    .size __aeabi_uldivmod, . - __aeabi_uldivmod
