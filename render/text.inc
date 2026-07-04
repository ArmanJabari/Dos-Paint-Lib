; ============================================================
; text.inc - character rendering
; dos-paint-lib | render module
;
; Usage:
;   RenderChar x, y, color_code, char
;
; Examples:
;   RenderChar 10,  10, 15, 'A'    ; Draw white letter 'A' at (10,10)
;   RenderChar 100, 50, 12, 66     ; Draw red letter 'B' (ASCII 66) at (100,50)
;
; Clobbers : AX, BX, CX, DX, SI, DI 
; Requires : ES = 0A000h  (call InitVGA first)
; ============================================================

%ifndef TEXT_INC
%define TEXT_INC

%include "render/font.inc"
%include "core/vgaoffset.inc"

; ----------------------------------------------------------
; Macro: RenderChar  x, y, color_code, char
; ----------------------------------------------------------
%macro RenderChar 4
    mov cx, %1          ; cx = x position
    mov dx, %2          ; dx = y position
    mov bl, %3          ; bl = color code
    mov al, %4          ; al = ASCII character code
    call _RenderChar
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_RenderChar:
    sub al, 65          ; offset to 'A' (ASCII 65)
    xor ah, ah          ; clear upper byte of AX
    shl ax, 3           ; multiply index by 8 (8 bytes per character)
    mov si, sys_font    ; get base address of font data
    add si, ax          ; si = pointer to character data

    call _GetVGAOffset  ; di = y * 320 + x (starting pixel offset)

    mov dx, 8           ; dx = row counter (8 rows per char)

_char_row_loop:
    mov al, [ds:si]     ; read one byte (row of pixels) from font
    inc si              ; advance to next byte in font

    push di             ; save current screen row starting offset
    mov cx, 8           ; cx = bit counter (8 pixels per row)

_char_bit_loop:
    shl al, 1           ; shift highest bit into carry flag
    jnc _skip_pixel     ; if carry is 0, pixel is transparent (skip drawing)
    mov [es:di], bl     ; if carry is 1, draw the colored pixel

_skip_pixel:
    inc di              ; move to next pixel position on screen
    dec cx              ; decrement bit counter
    jnz _char_bit_loop  ; loop until all 8 bits are processed

    pop di              ; restore screen row starting offset
    add di, 320         ; jump straight down to next row on screen
    dec dx              ; decrement row counter
    jnz _char_row_loop  ; loop until all 8 rows are drawn

    ret

%endif ; TEXT_INC
