SYS_WRITE = 1
SYS_EXIT = 60

STDIN  = 0
STDOUT = 1
STDERR = 2

# size of single chunk of bignum on bytes
CHUNK_SIZE = 8
TEST_A_HIGH = 0x0000000200000000
TEST_A_LOW  = 0x8000000000000000
TEST_B_HIGH = 0x0003800000000000
TEST_B_LOW  = 0x8000000000000000

.global add_bignum
.global add_bignum_fast

.section .text
TEST_BIGNUM_A:
	.8byte TEST_A_LOW
	.8byte TEST_A_HIGH
TEST_BIGNUM_B:
	.8byte TEST_B_LOW
	.8byte TEST_B_HIGH

add_test:
	leaq TEST_BIGNUM_A(%rip), %rsi
	leaq TEST_BIGNUM_B(%rip), %rdi
	movq $2, %rcx
	call add_bignum

	movq $SYS_EXIT, %rax
	#movq $0, %rdi # error code
	xor %rdi, %rdi
	syscall
  int3

# Adds two bignums and puts result in first of them
# bignums can differ in size
# the bigger one will contain result
# void add_bignum(struct bignum b1, struct bignum b2)
add_bignum:
  mov %rcx, %r8
  cmp %rcx, %rsi
  cmovs %rsi, %r8

	xor %rbx, %rbx # also clear carry flag
	# r10 now contains pointer to the end of bignum
	add_bignum_addloop:
		movq (%rsi, %rbx, 8), %r8
		adcq %r8, (%rdi, %rbx, 8)

		incq %rbx
		decq %r8
		jnz add_bignum_addloop

  # if sizes of bignums were different
  # adding carry of last operation to bigger
  # argument might be necessary
  cmp %rcx, %rsi
  jz add_bignum_end
  adcq $0, (%rdi, %rbx, 8)
	
  add_bignum_end:
    movq %rdi, %rax
    ret

# Adds two bignums and puts result in first of them
# both big nums must be the same fixed size
# void add_bignum_fast(int64_t *b1, int64_t *b2, int64_t size);
add_bignum_fast:
	xor %rbx, %rbx # also clear carry flag
	# r10 now contains pointer to the end of bignum
	add_bignum_fast_addloop:
		movq (%rsi, %rbx, 8), %r8
		adcq %r8, (%rdi, %rbx, 8)

		incq %rbx
		decq %rdx
		jnz add_bignum_fast_addloop
	
	ret
