; ============================================================
; DB.inc - Double Buffering
; dos-paint-lib | screen module
;
; Usage:
;   InitDB           ; Allocates 64KB for the hidden back-buffer
;   ExitDB           ; Frees the memory back to DOS
;   BeginFrame       ; Points all drawing functions to the back-buffer
;   EndFrame         ; Waits for VSync, then copies buffer to screen
;
; Examples:
;   InitVGA
;   InitDB           ; allocate buffer
;
;   BeginFrame       ; redirect drawing to buffer
;   ClearScreen 0    ; clear the buffer
;   ...
;   EndFrame         ; flip buffer to screen
;
; Clobbers : AX, BX, CX, DI, SI
; Requires : ES = 0A000h  (call InitVGA first)
;            src/core/timing.inc (for _WaitVSync)
; ============================================================

%ifndef DB_INC
%define DB_INC

%include "src/core/timing.inc"

; ----------------------------------------------------------
; Macro: InitDB
; No parameters needed
; ----------------------------------------------------------
%macro InitDB 0
    call _InitDB
%endmacro

; ----------------------------------------------------------
; Macro: ExitDB
; No parameters needed
; ----------------------------------------------------------
%macro ExitDB 0
    call _ExitDB
%endmacro

; ----------------------------------------------------------
; Macro: BeginFrame
; No parameters needed
; ----------------------------------------------------------
%macro BeginFrame 0
    call _BeginFrame
%endmacro

; ----------------------------------------------------------
; Macro: EndFrame
; No parameters needed
; ----------------------------------------------------------
%macro EndFrame 0
    call _EndFrame
%endmacro

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_InitDB:
    mov ah, 48h         ; DOS: Allocate Memory Block function
    mov bx, 4000        ; 4000 paragraphs (4000 * 16 bytes = 64000 bytes)
    int 21h             ; Call DOS interrupt
    jc _db_alloc_fail   ; If Carry Flag (CF) is set, allocation failed
    
    mov [_db_segment], ax ; Save segment address
    clc                 ; Clear Carry Flag to indicate success
    ret

_db_alloc_fail:
    stc                 ; Set Carry Flag to let user know allocation failed
    ret

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_ExitDB:
    mov ah, 49h         ; DOS: Free Memory Block function
    mov es, [_db_segment] ; ES = The segment we want to return to DOS
    int 21h             ; Call DOS interrupt
    ret

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_BeginFrame:
    mov ax, [_db_segment]
    mov es, ax          ; ES now points to the hidden buffer memory
    ret

; ----------------------------------------------------------
; Internal procedure (do not call directly)
; ----------------------------------------------------------
_EndFrame:
    call _WaitVSync     ; Wait for monitor refresh

    push ds             ; Save Data Segment
    
    mov ax, [_db_segment]
    mov ds, ax          ; DS = hidden buffer segment
    xor si, si          ; SI = 0 (Start of back buffer)

    mov ax, 0A000h      
    mov es, ax          ; ES = physical VGA segment
    xor di, di          ; DI = 0 (Start of physical screen)

    mov cx, 32000       ; 32000 words (64000 bytes)
    cld                 ; ensure forward direction
    rep movsw           ; fast copy from DS:SI to ES:DI

    pop ds              ; Restore Data Segment
    ret

; ----------------------------------------------------------
; Internal variables (do not access directly)
; ----------------------------------------------------------
_db_segment dw 0

%endif ; DB_INC
