
section .text
global ft_strdup
extern malloc
extern ft_strlen

ft_strdup:
    push rdi
    call ft_strlen
    inc rax

    mov rdi, rax
    call malloc wrt ..plt

    test rax, rax
    jz .fail

    pop rsi
    mov rdi, rax
    push rax
    xor rax, rax
.copy_loop:
    mov cl, byte [rsi + rax]
    mov byte [rdi + rax], cl
    cmp cl, 0
    je .copy_done
    inc rax
    jmp .copy_loop
.copy_done:
    pop rax
    ret

.fail:
    add rsp, 8
    xor rax, rax
    ret