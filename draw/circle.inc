; ============================================================
; circle.inc - Hollow circle drawing
; dos-paint-lib | draw module
;
; Usage:
;   DrawCircle x, y, color_code, radius
;
; Examples:
;   DrawCircle 160, 100, 15, 50     ; white circle at center, radius 50
;   DrawCircle 50,  50,  12, 20     ; red circle at (50,50), radius 20
;
; Clobbers : AX, CX, DX, SI, DI, BX
; Requires : ES = 0A000h  (call InitVGA first)
; ============================================================

%ifndef CIRCLE_INC
%define CIRCLE_INC
%include "core/vgaoffset.inc"

; ----------------------------------------------------------
; Macro: DrawCircle x, y, color_code, radius
; ----------------------------------------------------------
%macro DrawCircle 4
    mov cx, %1          ; cx = x position (center)
    mov dx, %2          ; dx = y position (center)
    mov bl, %3          ; bl = color code
    mov si, %4          ; si = radius
    call _DrawCircle
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_DrawCircle:
    push bp
    mov bp, sp
    sub sp, 10          ; allocate 10 bytes for local variables on stack

    ; Local variables mapping:
    ; [bp-2]  = xc (center X)
    ; [bp-4]  = yc (center Y)
    ; [bp-6]  = x  (current X offset, starts at radius)
    ; [bp-8]  = y  (current Y offset, starts at 0)
    ; [bp-10] = P  (decision parameter)

    mov [bp-2], cx      ; store xc
    mov [bp-4], dx      ; store yc
    mov [bp-6], si      ; x = radius
    mov word [bp-8], 0  ; y = 0

    ; Initial decision parameter: P = 1 - radius
    mov ax, 1
    sub ax, si
    mov [bp-10], ax     ; store P

_circle_loop:
    mov ax, [bp-6]      ; ax = x
    cmp ax, [bp-8]      ; compare x with y
    jl _circle_done     ; if x < y, the 1/8th octant is complete

    ; Draw the 8 symmetrical points for current (x, y) offset
    call _Circle8Points

    inc word [bp-8]     ; y++

    cmp word [bp-10], 0 ; check if P > 0
    jg _circle_p_gt_0

    ; P <= 0 condition:
    ; P = P + 2y + 1
    mov ax, [bp-8]      ; ax = y
    shl ax, 1           ; ax = 2y
    inc ax              ; ax = 2y + 1
    add [bp-10], ax     ; P += 2y + 1
    jmp _circle_loop

_circle_p_gt_0:
    ; P > 0 condition:
    dec word [bp-6]     ; x--
    
    ; P = P + 2y - 2x + 1
    mov ax, [bp-8]      ; ax = y
    shl ax, 1           ; ax = 2y
    mov cx, [bp-6]      ; cx = x
    shl cx, 1           ; cx = 2x
    sub ax, cx          ; ax = 2y - 2x
    inc ax              ; ax = 2y - 2x + 1
    add [bp-10], ax     ; P += 2y - 2x + 1
    jmp _circle_loop

_circle_done:
    mov sp, bp
    pop bp
    ret

; ----------------------------------------------------------
; Internal helper procedure: Draw 8 symmetrical points
; ----------------------------------------------------------
_Circle8Points:
    ; (xc + x, yc + y)
    mov cx, [bp-2]
    add cx, [bp-6]
    mov dx, [bp-4]
    add dx, [bp-8]
    call _GetVGAOffset
    mov [es:di], bl

    ; (xc - x, yc + y)
    mov cx, [bp-2]
    sub cx, [bp-6]
    mov dx, [bp-4]
    add dx, [bp-8]
    call _GetVGAOffset
    mov [es:di], bl

    ; (xc + x, yc - y)
    mov cx, [bp-2]
    add cx, [bp-6]
    mov dx, [bp-4]
    sub dx, [bp-8]
    call _GetVGAOffset
    mov [es:di], bl

    ; (xc - x, yc - y)
    mov cx, [bp-2]
    sub cx, [bp-6]
    mov dx, [bp-4]
    sub dx, [bp-8]
    call _GetVGAOffset
    mov [es:di], bl

    ; (xc + y, yc + x)
    mov cx, [bp-2]
    add cx, [bp-8]
    mov dx, [bp-4]
    add dx, [bp-6]
    call _GetVGAOffset
    mov [es:di], bl

    ; (xc - y, yc + x)
    mov cx, [bp-2]
    sub cx, [bp-8]
    mov dx, [bp-4]
    add dx, [bp-6]
    call _GetVGAOffset
    mov [es:di], bl

    ; (xc + y, yc - x)
    mov cx, [bp-2]
    add cx, [bp-8]
    mov dx, [bp-4]
    sub dx, [bp-6]
    call _GetVGAOffset
    mov [es:di], bl

    ; (xc - y, yc - x)
    mov cx, [bp-2]
    sub cx, [bp-8]
    mov dx, [bp-4]
    sub dx, [bp-6]
    call _GetVGAOffset
    mov [es:di], bl
    ret

%endif ; CIRCLE_INC
