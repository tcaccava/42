	.file	"bit_reverse_byte_order.c"
	.text
	.globl	bit_reverse_byte_order
	.type	bit_reverse_byte_order, @function
bit_reverse_byte_order:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	sall	$24, %eax
	movl	%eax, %edx
	movl	-4(%rbp), %eax
	shrl	$24, %eax
	orl	%eax, %edx
	movl	-4(%rbp), %eax
	shrl	$8, %eax
	andl	$65280, %eax
	orl	%eax, %edx
	movl	-4(%rbp), %eax
	sall	$8, %eax
	andl	$16711680, %eax
	orl	%edx, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	bit_reverse_byte_order, .-bit_reverse_byte_order
	.section	.rodata
	.align 8
.LC0:
	.string	"--- VERIFICA SWAP BYTE (bswap32) ---\n"
.LC1:
	.string	"Input iniziale:  0x%08X\n"
.LC2:
	.string	"Output invertito: 0x%08X\n"
	.align 8
.LC3:
	.string	"\nVerifica superata: I byte sono stati invertiti correttamente."
.LC4:
	.string	"\nErrore nel calcolo."
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$-1430532899, -8(%rbp)
	movl	-8(%rbp), %eax
	movl	%eax, %edi
	call	bit_reverse_byte_order
	movl	%eax, -4(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	-8(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	cmpl	$-573785174, -4(%rbp)
	jne	.L4
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L5
.L4:
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
.L5:
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 12.3.0-1ubuntu1~22.04.3) 12.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
