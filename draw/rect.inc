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
    mov dx, bp          ; dx = height counter
    mov bx, si          ; bx = width
    dec bx              ; bx = width - 1 (distance to right edge)

_rect_loop:
    cmp dx, bp          ; first row?
    je  _rect_full_row
    cmp dx, 1           ; last row?
    je  _rect_full_row

    mov byte [es:di], al    ; draw left edge pixel
    mov byte [es:bx+di], al ; draw right edge pixel using base+index addressing
    jmp _rect_next_row

_rect_full_row:
    push di                 ; save row start offset (rep stosb modifies di)
    mov cx, si              ; cx = width (loop counter for stosb)
    rep stosb               ; rapidly draw full horizontal line
    pop di                  ; restore row start offset

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
    mov cx, si          ; cx = width (restored each row)
    rep stosb           ; rapidly write AL to ES:DI and auto-increment DI, CX times

    add di, bx                  ; skip remaining pixels to reach next row start
    dec dx                      ; decrement row counter
    jnz _fill_outer             ; loop until all rows done
    ret

%endif ; RECT_INC
