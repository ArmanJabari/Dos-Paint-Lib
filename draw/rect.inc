; ============================================================
; rect.inc - Rectangle hollow and filled drawing
; dos-paint-lib | draw module
;
; Usage:
;   DrawRect x, y, color_code, width, height, fill
;
;   fill = 0 -> hollow rectangle
;   fill = 1 -> filled rectangle
;
; Examples:
;   DrawRect 10,  10,  15, 100, 50, 0   ; white hollow rect 
;   DrawRect 50,  50,  12, 80,  40, 1   ; red filled rect 
;   DrawRect 100, 100, 14, 60,  30, 0   ; yellow hollow rect 
;
; Clobbers : AX, BX, CX, DX, SI, DI, BP
; Requires : ES = 0A000h  (call InitVGA first)
; ============================================================

%ifndef RECT_INC
%define RECT_INC

; ----------------------------------------------------------
; Macro: DrawRect  x, y, color_code, width, height, fill
; ----------------------------------------------------------
%macro DrawRect 6
    mov cx, %1          ; cx = x position
    mov dx, %2          ; dx = y position
    mov bl, %3          ; bl = color code
    mov si, %4          ; si = width
    mov bp, %5          ; bp = height
    mov bh, %6          ; bh = fill flag
    call _DrawRect
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_DrawRect:
    mov di, dx          ; di = y position
    shl di, 8           ; di = y * 256
    mov ax, dx          ; ax = y position
    shl ax, 6           ; ax = y * 64
    add di, ax          ; di = y * 256 + y * 64 (y * 320)
    add di, cx          ; di = y * 320 + x (starting offset)

    mov al, bl          ; al = color

    cmp bh, 1           ; check is it filled
    je  _fill_rect      ; jump to fill routine

; ----------------------------------------------------------
; Hollow rectangle (outline only)
; ----------------------------------------------------------
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

; ----------------------------------------------------------
; Filled rectangle (every pixel drawn)
; ----------------------------------------------------------
_fill_rect:
    mov dx, bp          ; dx = height counter
    mov bx, 320
    sub bx, si          ; bx = 320 - width (row skip amount)

_fill_outer:
    mov cx, si          ; cx = width (restored each row since loop zeroes cx)
_fill_inner:
    mov byte [es:di], al        ; draw pixel
    inc di                      ; next pixel right
    loop _fill_inner

    add di, bx                  ; skip remaining pixels to reach next row start
    dec dx                      ; decrement row counter
    jnz _fill_outer             ; loop until all rows done
    ret

%endif ; RECT_INC
