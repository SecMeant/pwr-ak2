CHUNK_SIZE = 8

.text
/*
	void bignum_shift_left(struct bignum a, int64_t sw);
	rdi = int64_t *bignum
	rsi = int64_t bignum_size
	rdx = int64_t sw
*/
.global bignum_shift_left
bignum_shift_left:
	# bits shifted out of previous chunk
	xor %r9, %r9
	# temp register for shifting out bits
	xor %r8, %r8 
	# shift amount must be in cl register
	mov %rdx, %rcx
	# bignum index
	xor %rdx, %rdx

	bignum_shift_left_shift_loop:
		# fetch bignum chunk
		mov (%rdi, %rdx, CHUNK_SIZE), %r10
	
		# shift out bits to temp register
		shld %cl, %r10, %r8

		# shift current chunk
		shl %cl, %r10

		# "shift in" bits from previous chunk
		or %r9, %r10

		# save result
		mov %r10, (%rdi, %rdx, CHUNK_SIZE)

		# move current shifted out bits to previous chnk bits
		# next iteartion of loop will use it to fill next chunks
		# bits
		mov %r8, %r9

		# TODO i feel like this index counting can be done with
		#      just one register. To be optimized.
		# index++
		incq %rdx
		# while(bignum_size > 0)
		decq %rsi
		jnz bignum_shift_left_shift_loop

	ret 

/*
	void bignum_shift_left(struct bignum a, int64_t sw);
	rdi = int64_t *bignum
	rsi = int64_t bignum_size
	rdx = int64_t sw
*/
.global bignum_shift_right
bignum_shift_right:
	# bits shifted out of previous chunk
	xor %r9, %r9
	# temp register for shifting out bits
	xor %r8, %r8 
	# shift amount must be in cl register
	mov %rdx, %rcx
	# bignum index, start from most significant chunk
	decq %rsi

	bignum_shift_right_shift_loop:
		# fetch bignum chunk
		mov (%rdi, %rsi, CHUNK_SIZE), %r10
	
		# shift out bits to temp register
		shrd %cl, %r10, %r8

		# shift current chunk
		shr %cl, %r10

		# "shift in" bits from previous chunk
		or %r9, %r10

		# save result
		mov %r10, (%rdi, %rsi, CHUNK_SIZE)

		# move current shifted out bits to previous chnk bits
		# next iteartion of loop will use it to fill next chunks
		# bits
		mov %r8, %r9

		# index--
		# while(index >= 0)
		decq %rsi
		jns bignum_shift_right_shift_loop

	ret 
