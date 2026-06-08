; ============================================================
; pixel.inc - Single pixel drawing
; dos-paint-lib | draw module
;
; Usage:
;   DrawPixel x, y, color_code
;
; Examples:
;   DrawPixel 160, 100, 15     ; white pixel at center
;   DrawPixel 10,  10,  4      ; red   pixel at (10,10)
;
; Clobbers : AX, DI, DX (set to 0 after mul)
; Requires : ES = 0A000h  (call InitVGA first)
; ============================================================

%ifndef PIXEL_INC
%define PIXEL_INC

; ----------------------------------------------------------
; Macro: sets registers then calls the procedure
; ----------------------------------------------------------
%macro DrawPixel 3
    mov cx, %1          ; cx = x position
    mov dx, %2          ; dx = y position
    mov bl, %3          ; bl = color code
    call _DrawPixel
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_DrawPixel:
    mov ax, 320
    mul dx              ; ax = y * 320  |  dx zeroed (result fits 16-bit)
    add ax, cx          ; ax = y * 320 + x
    mov di, ax          ; di = pixel offset in VGA memory
    mov [es:di], bl     ; write color to screen
    ret

%endif ; PIXEL_INC
