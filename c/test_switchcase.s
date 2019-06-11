	.file	"test_switchcase.c"
	.section	.rodata
.LC0:
	.string	"do 1"
.LC1:
	.string	"do 2"
.LC2:
	.string	"do 3"
.LC3:
	.string	"do 4"
.LC4:
	.string	"do 5"
.LC5:
	.string	"do 6"
.LC6:
	.string	"default, not do"
	.text
	.globl	func_test_switch
	.type	func_test_switch, @function
func_test_switch:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	$0, -12(%ebp)
	cmpl	$6, 8(%ebp)
	ja	.L2
	movl	8(%ebp), %eax
	sall	$2, %eax
	addl	$.L9, %eax
	movl	(%eax), %eax
	jmp	*%eax
	.section	.rodata
	.align 4
	.align 4
.L9:
	.long	.L2
	.long	.L3
	.long	.L4
	.long	.L5
	.long	.L6
	.long	.L7
	.long	.L8
	.text
.L3:
	movl	$.LC0, (%esp)
	call	puts
	jmp	.L1
.L4:
	movl	$.LC1, (%esp)
	call	puts
	jmp	.L1
.L5:
	movl	$.LC2, (%esp)
	call	puts
	jmp	.L1
.L6:
	movl	$.LC3, (%esp)
	call	puts
	jmp	.L1
.L7:
	movl	$.LC4, (%esp)
	call	puts
	jmp	.L1
.L8:
	movl	$.LC5, (%esp)
	call	puts
	jmp	.L1
.L2:
	movl	$.LC6, (%esp)
	call	puts
	nop
.L1:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	func_test_switch, .-func_test_switch
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$32, %esp
	movl	$0, 28(%esp)
	jmp	.L12
.L13:
	movl	28(%esp), %eax
	movl	%eax, (%esp)
	call	func_test_switch
	addl	$1, 28(%esp)
.L12:
	cmpl	$6, 28(%esp)
	jle	.L13
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
