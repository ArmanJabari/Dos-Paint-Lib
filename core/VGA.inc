; ============================================================
; VGA.inc - VGA screen initialization and cleanup
; dos-paint-lib | core module
;
; Usage:
;   InitVGA          ; set mode 13h + point ES to VGA memory
;   ExitVGA          ; restore text mode
;
; Call InitVGA once at the START of your program.
; Call ExitVGA  once at the END   of your program.
;
; After InitVGA:
;   - Screen is 320x200 pixels, 256 colors
;   - ES is set to 0A000h (VGA framebuffer)
;   - All draw/ functions are ready to use
;
; Clobbers : AX (InitVGA and ExitVGA)
; ============================================================

%ifndef VGA_INC
%define VGA_INC

; ----------------------------------------------------------
; Macro: InitVGA
; No parameters needed
; ----------------------------------------------------------
%macro InitVGA 0
    call _InitVGA
%endmacro

; ----------------------------------------------------------
; Macro: ExitVGA
; No parameters needed
; ----------------------------------------------------------
%macro ExitVGA 0
    call _ExitVGA
%endmacro

; ----------------------------------------------------------
; Internal procedure: set VGA mode 13h
; ----------------------------------------------------------
_InitVGA:
    mov ax, 0013h       ; VGA mode 13h (320x200, 256 colors)
    int 10h             ; BIOS video interrupt

    mov ax, 0A000h      ; VGA framebuffer segment
    mov es, ax          ; ES now points to screen memory
    ret

; ----------------------------------------------------------
; Internal procedure: restore text mode
; ----------------------------------------------------------
_ExitVGA:
    mov ax, 0003h       ; text mode 3 (80x25)
    int 10h             ; BIOS video interrupt
    ret

%endif ; VGA_INC
