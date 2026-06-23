; ============================================================
; mouse.inc - Mouse input handling
; dos-paint-lib | input module
;
; Usage:
;   InitMouse          ; set up mouse driver
;   ShowMouse          ; display the mouse cursor
;   HideMouse          ; hide the mouse cursor
;
; After InitMouse returns:
;   AX = FFFFh if mouse support is available
;   AX = 0000h if no mouse driver found
;
; Note: Always call HideMouse BEFORE drawing to the screen to
; prevent graphical corruption, then call ShowMouse after.
;
; Clobbers : AX
; ============================================================

%ifndef MOUSE_INC
%define MOUSE_INC

; ----------------------------------------------------------
; Macro: InitMouse
; No parameters needed
; ----------------------------------------------------------
%macro InitMouse 0
    call _InitMouse
%endmacro

; ----------------------------------------------------------
; Macro: ShowMouse
; No parameters needed
; ----------------------------------------------------------
%macro ShowMouse 0
    call _ShowMouse
%endmacro

; ----------------------------------------------------------
; Macro: HideMouse
; No parameters needed
; ----------------------------------------------------------
%macro HideMouse 0
    call _HideMouse
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_InitMouse:
    mov ax, 0000h         ; BIOS mouse initialization function
    int 33h               ; initialize mouse
                          ; returns: AX = status (0000h/FFFFh)
    ret

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_ShowMouse:
    mov ax, 0001h         ; BIOS mouse show cursor function
    int 33h               ; display mouse pointer
    ret

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_HideMouse:
    mov ax, 0002h         ; BIOS mouse hide cursor function
    int 33h               ; remove mouse pointer from screen
    ret

%endif ; MOUSE_INC
