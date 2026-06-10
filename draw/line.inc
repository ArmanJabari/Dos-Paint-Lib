; ============================================================
; line.inc - Horizontal and vertical line drawing
; dos-paint-lib | draw module
;
; Usage:
;   DrawHLine x, y, color_code, length
;   DrawVLine x, y, color_code, length
;
; Examples:
;   DrawHLine 10,  50, 12, 200   ; red horizontal line at y=50,  200px wide
;   DrawHLine 10, 100, 14, 150   ; yel horizontal line at y=100, 150px wide
;   DrawVLine 50, 10,  10, 180   ; green vertical   line at x=50,  180px tall
;   DrawVLine 80, 20,  11, 100   ; cyan  vertical   line at x=80,  100px tall
;
; Clobbers : AX, CX, DI, DX, SI (set to 0 after mul)
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

; ----------------------------------------------------------
; Internal procedure: Vertical line
; ----------------------------------------------------------
_DrawVLine:
    mov ax, 320
    mul dx              ; ax = y * 320
    add ax, cx          ; ax = y * 320 + x
    mov di, ax          ; di = starting pixel offset
    mov cx, si          ; cx = loop counter (length)
_vline_loop:
    mov [es:di], bl     ; draw pixel
    add di, 320         ; move one pixel down (next row)
    loop _vline_loop
    ret

%endif ; LINE_INC
