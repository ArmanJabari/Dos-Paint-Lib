; ============================================================
; circle.inc - Hollow and Filled circle drawing
; dos-paint-lib | draw module
;
; Usage:
;   DrawCircle x, y, color_code, radius, fill
;
; Examples:
;   DrawCircle 160, 100, 15, 50, 1     ; white filled circle at center, radius 50
;   DrawCircle 50,  50,  12, 20, 0     ; red hollow circle at (50,50), radius 20
;
; Clobbers : AX, CX, DX, SI, DI, BX
; Requires : ES = 0A000h  (call InitVGA first)
; ============================================================

%ifndef CIRCLE_INC
%define CIRCLE_INC
%include "src/core/vga/offset.inc"
%include "src/draw/line/horizontal.inc"

; ----------------------------------------------------------
; Macro: DrawCircle x, y, color_code, radius, fill
; ----------------------------------------------------------
%macro DrawCircle 5
    mov cx, %1          ; cx = x position (center)
    mov dx, %2          ; dx = y position (center)
    mov bl, %3          ; bl = color code
    mov si, %4          ; si = radius
    mov bh, %5          ; bh = fill flag (0 = hollow, 1 = filled)
    call _DrawCircle
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_DrawCircle:
    push bp
    mov bp, sp
    sub sp, 12          ; allocate 12 bytes for local variables on stack

    ; Local variables mapping:
    ; [bp-2]  = xc (center X)
    ; [bp-4]  = yc (center Y)
    ; [bp-6]  = x  (current X offset, starts at radius)
    ; [bp-8]  = y  (current Y offset, starts at 0)
    ; [bp-10] = P  (decision parameter)
    ; [bp-12] = Fill flag

    mov [bp-2], cx      ; store xc
    mov [bp-4], dx      ; store yc
    mov [bp-6], si      ; x = radius
    mov word [bp-8], 0  ; y = 0
    mov byte [bp-12], bh ; store fill flag

    ; Initial decision parameter: P = 1 - radius
    mov ax, 1
    sub ax, si
    mov [bp-10], ax     ; store P

_circle_loop:
    mov ax, [bp-6]      ; ax = x
    cmp ax, [bp-8]      ; compare x with y
    jl _circle_done     ; if x < y, the 1/8th octant is complete

    ; Check if we need to fill the circle or draw hollow points
    cmp byte [bp-12], 0
    jne _do_fill
    
    ; Hollow circle: Draw the 8 symmetrical points
    call _Circle8Points
    jmp _circle_update_p

_do_fill:
    ; Filled circle: Draw horizontal lines connecting the points
    call _CircleFillLines

_circle_update_p:
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
; Internal helper procedure: Draw 4 horizontal lines to fill
; ----------------------------------------------------------
_CircleFillLines:
    ; 1) Line from (xc - x) to (xc + x) at Y = yc + y
    mov cx, [bp-2]
    sub cx, [bp-6]      ; cx = xc - x (start x)
    mov dx, [bp-4]
    add dx, [bp-8]      ; dx = yc + y (y pos)
    mov si, [bp-6]      ; x
    shl si, 1
    inc si              ; si = 2x + 1 (length)
    call _DrawHLine

    ; 2) Line from (xc - x) to (xc + x) at Y = yc - y
    mov cx, [bp-2]
    sub cx, [bp-6]      ; cx = xc - x (start x)
    mov dx, [bp-4]
    sub dx, [bp-8]      ; dx = yc - y (y pos)
    mov si, [bp-6]      ; x
    shl si, 1
    inc si              ; si = 2x + 1 (length)
    call _DrawHLine

    ; 3) Line from (xc - y) to (xc + y) at Y = yc + x
    mov cx, [bp-2]
    sub cx, [bp-8]      ; cx = xc - y (start x)
    mov dx, [bp-4]
    add dx, [bp-6]      ; dx = yc + x (y pos)
    mov si, [bp-8]      ; y
    shl si, 1
    inc si              ; si = 2y + 1 (length)
    call _DrawHLine

    ; 4) Line from (xc - y) to (xc + y) at Y = yc - x
    mov cx, [bp-2]
    sub cx, [bp-8]      ; cx = xc - y (start x)
    mov dx, [bp-4]
    sub dx, [bp-6]      ; dx = yc - x (y pos)
    mov si, [bp-8]      ; y
    shl si, 1
    inc si              ; si = 2y + 1 (length)
    call _DrawHLine

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
