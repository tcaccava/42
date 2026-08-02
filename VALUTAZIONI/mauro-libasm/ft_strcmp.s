
section .text
global ft_strcmp

ft_strcmp:
    ; incoming: rdi = s1, rsi = s2
    xor rax, rax
.loop:
    mov cl, byte [rdi + rax]
    mov dl, byte [rsi + rax]
    cmp cl, dl
    jne .diff
    cmp cl, 0
    je .end
    inc rax
    jmp .loop
.diff:
    movzx rax, cl
    movzx rdx, dl
    sub rax, rdx
    ret
.end:
    xor rax, rax
    ret