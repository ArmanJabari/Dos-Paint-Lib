org 100h

%include "src/paintlib.inc"

start:
    InitVGA

    mov cx, 0
    mov bl, 0

_loop:
    mov ax, cx
    mov si, 16
    xor dx, dx
    div si

    mov di, ax
    shl di, 3
    add di, 36

    shl dx, 3
    add dx, 96

    pusha
    DrawRect dx, di, bl, 8, 8, 1
    popa

    inc bl
    inc cx
    cmp cx, 256
    jl _loop

    WaitKey

    ExitVGA

    mov ax, 4C00h
    int 21h
