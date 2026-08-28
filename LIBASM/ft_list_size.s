global ft_list_size

struc s_list
    .data: resq 1
    .next: resq 1
endstruc

section .text

ft_list_size :
    xor eax, eax    ; resetto il registro di ritorno

.loop :
    test rdi, rdi   ; verifico che il puntatore alla lista non sia NULL 
    jz .exit        ; se Null ritorno immediato
    mov rdi, qword [rdi + s_list.next]  ; dereferenzio il puntatore beginlist ad offset + 8
                                        ; cosi' da ottenere il puntatore a next,che sposto in rdi
                                        ; in tal modo begin_list = begin_list->next
    inc rax                             ; incremento il contatore 
    jmp .loop                           ; rientro nel loop

.exit :
    ret
