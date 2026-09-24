global main

extern open
extern strcmp
extern dprintf
extern close
extern fork
extern execve
extern exit
extern waitpid

section .data
    runs: dw 5

    next_file: db "Sully_ .s", 0
    obj_file : db "Sully_ .o", 0
    exename:   db "./Sully_ ", 0

    nasm_path: db "/usr/bin/nasm", 0
    gcc_path:  db "/usr/bin/gcc", 0

    nasm_cmd:  db "nasm", 0
    f_flag:    db "-f", 0
    elf_flag:  db "elf64", 0
    o_flag:    db "-o", 0
    gcc_cmd:   db "gcc", 0

    nasm_args: dq nasm_cmd, f_flag, elf_flag, next_file, o_flag, obj_file, 0
    gcc_args:  dq gcc_cmd, obj_file, o_flag, exename, 0
    exec_args: dq exename, 0

section .rodata
    filename: db __FILE__, 0
    sully_orig: db "Sully.s", 0

    src_code: db "global main%1$c%1$cextern open%1$cextern strcmp%1$cextern dprintf%1$cextern close%1$cextern fork%1$cextern execve%1$cextern exit%1$cextern waitpid%1$c%1$csection .data%1$c    runs: dw %4$d%1$c%1$c    next_file: db %2$cSully_ .s%2$c, 0%1$c    obj_file : db %2$cSully_ .o%2$c, 0%1$c    exename:   db %2$c./Sully_ %2$c, 0%1$c%1$c    nasm_path: db %2$c/usr/bin/nasm%2$c, 0%1$c    gcc_path:  db %2$c/usr/bin/gcc%2$c, 0%1$c%1$c    nasm_cmd:  db %2$cnasm%2$c, 0%1$c    f_flag:    db %2$c-f%2$c, 0%1$c    elf_flag:  db %2$celf64%2$c, 0%1$c    o_flag:    db %2$c-o%2$c, 0%1$c    gcc_cmd:   db %2$cgcc%2$c, 0%1$c%1$c    nasm_args: dq nasm_cmd, f_flag, elf_flag, next_file, o_flag, obj_file, 0%1$c    gcc_args:  dq gcc_cmd, obj_file, o_flag, exename, 0%1$c    exec_args: dq exename, 0%1$c%1$csection .rodata%1$c    filename: db __FILE__, 0%1$c    sully_orig: db %2$cSully.s%2$c, 0%1$c%1$c    src_code: db %2$c%3$s%2$c, 0%1$c%1$csection .text%1$c%1$cmain:%1$c    push r12%1$c    push r13%1$c    push r14%1$c    push r15%1$c    push rbp%1$c%1$c    mov rbp, rdx%1$c    mov r12w, [rel runs]%1$c%1$c    lea rdi, [rel filename]%1$c    lea rsi, [rel sully_orig]%1$c    call strcmp wrt ..plt%1$c%1$c    test eax, eax%1$c    jnz .is_a_clone%1$c    jz .not_a_clone%1$c%1$c.invalid_fd:%1$c    pop rbp%1$c    pop r15%1$c    pop r14%1$c    pop r13%1$c    pop r12%1$c    mov eax, 1%1$c    ret%1$c%1$c.last_run:%1$c    pop rbp%1$c    pop r15%1$c    pop r14%1$c    pop r13%1$c    pop r12%1$c    xor rax, rax%1$c    ret%1$c%1$c.is_a_clone:%1$c    dec r12w%1$c    mov [rel runs], r12w%1$c%1$c.not_a_clone:%1$c    cmp word [rel runs], 0%1$c    jl .last_run%1$c%1$c    mov ax, [rel runs]%1$c    add al, 48%1$c    mov [rel next_file + 6], al%1$c    mov [rel obj_file + 6], al%1$c    mov [rel exename + 8], al%1$c%1$c    lea rdi, [rel next_file]%1$c    mov rsi, 0x241%1$c    mov rdx, 0o770%1$c    xor eax, eax%1$c    call open wrt ..plt%1$c    cmp eax, 0%1$c    jl .invalid_fd%1$c%1$c    mov r13d, eax%1$c%1$c    mov rdi, r13%1$c    lea rsi, [rel src_code]%1$c    mov rdx, 10%1$c    mov rcx, 34%1$c    lea r8,  [rel src_code]%1$c    movzx r9, word [rel runs]%1$c    xor eax, eax%1$c    call dprintf wrt ..plt%1$c%1$c    mov rdi, r13%1$c    call close wrt ..plt%1$c%1$c    call fork wrt ..plt%1$c    test rax, rax%1$c    jnz .wait_nasm%1$c%1$c    lea rdi, [rel nasm_path]%1$c    lea rsi, [rel nasm_args]%1$c    mov rdx, rbp%1$c    call execve wrt ..plt%1$c    mov rdi, 1%1$c    call exit wrt ..plt%1$c%1$c.wait_nasm:%1$c    mov rdi, rax%1$c    xor rsi, rsi%1$c    xor rdx, rdx%1$c    call waitpid wrt ..plt%1$c%1$c    call fork wrt ..plt%1$c    test rax, rax%1$c    jnz .wait_gcc%1$c%1$c    lea rdi, [rel gcc_path]%1$c    lea rsi, [rel gcc_args]%1$c    mov rdx, rbp%1$c    call execve wrt ..plt%1$c    mov rdi, 1%1$c    call exit wrt ..plt%1$c%1$c.wait_gcc:%1$c    mov rdi, rax%1$c    xor rsi, rsi%1$c    xor rdx, rdx%1$c    call waitpid wrt ..plt%1$c%1$c    call fork wrt ..plt%1$c    test rax, rax%1$c    jnz .wait_sully%1$c%1$c    lea rdi, [rel exename]%1$c    lea rsi, [rel exec_args]%1$c    mov rdx, rbp%1$c    call execve wrt ..plt%1$c    mov rdi, 1%1$c    call exit wrt ..plt%1$c%1$c.wait_sully:%1$c    mov rdi, rax%1$c    xor rsi, rsi%1$c    xor rdx, rdx%1$c    call waitpid wrt ..plt%1$c    jmp .last_run%1$c", 0

section .text

main:
    push r12
    push r13
    push r14
    push r15
    push rbp

    mov rbp, rdx
    mov r12w, [rel runs]

    lea rdi, [rel filename]
    lea rsi, [rel sully_orig]
    call strcmp wrt ..plt

    test eax, eax
    jnz .is_a_clone
    jz .not_a_clone

.invalid_fd:
    pop rbp
    pop r15
    pop r14
    pop r13
    pop r12
    mov eax, 1
    ret

.last_run:
    pop rbp
    pop r15
    pop r14
    pop r13
    pop r12
    xor rax, rax
    ret

.is_a_clone:
    dec r12w
    mov [rel runs], r12w

.not_a_clone:
    cmp word [rel runs], 0
    jl .last_run

    mov ax, [rel runs]
    add al, 48
    mov [rel next_file + 6], al
    mov [rel obj_file + 6], al
    mov [rel exename + 8], al

    lea rdi, [rel next_file]
    mov rsi, 0x241
    mov rdx, 0o770
    xor eax, eax
    call open wrt ..plt
    cmp eax, 0
    jl .invalid_fd

    mov r13d, eax

    mov rdi, r13
    lea rsi, [rel src_code]
    mov rdx, 10
    mov rcx, 34
    lea r8,  [rel src_code]
    movzx r9, word [rel runs]
    xor eax, eax
    call dprintf wrt ..plt

    mov rdi, r13
    call close wrt ..plt

    call fork wrt ..plt
    test rax, rax
    jnz .wait_nasm

    lea rdi, [rel nasm_path]
    lea rsi, [rel nasm_args]
    mov rdx, rbp
    call execve wrt ..plt
    mov rdi, 1
    call exit wrt ..plt

.wait_nasm:
    mov rdi, rax
    xor rsi, rsi
    xor rdx, rdx
    call waitpid wrt ..plt

    call fork wrt ..plt
    test rax, rax
    jnz .wait_gcc

    lea rdi, [rel gcc_path]
    lea rsi, [rel gcc_args]
    mov rdx, rbp
    call execve wrt ..plt
    mov rdi, 1
    call exit wrt ..plt

.wait_gcc:
    mov rdi, rax
    xor rsi, rsi
    xor rdx, rdx
    call waitpid wrt ..plt

    call fork wrt ..plt
    test rax, rax
    jnz .wait_sully

    lea rdi, [rel exename]
    lea rsi, [rel exec_args]
    mov rdx, rbp
    call execve wrt ..plt
    mov rdi, 1
    call exit wrt ..plt

.wait_sully:
    mov rdi, rax
    xor rsi, rsi
    xor rdx, rdx
    call waitpid wrt ..plt
    jmp .last_run
