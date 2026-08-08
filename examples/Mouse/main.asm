org 100h

%include "src/paintlib.inc"

start:
    InitVGA
    InitMouse
    InitDB
    SetMouseIcon mouse, 0, 0, 1

main_loop:
    BeginFrame
    ClearScreen 3
    ShowMouse
    EndFrame

    GetKey
    IfNotKey 27, main_loop

    ExitDB
    ExitVGA

    mov ax, 4C00h
    int 21h

%include "assets\sprites\mouse.inc"