section .text
global ft_strcmp

ft_strcmp:
	xor	rcx, rcx
.loop:
	movzx	eax, byte [rdi + rcx]
	movzx	edx, byte [rsi + rcx]
	cmp	al, dl
	jne	.diff
	test	al, al
	jz	.done
	inc	rcx
	jmp	.loop
.diff:
	sub	eax, edx
	ret
.done:
	xor	eax, eax
	ret
