; ============================================================
; line.inc - Horizontal, vertical and angled line drawing
; dos-paint-lib | draw module
;
; Usage:
;   DrawHLine x, y, color_code, length
;   DrawVLine x, y, color_code, length
;   DrawALine x0, y0, x1, y1, color_code
;
; Examples:
;   DrawHLine 10,  50, 12, 200   ; red horizontal line at y=50,  200px wide
;   DrawHLine 10, 100, 14, 150   ; yel horizontal line at y=100, 150px wide
;   DrawVLine 50, 10,  10, 180   ; green vertical   line at x=50,  180px tall
;   DrawVLine 80, 20,  11, 100   ; cyan  vertical   line at x=80,  100px tall
;   DrawALine 0, 0, 319, 199, 12  ; red angled line from top-left to bottom-right
;   DrawALine 0, 199, 319, 0,  15  ; white angled line from bottom-left to top-right
;
; Clobbers : AX, CX, DI, DX, SI 
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
; Macro: DrawALine  x0, y0, x1, y1, color_code
; ----------------------------------------------------------
%macro DrawALine 5
    mov cx, %1          ; cx = x0
    mov dx, %2          ; dx = y0
    mov si, %3          ; si = x1
    mov di, %4          ; di = y1
    mov bl, %5          ; bl = color code
    call _DrawALine
%endmacro

; ----------------------------------------------------------
; Internal procedure: Calculate VGA offset
; In  : CX = x, DX = y
; Out : DI = y * 320 + x
; ----------------------------------------------------------
_GetVGAOffset:
    mov di, dx          ; di = y position
    shl di, 8           ; di = y * 256
    mov ax, dx          ; ax = y position
    shl ax, 6           ; ax = y * 64
    add di, ax          ; di = y * 256 + y * 64 (y * 320)
    add di, cx          ; di = y * 320 + x (starting pixel offset)
    ret

; ----------------------------------------------------------
; Internal procedure: Horizontal line
; ----------------------------------------------------------
_DrawHLine:
    call _GetVGAOffset ; di = y * 320 + x (starting pixel offset)

    mov al, bl          ; al = color (stosb uses AL)
    mov cx, si          ; cx = loop counter (length)
    rep stosb           ; draw CX pixels at ES:DI 
    ret

; ----------------------------------------------------------
; Internal procedure: Vertical line
; ----------------------------------------------------------
_DrawVLine:
    call _GetVGAOffset ; di = y * 320 + x (starting pixel offset)

    mov cx, si          ; cx = loop counter (length)
_vline_loop:
    mov [es:di], bl     ; draw pixel
    add di, 320         ; move one pixel down (next row)
    loop _vline_loop
    ret

; ----------------------------------------------------------
; Internal procedure: Angled line
; ----------------------------------------------------------
_DrawALine:
    push bp
    mov bp, sp
    sub sp, 12          ; allocate 12 bytes on stack

    mov ax, si
    sub ax, cx          ; ax = x1 - x0 (dx)
    mov si, 1           ; si = 1 (sx_off)
    jns _aline_dx_pos
    neg ax              ; make dx positive
    mov si, -1          ; sx_off = -1
_aline_dx_pos:
    mov [bp-2], ax      ; [bp-2] = dx
    mov [bp-6], si      ; [bp-6] = sx_off

    mov ax, di
    sub ax, dx          ; ax = y1 - y0 (dy)
    mov di, 320         ; di = 320 (sy_off down)
    jns _aline_dy_pos
    neg ax              ; make dy positive
    mov di, -320        ; sy_off = -320 (up)
_aline_dy_pos:
    mov [bp-4], ax      ; [bp-4] = dy
    mov [bp-8], di      ; [bp-8] = sy_off

    mov ax, [bp-2]      ; ax = dx
    cmp ax, [bp-4]      ; compare dx and dy
    jge _aline_set_cnt
    mov ax, [bp-4]      ; ax = max(dx, dy)
_aline_set_cnt:
    inc ax
    mov [bp-10], ax     ; [bp-10] = loop counter

    call _GetVGAOffset ; di = y0 * 320 + x0 (starting pixel offset)

    mov ax, [bp-2]      ; ax = dx
    sub ax, [bp-4]      ; ax = err (dx - dy)

    mov cx, [bp-10]     ; cx = loop counter
_aline_loop:
    mov [es:di], bl     ; draw pixel

    mov si, ax
    shl si, 1           ; si = 2 * err (e2)

    mov dx, [bp-4]      ; dx = dy
    neg dx              ; dx = -dy
    cmp si, dx          ; compare e2 and -dy
    jle _aline_check_y

    sub ax, [bp-4]      ; err -= dy
    add di, [bp-6]      ; offset += sx_off
_aline_check_y:
    cmp si, [bp-2]      ; compare e2 and dx
    jge _aline_next_pixel

    add ax, [bp-2]      ; err += dx
    add di, [bp-8]      ; offset += sy_off
_aline_next_pixel:
    loop _aline_loop    ; next pixel

    mov sp, bp
    pop bp
    ret

%endif ; LINE_INC
