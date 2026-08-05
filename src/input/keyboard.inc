; ============================================================
; keyboard.inc - Keyboard input handling
; dos-paint-lib | input module
;
; Usage:
;   WaitKey             ; pause until any key is pressed
;   GetKey              ; check for keypress (non-blocking)
;   IfKey key, label    ; jump to label if key in AL matches
;   IfNotKey key, label ; jump to label if key in AL does not match
;
; Registers returned (WaitKey / GetKey):
;   AL = ASCII character code
;   AH = BIOS scancode
;   AX = 0 (GetKey only, if no key is pending)
;
; Common scancodes:
;   AH = 48h  Arrow Up
;   AH = 50h  Arrow Down
;   AH = 4Bh  Arrow Left
;   AH = 4Dh  Arrow Right
;   AH = 01h  Escape
;   AH = 1Ch  Enter
;
; Clobbers: AX
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
; Macro: IfKey key, label
; Compares AL with key and jumps to label if equal
; Replaces patterns like:
;   cmp al, 27
;   je escape_pressed
; ----------------------------------------------------------
%macro IfKey 2
    cmp al, %1
    je %2
%endmacro

; ----------------------------------------------------------
; Macro: IfNotKey key, label
; Compares AL with key and jumps to label if not equal
; Replaces patterns like:
;   cmp al, 27
;   jne main_loop
; ----------------------------------------------------------
%macro IfNotKey 2
    cmp al, %1
    jne %2
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

