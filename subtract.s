# define some variables
BIGNUM_CHUNK = 7 # amount of bignum chunks
WORD_SIZE = 8
# syscall requests
SYS_READ = 0
SYS_WRITE = 1
SYS_EXIT = 60
# standrad I/O streams
STDIN = 0
STDOUT = 1
STDER = 2
# buffer size
BUFFER_SIZE = 40 * 1
.data
	big_value_1: # 7 lines * 64bit argument = 448 
		.8byte 0x1
		.8byte 0x2
		.8byte 0x3
		.8byte 0x4
		.8byte 0x5
		.8byte 0x6
		.8byte 0x7
	big_value_2: # 7 lines * 64bit argument = 448
		.8byte 0x0
		.8byte 0x2
		.8byte 0x3
		.8byte 0x4
		.8byte 0x5
		.8byte 0x6
		.8byte 0x8
	
.text
.global _start
.global substract

_start:
	
	movq $BIGNUM_CHUNK, %rax
	movq $big_value_1, %rbx
	movq $big_value_2, %rcx
	call substract
	pop %rax
	movq %rbp, %rsp
	movq $SYS_EXIT, %rax
	xor %rdi, %rdi
	syscall

# from first bignum argument substract second bignum argument
# rax contains sizeof currently handle bignums
# rbx contains pointer to first bignum argument
# rcx constains pointer to second bignum argument
substract:
	# clear carry flag
	clc
	# set index counter to 0
	movq $0, %rdx
	# make divison with borrow
	L1:
	# arg1 = *(big_value_1 +off)
	movq (%rbx,%rdx,WORD_SIZE), %r8
	# arg2 = *(big_value_2 + off)
	movq (%rcx,%rdx,WORD_SIZE), %r9
	# arg2 = arg2 - arg1
	sbbq %r8, %r9
	# *big_value_2 = arg2 
	movq %r9, (%rcx,%rdx,WORD_SIZE)
	# increment index
	incq %rdx
	# decrement  
	decq %rax
	jnz L1

	ret
