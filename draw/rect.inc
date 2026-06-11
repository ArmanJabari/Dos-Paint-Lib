; ============================================================
; rect.inc - Rectangle hollow drawing
; dos-paint-lib | draw module
;
; Usage:
;   DrawRect x, y, color_code, width, height
;
; Examples:
;   DrawRect 10,  10,  15, 100, 50   ; white rect 
;   DrawRect 50,  50,  12, 80,  40   ; red rect 
;   DrawRect 100, 100, 14, 60,  30   ; yellow rect 
;
; Draws hollow only (top, bottom, left edge, right edge)
;
; Clobbers : AX, BX, CX, DX, SI, DI, BP
; Requires : ES = 0A000h  (call InitVGA first)
; ============================================================

%ifndef RECT_INC
%define RECT_INC

; ----------------------------------------------------------
; Macro: DrawRect  x, y, color_code, width, height
; ----------------------------------------------------------
%macro DrawRect 5
    mov cx, %1          ; cx = x position
    mov dx, %2          ; dx = y position
    mov bl, %3          ; bl = color code
    mov si, %4          ; si = width
    mov bp, %5          ; bp = height
    call _DrawRect
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_DrawRect:
    mov ax, 320
    mul dx              ; ax = y * 320  |  dx zeroed
    add ax, cx          ; ax = y * 320 + x
    mov di, ax          ; di = starting offset (top-left pixel)

    mov al, bl          ; al = color
    mov bx, si          ; bx = width
    mov dx, bp          ; dx = height counter (starts equal to bp)

_rect_loop:
    cmp dx, bp          ; first row?
    je  _rect_full_row
    cmp dx, 1           ; last row?
    je  _rect_full_row

    mov byte [es:di], al        ; left edge pixel
    mov si, di                  ; si = left edge offset
    add si, bx                  ; si = left + width
    dec si                      ; si = right edge offset (left + width - 1)
    mov byte [es:si], al        ; right edge pixel
    jmp _rect_next_row

_rect_full_row:
    mov cx, bx                  ; cx = width (loop counter)
    mov si, di                  ; si = start of row
_rect_row_loop:
    mov byte [es:si], al        ; draw pixel
    inc si                      ; next pixel
    loop _rect_row_loop

_rect_next_row:
    add di, 320                 ; move di down one row
    dec dx                      ; decrement row counter
    jnz _rect_loop              ; loop until all rows done
    ret

%endif ; RECT_INC
