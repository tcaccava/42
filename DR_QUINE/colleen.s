global main

extern printf

section .rodata

msg: db "global main",10,10,"extern printf",10,10,"section .rodata",10,10,"msg: db ","s,s",10,"section .text",10,"call_printf :",10,"    xor rax, rax",10,"    add rsp,8",10,"    call printf wrt ..plt",10,"    add rsp, 8",10,"    ret",10,10,"main :",10,"    lea rdi, [rel msg]",10,"    lea rsi, [rel msg]",10,"call call_printf",10,"    ret",0,10

section .text

call_printf :
    xor rax, rax
    pop r14
    mov rdi, r12
    mov rsi, r13
    call printf wrt ..plt
    push r14
    ret

main :
    lea rdi, [rel msg]
    lea rsi, [rel msg]
    push r12
    push r13
    push r14
    mov r12, rdi
    mov r13, rsi
    call call_printf
    pop rdi
    ret
    
ret
r14
ret
r14
r13
r12