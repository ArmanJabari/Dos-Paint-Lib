; ============================================================
; vertical.inc - Vertical line drawing
; dos-paint-lib | draw module
;
; Usage:
;   DrawVLine x, y, color_code, length
;
; Examples:
;   DrawVLine 50, 10,  10, 180   ; green vertical   line at x=50,  180px tall
;   DrawVLine 80, 20,  11, 100   ; cyan  vertical   line at x=80,  100px tall
;
; Clobbers : AX, BX, CX, DX, DI, SI 
; Requires : ES = 0A000h  (call InitVGA first)
; ============================================================
%ifndef VERTICAL_INC
%define VERTICAL_INC
%include "src/core/vga/offset.inc"

; ----------------------------------------------------------
; Macro: DrawVLine  x, y, color_code, length
; ----------------------------------------------------------
%macro DrawVLine 4
    mov cx, %1          ; cx = x position
    mov dx, %2          ; dx = y start
    mov bl, %3          ; bl = color code
    mov si, %4          ; si = length in pixels
    call _DrawVLine
%endmacro

; ----------------------------------------------------------
; Internal procedure: Vertical line
; ----------------------------------------------------------
_DrawVLine:
    call _GetVGAOffset  ; di = y * 320 + x (starting pixel offset)

    mov cx, si          ; cx = loop counter (length)
_vline_loop:
    mov [es:di], bl     ; draw pixel
    add di, 320         ; move one pixel down (next row)
    dec cx              ; countdown remaining pixels in the column
    jnz _vline_loop     ; continue drawing until the line is complete
    ret

%endif ; VERTICAL_INC