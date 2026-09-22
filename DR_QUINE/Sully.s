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
    filename: db %__FILE__, 0
    sully_orig: db "Sully.s", 0
    
    ; Ho lasciato la tua stringa monca come richiesto
    src_code: db "global main%1$c%1$c...", 0 
    
section .text

main:
    push r12          ; runs
    push r13          ; lo usiamo per salvare l'FD invece di distruggere lo stack
    push r14          
    push r15
    push rbp          
    
    mov rbp, rdx             ; Salvo envp in rbp
    mov r12w, [rel runs]     ; runs in r12w

    ; 1. Controllo identità (Corretto LEA invece di MOV)
    lea rdi, [rel filename]
    lea rsi, [rel sully_orig]
    call strcmp wrt ..plt
    
    ; 2. strcmp ritorna 0 se uguali. Quindi se NON è zero (jnz), è un clone.
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
    
    ; 3. Aggiorno tutti i file in un colpo solo direttamente in memoria
    mov ax, [rel runs]
    add al, 48
    mov [rel next_file + 6], al
    mov [rel obj_file + 6], al
    mov [rel exename + 8], al

    ; 4. OPEN
    lea rdi, [rel next_file]  ; Passo direttamente la stringa aggiornata
    mov rsi, 0x241
    mov rdx, 0o770
    xor eax, eax
    call open wrt ..plt
    cmp eax, 0
    jl .invalid_fd
    
    mov r13d, eax             ; Salvo il File Descriptor in R13 (callee-saved)

    ; 5. DPRINTF (Corretti tutti i LEA per i puntatori a stringa e ripulito lo stack)
    mov rdi, r13              ; 1° arg: FD
    lea rsi, [rel src_code]   ; 2° arg: Formato stringa
    mov rdx, 10               ; 3° arg: \n
    mov rcx, 34               ; 4° arg: "
    lea r8,  [rel src_code]   ; 5° arg: Codice sorgente
    movzx r9, word [rel runs] ; 6° arg: Runs (esteso a 64 bit per pulizia)
    xor eax, eax
    call dprintf wrt ..plt

    ; 6. CLOSE
    mov rdi, r13              ; Riprendo l'FD da R13
    call close wrt ..plt

    ; ---------------------------------------------------------
    ; FIGLIO 1: NASM
    ; ---------------------------------------------------------
    call fork wrt ..plt
    test rax, rax
    jnz .wait_nasm

    lea rdi, [rel nasm_path]  ; LEA per gli indirizzi!
    lea rsi, [rel nasm_args]  ; LEA per gli array di puntatori!
    mov rdx, rbp
    call execve wrt ..plt
    mov rdi, 1
    call exit wrt ..plt

.wait_nasm:
    mov rdi, rax
    xor rsi, rsi
    xor rdx, rdx
    call waitpid wrt ..plt

    ; ---------------------------------------------------------
    ; FIGLIO 2: LINKING (GCC)
    ; ---------------------------------------------------------
    call fork wrt ..plt
    test rax, rax
    jnz .wait_gcc

    lea rdi, [rel gcc_path]   ; LEA!
    lea rsi, [rel gcc_args]   ; LEA!
    mov rdx, rbp
    call execve wrt ..plt
    mov rdi, 1
    call exit wrt ..plt

.wait_gcc:
    mov rdi, rax
    xor rsi, rsi
    xor rdx, rdx
    call waitpid wrt ..plt

    ; ---------------------------------------------------------
    ; FIGLIO 3: ESECUZIONE FINALE
    ; ---------------------------------------------------------
    call fork wrt ..plt
    test rax, rax
    jnz .wait_sully

    lea rdi, [rel exename]    ; Passo path intero "./Sully_X"
    lea rsi, [rel exec_args]  ; Passo array argv {"./Sully_X", NULL}
    mov rdx, rbp
    call execve wrt ..plt
    mov rdi, 1
    call exit wrt ..plt

.wait_sully: 
    xor rdi, rdi
    call wait wrt ..plt
    jmp .last_run