; ============================================================
; timing.inc - Timing and delay utilities
; dos-paint-lib | core module
;
; Usage:
;   Delay milliseconds
;
; Examples:
;   Delay 500     ; pause 500ms
;   Delay 1000    ; pause 1 second
;   Delay 16      ; pause 16ms  (60fps frame pacing)
;   Delay 33      ; pause 33ms  (30fps frame pacing)
;
; Internally converts milliseconds to microseconds (ms * 1000)
; and passes to BIOS INT 15h AH=86h which handles the wait.
;
; Max delay : 65535ms (65 seconds)
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

%endif ; TIMING_INC
