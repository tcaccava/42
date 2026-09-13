; comment outside

global main

extern printf

section .rodata
msg: db "; comment outside%1$c%1$cglobal main%1$c%1$cextern printf%1$c%1$csection .rodata%1$cmsg: db %2$c%3$s%2$c, 0%1$c%1$csection .text%1$c%1$ccall_printf:%1$c    xor rax, rax%1$c    sub rsp, 8%1$c    call printf wrt ..plt%1$c    add rsp, 8%1$c    ret%1$c%1$cmain:%1$c    ; comment inside main%1$c    lea rdi, [rel msg]%1$c    mov rsi, 10%1$c    mov rdx, 34%1$c    lea rcx, [rel msg]%1$c    sub rsp, 8%1$c    call call_printf%1$c    add rsp, 8%1$c    ret%1$c", 0

section .text

call_printf:
    xor rax, rax
    sub rsp, 8
    call printf wrt ..plt
    add rsp, 8
    ret

main:
    ; comment inside main
    lea rdi, [rel msg]
    mov rsi, 10
    mov rdx, 34
    lea rcx, [rel msg]
    sub rsp, 8
    call call_printf
    add rsp, 8
    ret
