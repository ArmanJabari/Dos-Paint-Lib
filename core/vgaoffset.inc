; ============================================================
; vgaoffset.inc - VGA memory offset calculation
; dos-paint-lib | core module
;
; Usage: internal only, called by draw/render modules
;
; In  : CX = x, DX = y
; Out : DI = y * 320 + x
;
; Clobbers : AX, DI
; ============================================================

%ifndef VGAOFFSET_INC
%define VGAOFFSET_INC

_GetVGAOffset:
    mov di, dx          ; di = y position
    shl di, 8           ; di = y * 256
    mov ax, dx          ; ax = y position
    shl ax, 6           ; ax = y * 64
    add di, ax          ; di = y * 256 + y * 64 (y * 320)
    add di, cx          ; di = y * 320 + x (starting pixel offset)
    ret

%endif ; VGAOFFSET_INC