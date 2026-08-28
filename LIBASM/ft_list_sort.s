global ft_list_sort

struc s_list
    .data: resq 1
    .next: resq 1
endstruc

section .text

ft_list_sort :
    test rdi , rdi          ; verifico che begin_list non sia NULL
    jz .null_pointers       ; in tal caso return
    cmp qword [rdi], 0            ; verifico che *begin_list non sia NULL
    jz .null_pointers       ; in tal caso return 
    push rbx                ; salvo rbx in stack
    push r12                ; salvo r12 in stack
    push r13                ; salvo r13 in stack
    push r14                ; salvo r14 in stack
    sub rsp, 8              ; allineamento rsp % 16
    mov r14, [rdi]          ; r14 = *begin_list
    mov rbx, r14             ; rbx = *curr = *begin_list 
    mov r12, rsi            ; r12 = *cmp
    
.loop :
    mov r13, [rbx + s_list.next]   ; r13 = curr->next
    test r13, r13           ; verifico che curr->next != NULL
    jz .end_loop                ; in caso contrario ritorno
    mov rdi, [rbx]          ; curr->data = rdi
    mov rsi, [r13]          ; curr->next->data = rsi 
    
    call r12                ; chiamo cmp sui due argomenti precedenti 
    cmp eax, 0              ; verifico il return value di cmp
    jg .swap                ; se > 0 allora devo swappare i data
    mov rbx, r13            ; curr = curr->next
    jmp .loop

.swap :
    mov rcx, [rbx]          ; tmp = curr->data
    mov r8, [r13]           ; appoggio temporaneamente curr->next->data in r8
    mov [rbx] , r8          ; curr->data = curr->next->data
    mov [r13], rcx          ; curr->next->data = tmp
    mov rbx , r14           ; curr = *begin_list
    jmp .loop               ; ritorno nel loop dall'inizio

.end_loop :                      ; ripristino stack e ritorno
    add rsp, 8
    pop r14
    pop r13
    pop r12
    pop rbx
    ret

.null_pointers :
    ret