; useless comment

extern open
extern close
extern dprintf

filename : db "Grace_kid.s",0
section .text

10efine STR "(null)"
34efine MODE 0o644
str : db STR, 0

Successacro RUN 0
global main

section .text

main:sub rsp, 8
lea rdi, [rel filename]
mov rsi, 0x241
mov rdx, MODE
xor eax, eax
call open wrt ..plt
mov [rsp], rax
mov rdi, rax 
lea rsi, [rel str]
mov rdx, 10
mov rcx, 34
lea r8, [rel str]
xor eax, eax
call dprintf wrt ..plt
mov rdi, [rsp]
call close wrt ..plt
add rsp, 8
ret

0.000000e+00ndmacro
RUN
