global ft_list_push_front

extern malloc

struc s_list
    .data: resq 1
    .next: resq 1
endstruc

section .text

ft_list_push_front:
    test rdi, rdi              ; verifico che il puntatore passato come argomento non punti a null
    jz .exit                   ; in tal caso ritorno
    push rbx                   ; preservo rbx in quanto callee-saved
    push r12                   ; preservo r12 in quanto callee-saved
    sub rsp, 8                 ; allineamento rsp % 16 prima di malloc
    mov rbx , rdi              ; salvo begin list in rbx
    mov r12,  rsi              ; salvo data in r12
    mov rdi, s_list_size       ; sposto la size in rdi per chiamare malloc                
    call malloc wrt ..plt      ; chiamo malloc
    test rax, rax              ; verifico che la chiamata non sia fallita
    jz .cleanup                ; in caso di fallimento cleanup stack e ritorno
    mov [rax] , r12            ; faccio in modo che new_node->data punti a data
    mov rcx, [rbx]             ; sposto *begin_list in rcx
    mov [rax + s_list.next], rcx    ; faccio puntare new_node->next a *begin_list 
    mov [rbx], rax             ; faccio puntare *begin_list a new_node 
    jmp .cleanup

.cleanup :                     ; ripristino la stack con aproccio bottom-up
    add rsp, 8
    pop r12
    pop rbx

.exit :
    ret
