; ============================================================
; paintlib.inc - Master include file
; ============================================================

%ifndef PAINTLIB_INC
%define PAINTLIB_INC

; ----------------------------------------------------------
; Bypass library procedures to prevent accidental execution
; ----------------------------------------------------------
jmp _paintlib_skip

; === Core Modules ===
%include "src/core/vga/main.inc"
%include "src/core/vga/offset.inc"
%include "src/core/vga/palette.inc"
%include "src/core/timing.inc"

; === Draw Modules ===
%include "src/draw/pixel.inc"
%include "src/draw/line/horizontal.inc"
%include "src/draw/line/vertical.inc"
%include "src/draw/line/angled.inc"
%include "src/draw/rect.inc"
%include "src/draw/circle.inc"

; === Render Modules ===
%include "src/render/font.inc"
%include "src/render/sprite.inc"
%include "src/render/text.inc"

; === Input Modules ===
%include "src/input/keyboard.inc"
%include "src/input/mouse.inc"

; === Screen Modules ===
%include "src/screen/clear.inc"
%include "src/screen/DB.inc"

; ----------------------------------------------------------
; End of bypass jump
; ----------------------------------------------------------
_paintlib_skip:

%endif ; PAINTLIB_INC