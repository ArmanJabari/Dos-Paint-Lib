; ============================================================
; timing.inc - Timing and delay utilities
; dos-paint-lib | core module
;
; Usage:
;   Delay milliseconds
;   WaitVSync
;
; Examples:
;   Delay 500     ; pause 500ms
;   Delay 1000    ; pause 1 second
;
; Internally converts milliseconds to microseconds (ms * 1000)
; and passes to BIOS INT 15h AH=86h which handles the wait.
; Max delay : 65535ms (65 seconds)
;
; WaitVSync syncs loop to the VGA monitor's refresh rate 
; (usually 70Hz in Mode 13h). Prevents screen tearing.
;
; Clobbers : AX, BX, CX, DX
; ============================================================

%ifndef TIMING_INC
%define TIMING_INC

; ----------------------------------------------------------
; Macro: Delay  milliseconds
; ----------------------------------------------------------
%macro Delay 1
    mov bx, %1          ; bx = milliseconds
    call _Delay
%endmacro

; ----------------------------------------------------------
; Macro: WaitVSync
; No parameters needed
; ----------------------------------------------------------
%macro WaitVSync 0
    call _WaitVSync
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_Delay:
    mov ax, bx          ; ax = milliseconds
    xor dx, dx          ; dx = 0  (clear for multiplication)
    mov cx, 1000
    mul cx              ; dx:ax = milliseconds * 1000 (microseconds)

    mov cx, dx          ; cx = high word of microseconds
    mov dx, ax          ; dx = low word of microseconds

    mov ah, 86h
    int 15h             ; BIOS wait  cx:dx = microseconds to wait
    ret

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_WaitVSync:
    mov dx, 03DAh       ; dx = VGA Input Status Register 1 port

_vsync_low:
    in al, dx           ; read status register
    test al, 08h        ; test bit 3 (Vertical Retrace status)
    jnz _vsync_low      ; if bit 3 = 1, loop until it ends

_vsync_high:
    in al, dx           ; read status register again
    test al, 08h        ; test bit 3 again
    jz _vsync_high      ; if bit 3 = 0, loop until a new retrace begins
    
    ret

%endif ; TIMING_INC
