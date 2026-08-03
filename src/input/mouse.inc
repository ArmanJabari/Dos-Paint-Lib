; ============================================================
; mouse.inc - Mouse input handling
; dos-paint-lib | input module
;
; Usage:
;   InitMouse          ; set up mouse driver
;   ShowMouse          ; display the mouse cursor
;   HideMouse          ; hide the mouse cursor
;   GetMouse           ; get mouse position and button status
;
;   SetMouseIcon offset, hx, hy
;   SetMouseIcon offset, hx, hy, color_code
;
; After InitMouse returns:
;   AX = FFFFh if mouse support is available
;   AX = 0000h if no mouse driver found
;
; After GetMouse returns:
;   BX = button status (Bit 0: Left, Bit 1: Right, Bit 2: Middle)
;   CX = x position
;   DX = y position
;
; Note: Always call HideMouse BEFORE drawing to the screen to
; prevent graphical corruption, then call ShowMouse after.
;
; Clobbers : AX, BX, CX, DX (depending on macro called)
; ============================================================

%ifndef MOUSE_INC
%define MOUSE_INC

%include "src/render/sprite.inc"

; ----------------------------------------------------------
; Macro: InitMouse
; No parameters needed
; ----------------------------------------------------------
%macro InitMouse 0
    call _InitMouse
%endmacro

; ----------------------------------------------------------
; Macro: ShowMouse
; No parameters needed
; ----------------------------------------------------------
%macro ShowMouse 0
    call _ShowMouse
%endmacro

; ----------------------------------------------------------
; Macro: HideMouse
; No parameters needed
; ----------------------------------------------------------
%macro HideMouse 0
    call _HideMouse
%endmacro

; ----------------------------------------------------------
; Macro: GetMouse
; No parameters needed
; ----------------------------------------------------------
%macro GetMouse 0
    call _GetMouse
%endmacro

; ----------------------------------------------------------
; Macro: SetMouseIcon offset, hx, hy
; Renders a 16x16 solid custom mouse cursor
; ----------------------------------------------------------
%macro SetMouseIcon 3
    push ax                         ; Preserve AX register
    mov ax, %1                      ; Load sprite offset address into AX
    mov word [_mouse_offset], ax    ; Store sprite offset address
    mov ax, %2                      ; Load hotspot X offset into AX
    mov word [_mouse_hx], ax        ; Store hotspot X offset
    mov ax, %3                      ; Load hotspot Y offset into AX
    mov word [_mouse_hy], ax        ; Store hotspot Y offset
    mov byte [_mouse_is_custom], 1  ; Mark mouse as using a custom icon
    mov byte [_mouse_has_key], 0    ; Disable transparent color keying
    pop ax                          ; Restore AX register
%endmacro

; ----------------------------------------------------------
; Macro: SetMouseIcon offset, hx, hy, color_code
; Renders a 16x16 transparent custom mouse cursor
; ----------------------------------------------------------
%macro SetMouseIcon 4
    push ax                         ; Preserve AX register
    mov ax, %1                      ; Load sprite offset address into AX
    mov word [_mouse_offset], ax    ; Store sprite offset address
    mov ax, %2                      ; Load hotspot X offset into AX
    mov word [_mouse_hx], ax        ; Store hotspot X offset
    mov ax, %3                      ; Load hotspot Y offset into AX
    mov word [_mouse_hy], ax        ; Store hotspot Y offset
    mov al, %4                      ; Load transparent color index into AL
    mov byte [_mouse_color_key], al ; Store color key for transparency
    mov byte [_mouse_is_custom], 1  ; Mark mouse as using a custom icon
    mov byte [_mouse_has_key], 1    ; Enable transparent color keying
    pop ax                          ; Restore AX register
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_InitMouse:
    jmp _mouse_init_start           ; Jump over internal data declarations

    _mouse_is_custom db 0           ; Flag: 1 if custom icon is active, 0 if standard
    _mouse_has_key   db 0           ; Flag: 1 if transparency key is enabled, 0 if solid
    _mouse_offset    dw 0           ; Pointer to the custom cursor sprite graphics data
    _mouse_hx        dw 0           ; Hotspot X offset for mouse alignment
    _mouse_hy        dw 0           ; Hotspot Y offset for mouse alignment
    _mouse_color_key db 0           ; Transparent color index for rendering
    _mouse_visible   db 0           ; Flag: 1 if mouse is visible, 0 if hidden

_mouse_init_start:
    mov ax, 0000h         ; BIOS mouse initialization function
    int 33h               ; initialize mouse
                          ; returns: AX = status (0000h/FFFFh)
    ret

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_ShowMouse:
    cmp byte [_mouse_visible], 1    ; Check if mouse is already visible
    je _mouse_show_do_custom        ; If visible, skip standard show and handle custom rendering

    mov byte [_mouse_visible], 1    ; Mark mouse status as visible
    
    cmp byte [_mouse_is_custom], 1  ; Check if custom mouse mode is enabled
    je _mouse_show_do_custom        ; If custom, bypass standard INT 33h show logic
    
    mov ax, 0001h         ; BIOS mouse show cursor function
    int 33h               ; display mouse pointer
    ret

_mouse_show_do_custom:
    cmp byte [_mouse_is_custom], 1  ; Confirm if custom mouse rendering is enabled
    jne _mouse_show_done            ; If not custom, skip custom drawing logic

    pusha                           ; Save general registers before rendering
    call _GetMouse                  ; Get current raw mouse coordinates (CX=X, DX=Y)
    sub cx, word [_mouse_hx]        ; Subtract hotspot X offset to align sprite origin
    sub dx, word [_mouse_hy]        ; Subtract hotspot Y offset to align sprite origin
    mov si, word [_mouse_offset]    ; Load cursor sprite data address into SI

    cmp byte [_mouse_has_key], 1    ; Check if transparency key is enabled
    je _mouse_show_with_key         ; Jump to transparent sprite rendering
    
    RenderSprite cx, dx, 16, 16, si ; Render solid 16x16 custom cursor sprite
    jmp _mouse_show_custom_done     ; Finish custom rendering process

_mouse_show_with_key:
    mov al, byte [_mouse_color_key] ; Load transparent color key into AL
    RenderSprite cx, dx, 16, 16, si, al ; Render 16x16 sprite with transparent key color

_mouse_show_custom_done:
    popa                            ; Restore saved general registers
_mouse_show_done:
    ret                             ; Return from _ShowMouse procedure

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_HideMouse:
    mov byte [_mouse_visible], 0    ; Update status flag to reflect hidden state
    
    cmp byte [_mouse_is_custom], 1  ; Check if using a custom cursor
    je _mouse_hide_done             ; Skip INT 33h hide driver call if custom cursor
    
    mov ax, 0002h         ; BIOS mouse hide cursor function
    int 33h               ; remove mouse pointer from screen
_mouse_hide_done:
    ret

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_GetMouse:
    mov ax, 0003h         ; BIOS mouse get position and button status
    int 33h               ; returns: BX, CX, DX
    
    shr cx, 1             ; Divide X by 2 to map 640 to 320
    ret

%endif ; MOUSE_INC
