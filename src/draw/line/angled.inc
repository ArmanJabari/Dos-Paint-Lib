; ============================================================
; angled.inc - Angled line drawing
; ============================================================
%ifndef ANGLED_INC
%define ANGLED_INC
%include "src/core/vga/offset.inc"

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

    call _GetVGAOffset  ; di = y0 * 320 + x0 (starting pixel offset)

    mov ax, [bp-2]      ; ax = dx
    sub ax, [bp-4]      ; ax = err (dx - dy)

    mov cx, [bp-10]     ; cx = loop counter

    mov dx, [bp-4]      ; dx = dy
    neg dx              ; dx = -dy

_aline_loop:
    mov [es:di], bl     ; draw pixel

    mov si, ax
    shl si, 1           ; si = 2 * err (e2)

    cmp si, dx          ; compare e2 and -dy
    jle _aline_check_y

    add ax, dx          ; err += -dy
    add di, [bp-6]      ; offset += sx_off
_aline_check_y:
    cmp si, [bp-2]      ; compare e2 and dx
    jge _aline_next_pixel

    add ax, [bp-2]      ; err += dx
    add di, [bp-8]      ; offset += sy_off
_aline_next_pixel:
    dec cx              ; track total line length remaining
    jnz _aline_loop     ; calculate and draw the next pixel in the path

    mov sp, bp
    pop bp
    ret

%endif ; ANGLED_INC