org 100h

jmp start

%include "core/VGA.inc"
%include "draw/pixel.inc"

start:
    InitVGA

    DrawPixel  160, 100, 15
    DrawPixel  10,  10,  4
    DrawPixel  310, 10,  1
    DrawPixel  10,  190, 2
    DrawPixel  310, 190, 14

    mov ah, 00h
    int 16h

    ExitVGA

    mov ax, 4C00h
    int 21h
