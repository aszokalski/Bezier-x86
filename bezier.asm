section .data
    t_beg:  dq 0.0
    t_step: dq 0.1
    t_end:  dq 1.0

section .bss
    tmp_x:  resd 5  ; 5*4bytes
    tmp_y:  resd 5  ; 5*4bytes
section .text
global bezier

; Calculates (height - y - 1)*width + x
; Arguments:
; - %1: width
; - %2: height
; - %3: x
; - %4: y
; Returns to rax
%macro CALCULATE_INDEX 4
    mov rax, %2
    sub rax, %4                 ; height - y
    sub rax, 1                  ; height - y - 1
    imul rax, %1                ; (height - y - 1) * width
    add rax, %3                 ; (height - y - 1)*width + x
%endmacro

; Calculates (xb-xa)*t + xa
; Arguments:
; - %1: xa
; - %2: xb
; - %3: t register (xmm)
; Return to xa
%macro CALCULATE_INTERPOLATION 3
    ; Convert %1 to signed int
    mov eax, %1
    cdqe
    mov %1, eax

    push rax                     ; push rax

    ; Convert %2 to signed int
    mov eax, %2
    cdqe
    mov %2, eax

    pop rax                     ; load rax back from the stack

    sub %2, %1                  ; xb = xb - xa
    cvtsi2sd xmm5, %2           ; xmm1 = (double) xb

    mulsd xmm5, %3              ; xmm1 = t(xb - xa)
    cvtsd2si %2, xmm5           ; xb = (int) xmm1
    add %2, %1                  ; xb = (xb-xa)t + xa
    mov %1, %2                  ; Return to xa
%endmacro

; Draws an n-th order bezier curve on a given pixel array using given points:
; Arguments:
; - rdi = unsigned char *(1 byte) *pixelArray
; - rsi = unsigned int *(4 bytes) *pointsX
; - rdx = unsigned int *(4 bytes) *pointsY
; - rcx = int (4 bytes) n
; - r8 = int (4 bytes) pixel array width
; - r9 = int (4 bytes) pixel array height
; pixelArray[height*width] - pixels from the left bottom corner to the top right (0 to 255)
bezier:
    ; Prologue
    push	rbp
    mov		rbp, rsp
    push	rbx
    push	r12

    ; Body
    mov r13, r8                     ; r13 = width
    mov r14, r9                     ; r14 = height
    movsd xmm0, qword[rel t_beg]    ; xmm0 = t_beg
    movsd xmm1, qword[rel t_step]   ; xmm1 = t_step
    movsd xmm2, qword[rel t_end]    ; xmm2 = t_end
t_loop:
    ; Prepare for copy loop
    mov r8, rsi                     ; r8 = &pointsX
    mov r9, rdx                     ; r9 = &pointsY
    mov r10, tmp_x                  ; r10 = &tmp_x
    mov r11, tmp_y                  ; r11 = &tmp_y

    mov r12, 5                      ; counter (r12) = 5
copy_loop:
    mov eax, [r8]                   ; load from pointsX
    mov [r10], eax                  ; store to tmp_x

    mov eax, [r9]                   ; load from pointsY
    mov [r11], eax                  ; store to tmp_y

    ; Increment all pointers
    add r8, 4
    add r9, 4
    add r10, 4
    add r11, 4

    ; Decrement counter
    dec r12
    test r12, r12
    jnz copy_loop                   ; loop until counter reaches 0
end_copy_loop:
    ; Prepare for point loop
    mov r15, 4                      ; new array size = 4
point_loop:
    mov r10, tmp_x                  ; r10 = &tmp_x
    mov r11, tmp_y                  ; r11 = &tmp_y
    mov r12, r15                    ; counter (r12) = new array size
interpolation_loop:
    mov eax, [r10]                  ; eax = tmp_x[i]
    mov ebx, [r10 + 4]              ; ebx = tmp_x[i+1]
    CALCULATE_INTERPOLATION eax, ebx, xmm0  ; calculate x interpolation -> result in eax
    mov [r10], eax                  ; store the result to tmp_x[i]

    mov eax, [r11]                  ; eax = tmp_y[i]
    mov ebx, [r11 + 4]              ; ebx = tmp_y[i+1]
    CALCULATE_INTERPOLATION eax, ebx, xmm0  ; calculate x interpolation -> result in eax
    mov [r11], eax                  ; store the result to tmp_y[i]

    add r10, 4                      ; next x
    add r11, 4                      ; next y

    dec r12                         ; --counter
    test r12, r12
    jnz interpolation_loop          ; loop until counter reaches 0
end_interpolation_loop:
    dec r15                         ; --size
    test r15, r15
    jnz point_loop                  ; loop until size is not 0
end_point_loop:
    mov r10d, [r10 - 4]             ; r10d = result x (upper half will be zeroed)
    mov r11d, [r11 - 4]             ; r11d = result y (upper half will be zeroed)

    CALCULATE_INDEX r13, r14, r10, r11 ; rax = index

    mov byte [rdi + rax], 0         ; pixelArray[index] = 0

    ; Increment t
    addsd xmm0, xmm1                ; t_beg += t_step
    ucomisd xmm0, xmm2              ; compare less than
    jl t_loop                       ; t_beg < t_end -> loop_t
end_t_loop:
    ; Epilogue
	pop		r12
	pop		rbx
	mov		rsp, rbp
	pop		rbp
	ret
