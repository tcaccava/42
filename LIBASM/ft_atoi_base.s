global has_dup
global has_forbid_char
extern ft_strlen


section .rodata 
forbidden: db " +-", 9, 10, 11, 12, 13, 0

section .text

has_dup :
    xor eax, eax
    xor edx, edx
    xor r8, r8

.loop :
    cmp byte [rdi], 0
    jz .end
    inc r8
    jmp .int_loop
    inc rdi

.int_loop :
    cmp byte[rdi + r8], 0
    jz .end_int_loop
    mov dl, byte [rdi + r8]
    cmp  dl, byte[rdi]
    jz .if_equal
    inc r8
    jmp .int_loop

.if_equal :
    mov rax, 1
    ret

.end_int_loop :
    inc rdi

.end :
    xor eax, eax
    ret



has_forbid_char :
    lea rsi, [rel forbidden]
    jmp .loop
 
.loop :
    cmp byte [rdi], 0
    jz .end
    xor eax, eax
    jmp .int_loop
    inc rdi
    jmp .loop

.int_loop :
    cmp byte [rsi + rax], 0
    jz .exit_int_loop
    mov r8b, byte[rsi + rax]
    cmp [rdi], r8b
    jz .end_int_loop
    inc rax
    jmp .int_loop

.exit_int_loop :
    inc rdi
    jmp .loop

.end_int_loop :
    mov rax, 1
    ret

.end :
    xor eax, eax
    ret
