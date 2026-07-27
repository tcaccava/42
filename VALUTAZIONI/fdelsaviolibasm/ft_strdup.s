section .text
global ft_strdup
extern malloc
extern ft_strlen
extern ft_strcpy

ft_strdup:
	push	rdi
	call	ft_strlen
	inc	rax
	mov	rdi, rax
	call	malloc wrt ..plt
	test	rax, rax
	jz	.fail
	mov	rdi, rax
	pop	rsi
	jmp	ft_strcpy
.fail:
	pop	rdi
	xor	rax, rax
	ret
