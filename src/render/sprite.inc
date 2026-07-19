; ============================================================
; sprite.inc - 2D Sprite Rendering
; dos-paint-lib | render module
;
; Usage:
;   RenderSprite x, y, width, height, offset
;   RenderSprite x, y, width, height, offset, color_code
;
; Examples:
;   RenderSprite 100, 50, 16, 16, my_sprite_data
;
; Clobbers : AX, BX, CX, DX, SI, DI, BP 
; Requires : ES = 0A000h  (call InitVGA first)
;            DS pointing to the segment containing sprite data
; ============================================================

%ifndef SPRITE_INC
%define SPRITE_INC
%include "src/core/vga/offset.inc"

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
; Macro: RenderSprite x, y, width, height, offset, color_code
; ----------------------------------------------------------
%macro RenderSprite 6
    mov cx, %1          ; cx = x position
    mov dx, %2          ; dx = y position
    mov bx, %3          ; bx = width
    mov bp, %4          ; bp = height
    mov si, %5          ; si = offset (pointer to sprite data)
    mov al, %6          ; al = transparent color code
    call _RenderSpriteT
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_RenderSprite:
    cld                 ; ensure forward direction for rep movsb
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

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_RenderSpriteT:
    push ax             ; save transparent color temporarily
    cld                 ; ensure forward direction for lodsb
    call _GetVGAOffset  ; di = y * 320 + x (starting offset on screen)
    pop ax              ; restore transparent color into AL

    mov ah, al          ; ah = transparent color code (store it safely)
    
    mov dx, bp          ; dx = row counter (height)
    mov bp, 320         
    sub bp, bx          ; bp = 320 - width (skip distance to next row)

_sprite_t_row_loop:
    mov cx, bx          ; cx = width (pixels to process in this row)

_sprite_t_col_loop:
    lodsb               ; load next pixel color from DS:SI into AL, increment SI
    cmp al, ah          ; compare pixel color (AL) with transparent color (AH)
    je _sprite_t_skip   ; if they match, skip drawing this pixel

    mov [es:di], al     ; draw pixel to screen (ES:DI)

_sprite_t_skip:
    inc di              ; move to next screen pixel horizontally
    dec cx              ; decrement column counter
    jnz _sprite_t_col_loop ; loop until all pixels in row are processed

    add di, bp          ; skip remaining pixels on screen to reach next row
    dec dx              ; decrement row counter
    jnz _sprite_t_row_loop ; loop until all rows are drawn
    ret

%endif ; SPRITE_INC
