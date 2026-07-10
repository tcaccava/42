	.file	"bit_reverse_byte_order.c"
	.text
	.p2align 4
	.globl	bit_reverse_byte_order
	.type	bit_reverse_byte_order, @function
bit_reverse_byte_order:
.LFB23:
	.cfi_startproc
	endbr64
	movl	%edi, %eax
	bswap	%eax
	ret
	.cfi_endproc
.LFE23:
	.size	bit_reverse_byte_order, .-bit_reverse_byte_order
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"--- VERIFICA SWAP BYTE (bswap32) ---\n"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"Input iniziale:  0x%08X\n"
.LC2:
	.string	"Output invertito: 0x%08X\n"
	.section	.rodata.str1.8
	.align 8
.LC3:
	.string	"\nVerifica superata: I byte sono stati invertiti correttamente."
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB24:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	movl	$-1430532899, %edx
	leaq	.LC1(%rip), %rsi
	xorl	%eax, %eax
	movl	$1, %edi
	call	__printf_chk@PLT
	movl	$-573785174, %edx
	leaq	.LC2(%rip), %rsi
	xorl	%eax, %eax
	movl	$1, %edi
	call	__printf_chk@PLT
	leaq	.LC3(%rip), %rdi
	call	puts@PLT
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE24:
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
