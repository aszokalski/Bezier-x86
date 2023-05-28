section .data
    t: dq 0.25
section .text
global bezier

%macro CALCULATE_INTERPOLATION 4 ; xa, xb, t, ret
    movsd xmm0, qword[rel %3]   ; xmm0 = t
    mov r8, %1                  ; r8 = xa
    mov r9, %2                  ; r9 = xb
    sub r9, r8                  ; r9 = xb - xa
    cvtsi2sd xmm1, r9           ; xmm1 = (double) r9

    mulsd xmm1, xmm0            ; xmm1 = t(xb - xa)
    cvtsd2si r9, xmm1           ; r9 = (int) xmm1
    add r9, r8                  ; r9 = (xb-xa)t + xa
    mov %4, r9                  ; return r9
%endmacro

; Draws an n-th order bezier curve on a given pixel array using given points:
; Arguments:
; - rdi = unsigned char *(1 byte) *pixelArray
; - rsi = unsigned long (4 bytes) height
; - rdx = unsigned long (4 bytes) width
; - rcx = unsigned int *(4 bytes) *pointsX
; - r8 = unsigned int *(4 bytes) *pointsY
; - r9 = int (4 bytes) n
; pixelArray[height*width] - pixels from the left bottom corner to the top right (0 to 255)

bezier:
    mov rdi, 255
    mov rsi, 8710
    CALCULATE_INTERPOLATION rdi, rsi, t, rax
  ret                 ; Return


; Draws a point on the curve for given t (0, 1) and points
; Arguments:
; - rdi = unsigned int *(4 bytes) *pointsX
; - rsi = unsigned int *(4 bytes) *pointsX
; - rdx = float t
draw_curve_point:
