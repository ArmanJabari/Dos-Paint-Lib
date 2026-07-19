; ============================================================
; font.inc - 8x8 Monochrome Bitmap Font (0-9, A-Z)
; dos-paint-lib | render module
;
; Description:
;   Contains binary data for 8x8 font characters.
;   Supports numbers and uppercase ASCII letters 'A' to 'Z'.
; ============================================================

%ifndef FONT_INC
%define FONT_INC

sys_font:
    ; ASCII 48: '0'
    db 00111100b
    db 01000010b
    db 01000010b
    db 01000010b
    db 01000010b
    db 01000010b
    db 00111100b
    db 00000000b

    ; ASCII 49: '1'
    db 00001000b
    db 00011000b
    db 00101000b
    db 00001000b
    db 00001000b
    db 00001000b
    db 00111110b
    db 00000000b

    ; ASCII 50: '2'
    db 00111100b
    db 01000010b
    db 00000010b
    db 00001100b
    db 00110000b
    db 01000000b
    db 01111110b
    db 00000000b

    ; ASCII 51: '3'
    db 00111100b
    db 01000010b
    db 00000010b
    db 00011100b
    db 00000010b
    db 01000010b
    db 00111100b
    db 00000000b

    ; ASCII 52: '4'
    db 00000100b
    db 00001100b
    db 00010100b
    db 00100100b
    db 01111110b
    db 00000100b
    db 00000100b
    db 00000000b

    ; ASCII 53: '5'
    db 01111110b
    db 01000000b
    db 01111100b
    db 00000010b
    db 00000010b
    db 01000010b
    db 00111100b
    db 00000000b

    ; ASCII 54: '6'
    db 00111100b
    db 01000000b
    db 01111100b
    db 01000010b
    db 01000010b
    db 01000010b
    db 00111100b
    db 00000000b

    ; ASCII 55: '7'
    db 01111110b
    db 00000010b
    db 00000010b
    db 00000100b
    db 00001000b
    db 00010000b
    db 00100000b
    db 00000000b

    ; ASCII 56: '8'
    db 00111100b
    db 01000010b
    db 01000010b
    db 00111100b
    db 01000010b
    db 01000010b
    db 00111100b
    db 00000000b

    ; ASCII 57: '9'
    db 00111100b
    db 01000010b
    db 01000010b
    db 01000010b
    db 00111110b
    db 00000010b
    db 00111100b
    db 00000000b

    ; ASCII 65: 'A'
    db 00011000b
    db 00100100b
    db 01000010b
    db 01000010b
    db 01111110b
    db 01000010b
    db 01000010b
    db 00000000b

    ; ASCII 66: 'B'
    db 01111100b
    db 01000010b
    db 01000010b
    db 01111100b
    db 01000010b
    db 01000010b
    db 01111100b
    db 00000000b

    ; ASCII 67: 'C'
    db 00111100b
    db 01000010b
    db 01000000b
    db 01000000b
    db 01000000b
    db 01000010b
    db 00111100b
    db 00000000b

    ; ASCII 68: 'D'
    db 01111000b
    db 01000100b
    db 01000010b
    db 01000010b
    db 01000010b
    db 01000100b
    db 01111000b
    db 00000000b

    ; ASCII 69: 'E'
    db 01111110b
    db 01000000b
    db 01000000b
    db 01111100b
    db 01000000b
    db 01000000b
    db 01111110b
    db 00000000b

    ; ASCII 70: 'F'
    db 01111110b
    db 01000000b
    db 01000000b
    db 01111100b
    db 01000000b
    db 01000000b
    db 01000000b
    db 00000000b

    ; ASCII 71: 'G'
    db 00111100b
    db 01000010b
    db 01000000b
    db 01001110b
    db 01000010b
    db 01000010b
    db 00111100b
    db 00000000b

    ; ASCII 72: 'H'
    db 01000010b
    db 01000010b
    db 01000010b
    db 01111110b
    db 01000010b
    db 01000010b
    db 01000010b
    db 00000000b

    ; ASCII 73: 'I'
    db 01111100b
    db 00010000b
    db 00010000b
    db 00010000b
    db 00010000b
    db 00010000b
    db 01111100b
    db 00000000b

    ; ASCII 74: 'J'
    db 00001110b
    db 00000100b
    db 00000100b
    db 00000100b
    db 01000100b
    db 01000100b
    db 00111000b
    db 00000000b

    ; ASCII 75: 'K'
    db 01000100b
    db 01001000b
    db 01010000b
    db 01100000b
    db 01010000b
    db 01001000b
    db 01000100b
    db 00000000b

    ; ASCII 76: 'L' 
    db 01000000b
    db 01000000b
    db 01000000b
    db 01000000b
    db 01000000b
    db 01000000b
    db 01111100b
    db 00000000b

    ; ASCII 77: 'M' 
    db 01000010b
    db 01100110b
    db 01011010b
    db 01000010b
    db 01000010b
    db 01000010b
    db 01000010b
    db 00000000b

    ; ASCII 78: 'N'
    db 01000010b
    db 01100010b
    db 01010010b
    db 01001010b
    db 01000110b
    db 01000010b
    db 01000010b
    db 00000000b

    ; ASCII 79: 'O'
    db 00111100b
    db 01000010b
    db 01000010b
    db 01000010b
    db 01000010b
    db 01000010b
    db 00111100b
    db 00000000b

    ; ASCII 80: 'P'
    db 01111100b
    db 01000010b
    db 01000010b
    db 01111100b
    db 01000000b
    db 01000000b
    db 01000000b
    db 00000000b

    ; ASCII 81: 'Q'
    db 00111100b
    db 01000010b
    db 01000010b
    db 01000010b
    db 01010010b
    db 01001010b
    db 00111100b
    db 00000010b

    ; ASCII 82: 'R'
    db 01111100b
    db 01000010b
    db 01000010b
    db 01111100b
    db 01001000b
    db 01000100b
    db 01000010b
    db 00000000b

    ; ASCII 83: 'S'
    db 00111100b
    db 01000010b
    db 01000000b
    db 00111100b
    db 00000010b
    db 01000010b
    db 00111100b
    db 00000000b

    ; ASCII 84: 'T' 
    db 01111100b
    db 00010000b
    db 00010000b
    db 00010000b
    db 00010000b
    db 00010000b
    db 00010000b
    db 00000000b

    ; ASCII 85: 'U'
    db 01000010b
    db 01000010b
    db 01000010b
    db 01000010b
    db 01000010b
    db 01000010b
    db 00111100b
    db 00000000b

    ; ASCII 86: 'V'
    db 01000010b
    db 01000010b
    db 01000010b
    db 01000010b
    db 01000010b
    db 00100100b
    db 00011000b
    db 00000000b

    ; ASCII 87: 'W' 
    db 01000010b
    db 01000010b
    db 01000010b
    db 01011010b
    db 01100110b
    db 01000010b
    db 01000010b
    db 00000000b

    ; ASCII 88: 'X' 
    db 01000010b
    db 01000010b
    db 00100100b
    db 00011000b
    db 00100100b
    db 01000010b
    db 01000010b
    db 00000000b

    ; ASCII 89: 'Y' 
    db 01000100b
    db 01000100b
    db 00101000b
    db 00010000b
    db 00010000b
    db 00010000b
    db 00010000b
    db 00000000b

    ; ASCII 90: 'Z'
    db 01111110b
    db 00000010b
    db 00000100b
    db 00001000b
    db 00010000b
    db 00100000b
    db 01111110b
    db 00000000b

%endif ; FONT_INC
