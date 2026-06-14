; ============================================================
; keyboard.inc - Keyboard input handling
; dos-paint-lib | input module
;
; Usage:
;   WaitKey          ; pause until any key is pressed
;
; After WaitKey returns:
;   AL = ASCII code of the key pressed
;   AH = BIOS scancode of the key pressed
;
; Common scancodes:
;   AH = 48h  Arrow Up
;   AH = 50h  Arrow Down
;   AH = 4Bh  Arrow Left
;   AH = 4Dh  Arrow Right
;   AH = 01h  Escape
;   AH = 1Ch  Enter
;
; Clobbers : AX
; ============================================================

%ifndef KEYBOARD_INC
%define KEYBOARD_INC

; ----------------------------------------------------------
; Macro: WaitKey
; No parameters needed
; ----------------------------------------------------------
%macro WaitKey 0
    call _WaitKey
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_WaitKey:
    mov ah, 00h         ; BIOS keyboard read function
    int 16h             ; wait for keypress
                        ; returns: AL = ASCII, AH = scancode
    ret

%endif ; KEYBOARD_INC

