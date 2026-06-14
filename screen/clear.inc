; ============================================================
; clear.inc - Screen clearing
; dos-paint-lib | screen module
;
; Usage:
;   ClearScreen color_code
;
; Examples:
;   ClearScreen 0    ; fill screen black
;   ClearScreen 4    ; fill screen red
;   ClearScreen 15   ; fill screen white
;
; Uses rep stosb to fill all 64000 pixels in one fast sweep.
; Much faster than a manual pixel loop.
;
; Clobbers : AX, CX, DI
; Requires : ES = 0A000h  (call InitVGA first)
; ============================================================

%ifndef CLEAR_INC
%define CLEAR_INC

; ----------------------------------------------------------
; Macro: ClearScreen  color_code
; ----------------------------------------------------------
%macro ClearScreen 1
    mov bl, %1          ; bl = color code
    call _ClearScreen
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_ClearScreen:
    mov al, bl          ; al = color to fill
    xor di, di          ; di = 0 (start of VGA framebuffer)
    mov cx, 64000       ; 320 * 200 = 64000 total pixels
    rep stosb           ; fill ES:DI with AL, CX times
    ret

%endif ; CLEAR_INC
