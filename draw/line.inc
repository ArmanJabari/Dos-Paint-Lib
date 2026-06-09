; ============================================================
; line.inc - Horizontal line drawing
; dos-paint-lib | draw module
;
; Usage:
;   DrawHLine x, y, color_code, length
;
; Examples:
;   DrawHLine 10,  50, 12, 200   ; red horizontal line at y=50,  200px wide
;   DrawHLine 10, 100, 14, 150   ; yel horizontal line at y=100, 150px wide
;
; Clobbers : AX, CX, DI, DX (set to 0 after mul)
; Requires : ES = 0A000h  (call InitVGA first)
; ============================================================
%ifndef LINE_INC
%define LINE_INC

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
    mov ax, 320
    mul dx              ; ax = y * 320
    add ax, cx          ; ax = y * 320 + x
    mov di, ax          ; di = starting pixel offset
    mov cx, si          ; cx = loop counter (length)
_hline_loop:
    mov [es:di], bl     ; draw pixel
    inc di              ; move one pixel right
    loop _hline_loop
    ret

%endif ; LINE_INC
