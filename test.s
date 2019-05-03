	.text
	.file	"test.c"
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	$0, -4(%rbp)
	movq	$1, -16(%rbp)
	movq	$2, -24(%rbp)
	movslq	k, %rax
	movq	%rax, -32(%rbp)
	movq	-16(%rbp), %rax
	andq	-32(%rbp), %rax
	cmpq	$0, %rax
	je	.LBB0_2
# %bb.1:
	movq	-24(%rbp), %rax
	andq	-32(%rbp), %rax
	cmpq	$0, %rax
	je	.LBB0_4
.LBB0_2:
	movq	-24(%rbp), %rax
	andq	-32(%rbp), %rax
	cmpq	$0, %rax
	je	.LBB0_5
# %bb.3:
	movq	-16(%rbp), %rax
	andq	-32(%rbp), %rax
	cmpq	$0, %rax
	jne	.LBB0_5
.LBB0_4:
	movq	-16(%rbp), %rax
	movq	%rax, -24(%rbp)
.LBB0_5:
	xorl	%eax, %eax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	k,@object               # @k
	.data
	.p2align	2
k:
	.long	1188386                 # 0x122222
	.size	k, 4


	.ident	"clang version 7.0.0-3 (tags/RELEASE_700/final)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym main
	.addrsig_sym k
