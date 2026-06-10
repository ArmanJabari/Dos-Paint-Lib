org 100h

jmp start

%include "core/VGA.inc"
%include "draw/pixel.inc"
%include "draw/line.inc"

start:
    InitVGA

    DrawPixel  160, 100, 15
    DrawPixel  10,  10,  4
    DrawPixel  310, 10,  1
    DrawPixel  10,  190, 2
    DrawPixel  310, 190, 14

    DrawHLine  10,  50,  12, 200
    DrawHLine  10,  100, 14, 150

    DrawVLine 50, 10,  10, 180
    DrawVLine 80, 20,  11, 100

    mov ah, 00h
    int 16h

    ExitVGA

    mov ax, 4C00h
    int 21h
