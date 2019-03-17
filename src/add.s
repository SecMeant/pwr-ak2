CHUNK_SIZE = 8

.global bignum_add 
.global add_bignum_fast

.section .text
# void add_bignum(struct bignum b1, struct bignum b2)
bignum_add:
  mov %rsi, %r8
  cmp %rsi, %rcx
  cmovs %rcx, %r8

	# index
	xor %r9, %r9 # also clear carry flag
	add_bignum_addloop:
		movq (%rdx, %r9, CHUNK_SIZE), %r10
		adcq %r10, (%rdi, %r9, CHUNK_SIZE)

		incq %r9
		decq %r8
		jnz add_bignum_addloop

  # if sizes of bignums were different
  # adding carry of last operation to bigger
  # argument might be necessary
  cmp %rsi, %rcx
  jz add_bignum_end
  adcq $0, (%rdi, %r9, CHUNK_SIZE)
	
  add_bignum_end:
    ret



