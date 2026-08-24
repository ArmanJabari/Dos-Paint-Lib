; ============================================================
; polygon.inc - Polygon hollow drawing
; dos-paint-lib | draw module
;
; Usage:
;   DrawPoly points_ptr, count, color_code
;
;   points_ptr:
;       dw x0, y0
;       dw x1, y1
;       dw x2, y2
;
; Examples:
;   DrawPoly triangle_pts, 3, 14       ; draw yellow triangle
;   DrawPoly hex_pts,      6, 15       ; draw white hexagon
;
; Clobbers : AX, BX, CX, DX, SI, DI, BP
; Requires : ES = 0A000h (call InitVGA first)
; ============================================================

%ifndef POLYGON_INC
%define POLYGON_INC

%include "src/draw/line/angled.inc"

; ----------------------------------------------------------
; Macro: DrawPoly points_ptr, count, color_code
; ----------------------------------------------------------
%macro DrawPoly 3
    mov si, %1          ; si = pointer to vertex array
    mov cx, %2          ; cx = vertex count
    mov bl, %3          ; bl = color code
    call _DrawPoly
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_DrawPoly:
    push bp
    mov bp, sp
    sub sp, 18          ; allocate 18 bytes for local variables on stack

    ; Local variables mapping:
    ; [bp-2]  = Current points array pointer
    ; [bp-4]  = Remaining points counter
    ; [bp-6]  = Color code (byte)
    ; [bp-8]  = First vertex X (x0) - saved to close polygon
    ; [bp-10] = First vertex Y (y0) - saved to close polygon
    ; [bp-12] = Current segment start X (x_start)
    ; [bp-14] = Current segment start Y (y_start)
    ; [bp-16] = Current segment end X (x_end)
    ; [bp-18] = Current segment end Y (y_end)

    mov [bp-2], si      ; store pointer to vertex array
    mov [bp-4], cx      ; store vertex count
    mov [bp-6], bl      ; store color code

    ; Validate minimum vertex count
    cmp word [bp-4], 3
    jb _poly_done

    ; Read and store the initial vertex (x0, y0)
    mov si, [bp-2]

    mov ax, [si]
    mov [bp-8], ax       ; store x0 (for closing segment)
    mov [bp-12], ax      ; store initial segment start X

    mov ax, [si+2]
    mov [bp-10], ax      ; store y0 (for closing segment)
    mov [bp-14], ax      ; store initial segment start Y

    ; Advance array pointer to next vertex and decrement remaining counter
    add word [bp-2], 4   ; next point (4 bytes: 2 words for X and Y)
    dec word [bp-4]      ; vertices left to connect

_poly_loop:
    mov si, [bp-2]

    mov ax, [si]
    mov [bp-16], ax      ; current segment end X
    mov ax, [si+2]
    mov [bp-18], ax      ; current segment end Y

    ; Draw line segment from (x_start, y_start) to (x_end, y_end)
    DrawALine [bp-12], [bp-14], [bp-16], [bp-18], [bp-6]

    ; Shift end coordinates to become next start coordinates
    mov ax, [bp-16]
    mov [bp-12], ax      ; x_start = x_end

    mov ax, [bp-18]
    mov [bp-14], ax      ; y_start = y_end

    add word [bp-2], 4   ; move to next vertex in array
    dec word [bp-4]      ; decrement vertex counter
    jnz _poly_loop       ; repeat until all consecutive vertices connected

    ; Draw final closing line from last vertex back to first vertex (x0, y0)
    DrawALine [bp-12], [bp-14], [bp-8], [bp-10], [bp-6]

_poly_done:
    mov sp, bp           ; restore stack pointer
    pop bp               ; restore base pointer
    ret

%endif ; POLYGON_INC