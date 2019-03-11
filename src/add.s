.extern puts
.extern printf

SYS_WRITE = 1
SYS_EXIT = 60
STDOUT = 1

TEST_A_HIGH = 0x0000000200000000
TEST_A_LOW  = 0x8000000000000000
TEST_B_HIGH = 0x0003800000000000
TEST_B_LOW  = 0x8000000000000000

.section .data
HELLO_STR: .asciz "Hello world\n"
HELLO_STR_LEN = . - HELLO_STR
FORMAT: .asciz "0x%016lx 0x%016lx\n"

TEST_BIGNUM_A:
	.8byte TEST_A_LOW
	.8byte TEST_A_HIGH
TEST_BIGNUM_B:
	.8byte TEST_B_LOW
	.8byte TEST_B_HIGH

.section .text
.global asm_main
asm_main:
	leaq HELLO_STR(%rip), %rdi
	call puts 

	movq $TEST_A_HIGH, %rax
	movq $TEST_A_LOW, %rbx
	movq $TEST_B_HIGH, %rcx
	movq $TEST_B_LOW, %rdx
	call add128

	leaq TEST_BIGNUM_A(%rip), %rsi
	leaq TEST_BIGNUM_B(%rip), %rdi
	movq $2, %rcx
	call add_bignum

	movq $SYS_EXIT, %rax
	#movq $0, %rdi # error code
	xor %rdi, %rdi
	syscall

loop:
	jmp loop

# arguments passed in rax:rbx and rcx:rdx
# return value in rax:rbx
add128:
	addq %rdx, %rbx
	adcq %rcx, %rax
	ret

# Adds two bignums, stores results in second one
# rdi = i64 *bignum1
# rsi = i64 *bignum2
# rdx = i64 bignum_size in 64 bit chunks
# return value under rax
# discards: r8, r9, rbx
.global add_bignum
add_bignum:
	xor %rbx, %rbx # also clear carry flag
	# r10 now contains pointer to the end of bignum
	add_bignum_addloop:
		movq (%rsi, %rbx, 8), %r8
		adcq %r8, (%rdi, %rbx, 8)

		incq %rbx
		decq %rdx
		jnz add_bignum_addloop
	
	movq %rdi, %rax
	ret

# null terminated string on stack
#puts:
#	movq 8(%rsp), %rsi
#	movq %rsi, %rdx
#
#puts_str_len:
#	movb (%rdx), %al
#	addq $1, %rdx
#	test %al, %al
#	jnz puts_str_len
#
#	movq $SYS_WRITE, %rax
#	movq $STDOUT, %rbx
#	# rsi already loaded
#	subq %rsi, %rdx
#	syscall
#	ret $8
