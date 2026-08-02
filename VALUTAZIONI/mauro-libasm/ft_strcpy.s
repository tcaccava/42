

section .text
global ft_strcpy

ft_strcpy:
    xor rax, rax
.loop:
    mov cl, byte [rsi + rax]
    mov byte [rdi + rax], cl
    cmp cl, 0
    je .end
    inc rax
    jmp .loop
.end:
    mov rax, rdi
    ret