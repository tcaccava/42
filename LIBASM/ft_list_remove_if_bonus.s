global ft_list_remove_if

struc s_list
    .data: resq 1
    .next: resq 1
endstruc

extern free               ; mi servira' per deallocare il nodo  

section .text

ft_list_remove_if :
    test rdi, rdi         ; controllo che begin_list != NULL
    jz .return            ; in caso contrario return
    cmp qword [rdi], 0    ; controllo che *begin_list != NULL
    jz .return            ; in caso contrario ritorno
    push rbx              ; preservo i registri callee-saved che mi serviranno
    push r12
    push r13
    push r14
    push r15
    mov rbx, rdi          ; rbx = curr = begin_list
    mov r12, rsi          ; r12 = data ref 
    mov r13 , rdx         ; r13 = cmp
    mov r14 , rcx         ; r14 = free_fct

.loop :
    cmp qword [rbx], 0     ;*curr == NULL ?
    jz .end                ; ret
    mov rax , [rbx]        ; rax = *curr 
    mov rdi, [rax]         ; rdi = *curr->data
    mov rsi, r12           ; rsi = data_ref
    call r13               ; call cmp
    test rax, rax          ; verifico che cmp non dia zero,caso in cui (*curr)->data e dataref sono uguali
    jz .remove_node        ; in caso di match rimuovo il nodo
    mov rdi, [rbx]         ; rdi = *curr
    lea rbx, [rdi + s_list.next] ; rbx = curr = &(*curr)->next
    jmp .loop              ; ritorno nel loop

.remove_node :
    mov r15, [rbx]         ; r15 = *tmp = *curr 
    mov r8, [r15 + s_list.next]   ; r8 = *curr->next
    mov [rbx] , r8         ; *curr = *curr->next
    mov rdi, [r15]         ; rdi = tmp->data
    call r14               ; call free_fct
    mov rdi, r15           ; rdi = tmp
    call free wrt ..plt    ; rdi = tmp->data
    jmp .loop

.return :
    ret

.end :                     ; ripristino stack e ritorno
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    ret