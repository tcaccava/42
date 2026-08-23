global ft_read

export __errno_location

segment .text

ft_read :
    mov rax, 2
    syscall
    cmp rax, -4095
    jae .error
    ret

.error :
    neg rax
    push rax
    call __errno_location wrt ..plt
    pop rdx
    mov [rdx], eax