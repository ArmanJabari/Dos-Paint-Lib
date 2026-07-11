; ============================================================
; palette.inc - VGA color palette manipulation
; dos-paint-lib | core module
;
; Usage:
;   SetColor index, r, g, b
;
; Examples:
;   SetColor 15, 63, 63, 63       ; Set color index 15 to pure white
;   SetColor 4,  63,  0,  0       ; Set color index 4 to bright red
;   
; Note: VGA RGB values range from 0 to 63
;
; Clobbers : AX, BX, CX, DX
; Requires : ES = 0A000h (InitVGA)
; ============================================================

%ifndef PALETTE_INC
%define PALETTE_INC

; ----------------------------------------------------------
; Macro: SetColor index, r, g, b
; ----------------------------------------------------------
%macro SetColor 4
    mov bl, %1          ; bl = palette index (0-255)
    mov dh, %2          ; dh = red   (0-63)
    mov ch, %3          ; ch = green (0-63)
    mov cl, %4          ; cl = blue  (0-63)
    call _SetColor
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_SetColor:
    mov dx, 03C8h       ; VGA DAC Write Index Register port
    mov al, bl          ; al = palette index
    out dx, al          ; Tell VGA we want to modify this color index

    inc dx              ; dx = 03C9h (VGA DAC Data Register port)
    
    mov al, dh          ; Red component
    out dx, al          ; Write red

    mov al, ch          ; Green component
    out dx, al          ; Write green

    mov al, cl          ; Blue component
    out dx, al          ; Write blue

    ret

%endif ; PALETTE_INC
