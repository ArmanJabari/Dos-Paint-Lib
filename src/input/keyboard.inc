; ============================================================
; keyboard.inc - Keyboard input handling
; dos-paint-lib | input module
;
; Usage:
;   WaitKey          ; pause until any key is pressed
;   GetKey           ; check for keypress (non-blocking)
;
; After WaitKey returns:
;   AL = ASCII code of the key pressed
;   AH = BIOS scancode of the key pressed
;
; After GetKey returns:
;   AX = 0 if no key was pressed
;   Otherwise, AL = ASCII, AH = scancode
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
; Macro: GetKey
; No parameters needed
; ----------------------------------------------------------
%macro GetKey 0
    call _GetKey
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_WaitKey:
    mov ah, 00h         ; BIOS keyboard read function
    int 16h             ; wait for keypress
                        ; returns: AL = ASCII, AH = scancode
    ret

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_GetKey:
    mov ah, 01h         ; BIOS keyboard status check
    int 16h             ; check for keypress
    jz .no_key          ; jump if buffer is empty
    
    mov ah, 00h         ; read and consume key
    int 16h             ; returns: AL = ASCII, AH = scancode
    ret

.no_key:
    xor ax, ax          ; return 0 if no key pressed
    ret

%endif ; KEYBOARD_INC

