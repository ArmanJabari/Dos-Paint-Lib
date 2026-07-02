; ============================================================
; sprite.inc - 2D Sprite Rendering
; dos-paint-lib | render module
;
; Usage:
;   RenderSprite x, y, width, height, offset
;
; Examples:
;   RenderSprite 100, 50, 16, 16, my_sprite_data
;
; Clobbers : AX, BX, CX, DX, SI, DI 
; Requires : ES = 0A000h  (call InitVGA first)
;            DS pointing to the segment containing sprite data
; ============================================================

%ifndef SPRITE_INC
%define SPRITE_INC
%include "core/vgaoffset.inc"

; ----------------------------------------------------------
; Macro: RenderSprite x, y, width, height, offset
; ----------------------------------------------------------
%macro RenderSprite 5
    mov cx, %1          ; cx = x position
    mov dx, %2          ; dx = y position
    mov bx, %3          ; bx = width
    mov bp, %4          ; bp = height
    mov si, %5          ; si = offset (pointer to sprite data)
    call _RenderSprite
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_RenderSprite:
    call _GetVGAOffset  ; di = y * 320 + x (starting offset on screen)

    mov dx, bp          ; dx = row counter (height)
    mov ax, 320         
    sub ax, bx          ; ax = 320 - width (skip distance to next row)

_sprite_row_loop:
    mov cx, bx          ; cx = width (pixels to draw in this row)
    rep movsb           ; copy CX bytes from DS:SI (sprite) to ES:DI (screen)

    add di, ax          ; skip remaining pixels on screen to reach next row
    dec dx              ; decrement row counter
    jnz _sprite_row_loop ; loop until all rows are drawn
    ret

%endif ; SPRITE_INC
