global main

extern open
extern strcmp
extern dprintf
extern close
extern fork
extern execve
extern exit
extern waitpid
extern wait

section .data
    runs: dw 5
    next_file: db "Sully_ .s", 0

section .rodata
    filename: db %__FILE__, 0
    sully_orig: db "Sully.s", 0
    src_code : db "global main%1$c%1$cextern open%1$cextern strcmp%1$cextern dprintf%1$cextern close%1$cextern fork%1$cextern execve%1$cextern exit%1$cextern waitpid%1$cextern wait%1$c%1$csection .data%1$c    runs: dw 5%1$c    next_file: db %2$cSully_ .s%2$c, 0%1$c%1$csection .rodata%1$c    filename: db %__FILE__,%1$csully_orig: db %2$cSully.s%2$c, 0%1$csection .text%1$c%1$cmain:", 0
    
section .text

main:
    push r12          ; runs
    push r13          ; filename
    push r14          ; next file + 6
    mov r12w, [rel runs]  ; runs su r12
    mov rcx, [rel filename]
    mov rdi, [rcx]
    mov rdx, [rel sully_orig]
    mov rsi, [rdx]
    call strcmp wrt ..plt     ; is the real sully
    test eax, eax
    jz .is_a_clone
    jnz .not_a_clone

.is_a_clone:
    sub r12w, 1
    mov [rel runs], r12w

.not_a_clone :
    cmp r12w, 0
    jl .last runs
    lea r14 , [rel next_file + 6]
    mov ax, [rel runs]
    add al , 48
    mov [r14], al
    lea rdi, [rel r14 - 6]
    mov rsi,  0x241
    mov rdx , 0o777
    xor eax, eax
    call open wrt ..plt
    cmp eax, 0
    jl .invalid_fd


.invalid_fd :
    mov rax, 1
    ret 

.last_run :
    xor eax, eax
    ret






