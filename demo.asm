org 100h

jmp start

%include "core/VGA.inc"
%include "core/timing.inc"
%include "draw/pixel.inc"
%include "draw/line.inc"
%include "draw/rect.inc"
%include "input/keyboard.inc"
%include "input/mouse.inc"
%include "screen/clear.inc"

start:
    InitVGA
    InitMouse
    ShowMouse

    HideMouse
    DrawPixel  160, 100, 15
    DrawPixel  10,  10,  4
    DrawPixel  310, 10,  1
    DrawPixel  10,  190, 2
    DrawPixel  310, 190, 14

    DrawHLine  10,  50,  12, 200
    DrawHLine  10,  100, 14, 150

    DrawVLine 50, 10,  10, 180
    DrawVLine 80, 20,  11, 100

    DrawRect 10,  10,  15, 100, 50, 0
    DrawRect 50,  50,  12, 80,  40, 1
    DrawRect 100, 100, 14, 60,  30, 0
    ShowMouse

    Delay 1000

    HideMouse
    ClearScreen 0
    ShowMouse

    HideMouse
    DrawALine 0, 0, 319, 199, 12
    DrawALine 0, 199, 319, 0, 15
    ShowMouse

    WaitKey

    HideMouse
    ExitVGA

    mov ax, 4C00h
    int 21h
