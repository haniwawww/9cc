.global _plus, _main

_plus:
    sub     sp, sp, #16
    str     w0, [sp, #12]
    str     w1, [sp, #8]
    ldr     w8, [sp, #12]
    ldr     w9, [sp, #8]
    add     w0, w8, w9
    add     sp, sp, #16
    ret

_main:
    sub     sp, sp, #32
    stp     x29, x30, [sp, #16]
    add     x29, sp, #16
    stur    wzr, [x29, #-4]
    mov     w0, #3
    mov     w1, #4
    bl      _plus
    ldp     x29, x30, [sp, #16]
    add     sp, sp, #32
    ret
