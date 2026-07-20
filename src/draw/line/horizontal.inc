; ============================================================
; horizontal.inc - Horizontal line drawing
; dos-paint-lib | draw module
;
; Usage:
;   DrawHLine x, y, color_code, length
;
; Examples:
;   DrawHLine 10,  50, 12, 200   ; red horizontal line at y=50,  200px wide
;   DrawHLine 10, 100, 14, 150   ; yel horizontal line at y=100, 150px wide
;
; Clobbers : AX, BX, CX, DX, DI, SI 
; Requires : ES = 0A000h  (call InitVGA first)
; ============================================================
%ifndef HORIZONTAL_INC
%define HORIZONTAL_INC
%include "src/core/vga/offset.inc"

; ----------------------------------------------------------
; Macro: DrawHLine  x, y, color_code, length
; ----------------------------------------------------------
%macro DrawHLine 4
    mov cx, %1          ; cx = x start
    mov dx, %2          ; dx = y position
    mov bl, %3          ; bl = color code
    mov si, %4          ; si = length in pixels
    call _DrawHLine
%endmacro

; ----------------------------------------------------------
; Internal procedure: Horizontal line
; ----------------------------------------------------------
_DrawHLine:
    cld                 ; ensure forward direction for rep stosb
    call _GetVGAOffset  ; di = y * 320 + x (starting pixel offset)

    mov al, bl          ; al = color (stosb uses AL)
    mov cx, si          ; cx = loop counter (length)
    rep stosb           ; draw CX pixels at ES:DI 
    ret

%endif ; HORIZONTAL_INC