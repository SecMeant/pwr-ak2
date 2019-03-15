CHUNK_SIZE = 8

.text
.global bignum_is_negative
.global bignum_copy
# bool bignum_is_negative(bignum b1);
bignum_is_negative:
	decq %rsi
	movq (%rdi, %rsi, CHUNK_SIZE), %rdx
	xor %rax,%rax
	test %rdx, %rdx
	sets %al
	ret

# safetly copies second operand to first operand
bignum_copy:
	movq %rsi, %r8
	cmp %rsi, %rcx
	cmovs %rcx, %r8
	decq %r8

	bignum_copy_copyloop:
		movq (%rdx, %r8, CHUNK_SIZE), %r9
		movq %r9, (%rdi, %r8, CHUNK_SIZE)

		decq %r8
		jns bignum_copy_copyloop
	ret
