; ============================================================
; palette.inc - VGA color palette manipulation
; dos-paint-lib | core module
;
; Usage:
;   SetColor index, r, g, b
;   SetPalette palette_ptr
;
; Examples:
;   SetColor 15, 63, 63, 63       ; Set color index 15 to pure white
;   SetColor 4,  63,  0,  0       ; Set color index 4 to bright red
;   SetPalette my_palette_data     ; Load a full 768-byte RGB array
;   
; Note: VGA RGB values range from 0 to 63 (6-bit DAC resolution)
;
; Clobbers : AX, BX, CX, DX, SI
; Requires : ES = 0A000h (InitVGA)
; ============================================================

%ifndef PALETTE_INC
%define PALETTE_INC

; ----------------------------------------------------------
; Macro: SetColor index, r, g, b
; ----------------------------------------------------------
%macro SetColor 4
    mov bl, %1          ; BL = palette index (0-255)
    mov dh, %2          ; DH = red component   (0-63)
    mov ch, %3          ; CH = green component (0-63)
    mov cl, %4          ; CL = blue component  (0-63)
    call _SetColor
%endmacro

; ----------------------------------------------------------
; Macro: SetPalette palette_ptr
; Loads a full 768-byte RGB array into the VGA DAC
; ----------------------------------------------------------
%macro SetPalette 1
    mov si, %1          ; SI = pointer to the 768-byte palette array (DS:SI)
    call _SetPalette
%endmacro

; ----------------------------------------------------------
; Internal procedure: _SetColor
; Modifies a single color index in the VGA DAC palette
; ----------------------------------------------------------
_SetColor:
    mov dx, 03C8h       ; DX = VGA DAC Write Index Register port
    mov al, bl          ; AL = color index to change
    out dx, al          ; Tell VGA hardware which color index we want to update

    inc dx              ; DX = 03C9h (VGA DAC Data Register port)
    
    mov al, dh          ; Load Red value
    out dx, al          ; Write Red to DAC

    mov al, ch          ; Load Green value
    out dx, al          ; Write Green to DAC

    mov al, cl          ; Load Blue value
    out dx, al          ; Write Blue to DAC

    ret

; ----------------------------------------------------------
; Internal procedure: _SetPalette
; Fast bulk-transfer of 256 RGB triplets (768 bytes) to DAC
; ----------------------------------------------------------
_SetPalette:
    mov dx, 03C8h       ; DX = VGA DAC Write Index Register port
    xor al, al          ; AL = 0 (Start writing at index 0)
    out dx, al          ; Tell VGA DAC to auto-increment starting from index 0

    inc dx              ; DX = 03C9h (VGA DAC Data Register port)
    mov cx, 768         ; CX = loop counter (256 colors * 3 bytes per color)
    
    cld                 ; Clear Direction Flag
    rep outsb           ; Write 768 bytes from [DS:SI] sequentially to port 03C9h
    
    ret

%endif ; PALETTE_INC
