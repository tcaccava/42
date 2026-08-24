global ft_read

extern __errno_location

segment .text

ft_read :
    mov rax, 0 ; un alternativa con opcode piu' piccolo e' direttamente xor eax, eax
    syscall
    cmp rax, -4095
    jae .error
    ret

.error :
    neg rax
    push rax
    call __errno_location wrt ..plt
    pop rdx
    mov [rax], edx
    mov rax, -1
    ret

; codice sostanzialmente speculare a quello di ft_write,cambia unicamente l'id syscall che viene caricato in rax
