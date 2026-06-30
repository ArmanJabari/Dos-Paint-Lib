org 100h

jmp start

%include "core/VGA.inc"
%include "core/timing.inc"
%include "draw/pixel.inc"
%include "draw/line.inc"
%include "draw/rect.inc"
%include "render/sprite.inc"
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

    mov word [y_pos], 96    
    mov word [y_dir], 1     

main_loop:
    WaitVSync

    HideMouse
    DrawRect 156, [y_pos], 0, 8, 8, 1

    DrawALine 0, 0, 319, 199, 12
    DrawALine 0, 199, 319, 0, 15

    mov ax, [y_pos]
    add ax, [y_dir]
    mov [y_pos], ax

    cmp ax, 0
    jle change_dir_down
    cmp ax, 192
    jge change_dir_up
    jmp draw_sprite

change_dir_down:
    mov word [y_dir], 1
    jmp draw_sprite

change_dir_up:
    mov word [y_dir], -1

draw_sprite:
    RenderSprite 156, [y_pos], 8, 8, smily_face
    ShowMouse

    GetKey
    cmp ax, 0
    jz main_loop

    HideMouse
    ExitVGA

    mov ax, 4C00h
    int 21h

y_pos: dw 0
y_dir: dw 1

smily_face:
    db  0,  0, 14, 14, 14, 14,  0,  0
    db  0, 14, 14, 14, 14, 14, 14,  0
    db 14, 14,  0, 14, 14,  0, 14, 14
    db 14, 14, 14, 14, 14, 14, 14, 14
    db 14,  0, 14, 14, 14, 14,  0, 14
    db 14, 14,  0,  0,  0,  0, 14, 14
    db  0, 14, 14, 14, 14, 14, 14,  0
    db  0,  0, 14, 14, 14, 14,  0,  0
