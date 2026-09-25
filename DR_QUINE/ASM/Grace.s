; useless comment

extern open
extern close
extern dprintf

filename : db "Grace_kid.s",0

section .text

%define STR "; useless comment%1$c%1$cextern open%1$cextern close%1$cextern dprintf%1$c%1$cfilename : db %2$cGrace_kid.s%2$c,0%1$c%1$csection .text%1$c%1$c%%define STR %2$c%3$s%2$c%1$c%%define MODE 0o644%1$cstr : db STR, 0%1$c%1$c%%macro RUN 0%1$cglobal main%1$c%1$cmain:%1$csub rsp, 8%1$clea rdi, [rel filename]%1$cmov rsi, 0x241%1$cmov rdx, MODE%1$cxor eax, eax%1$ccall open wrt ..plt%1$cmov [rsp], rax%1$cmov rdi, rax %1$clea rsi, [rel str]%1$cmov rdx, 10%1$cmov rcx, 34%1$clea r8, [rel str]%1$cxor eax, eax%1$ccall dprintf wrt ..plt%1$cmov rdi, [rsp]%1$ccall close wrt ..plt%1$cadd rsp, 8%1$cret%1$c%%endmacro%1$cRUN%1$c"
%define MODE 0o644
str : db STR, 0

%macro RUN 0
global main

main:
sub rsp, 8
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
%endmacro
RUN
