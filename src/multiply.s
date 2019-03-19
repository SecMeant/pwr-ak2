.extern malloc

CHUNK_SIZE = 8
INT64_MAX = 0x7FFFFFFFFFFFFFFF
FIRST_VALUE_SIGN_MASK = 0x1
SECOND_VALUE_SIGN_MASK = 0x2

.text
.global bignum_multiply

# first bignum argument multiply with second bignum argument
# return pointer to allocated memory
# rdi -> R9   contains pointer to first bignum argument
# rsi -> R8   contains information about size of first bignum
# rdx -> R10  contains pointer to second bignum argument
# rcx -> R8   contains information about size of second bignum

#   	A B C
# x 	D E F
# ------------
#		   FC
#        FB
#      FA
#        EC
#      EB
#      ...
# +
# ------------
bignum_multiply:
	push %rbp
	movq %rsp, %rbp
	# save some registers
	push %rbx
	push %r12
	push %r13
	push %r14
	push %r15

	# get bigger value of operands
	cmpq %rsi, %rcx
	movq %rcx,%r8
	jge bignum_multiply_bigest_found
	movq %rsi,%r8

bignum_multiply_bigest_found:
	# allocate memory on stack
	subq $40, %rsp
	# save registrs
	movq %r8, 16(%rsp)
	movq %rdi, 24(%rsp)
	movq %rdx, 32(%rsp)
	# calculate proper size for multiplication result 
	movq %r8, %rax
	# $16 = $2*$8 , result will be twice as big as argument, also convert value to word size   
	movq $16, %rbx
	mulq %rbx
	# allocate memory for result 
	movq %rax, %rdi
	call malloc
	# restore registrs
	movq 16(%rsp), %r8
	movq 24(%rsp), %r9
	movq 32(%rsp), %r10
	# save allocated memory pointer on stack
	# !!!!!!!!!!!!!!!!!!!!!!!!!!!  MEMORY POINTER FOR RESULT !!!!!!!!!!!!!!!!!!!!!!!!
	movq %rax, 8(%rsp)
	# set all values in allocated memory to 0 
	movq %r8, %rbx
	call zero_memory
	# flag information for numbers
	# if LSB is set, this means that first number is negative
	# if (LSB+1) is set, this means that second number is negative
	movq $0, 16(%rsp)
	# check numbers sign
	movq $INT64_MAX , %rax # bigest posible positve number in U2 
  	movq %r8, %rdi # copy bignum chunk_size
  	decq %rdi # decreament to obtain offset
  	movq (%r9,%rdi,CHUNK_SIZE), %rbx
  	cmpq %rax, %rbx
  	jb bignum_multiply_check_next_number
  	# set flag for first number
  	orq $FIRST_VALUE_SIGN_MASK, 16(%rsp)

bignum_multiply_check_next_number: 

  	movq (%r10, %rdi, CHUNK_SIZE), %rbx
  	cmpq %rax, %rbx
  	jb bignum_multiply_continue_multiplication
  	# set flag for second number
  	orq $SECOND_VALUE_SIGN_MASK, 16(%rsp)	

bignum_multiply_continue_multiplication:
	
	# u64 i = 0
 	movq $0, %r12 
 	# u64 j = 0
	movq $0, %r13
	# save actual bignum size
  	movq %r8, %r15 
  	# we split bignum to separated loops
  	# in case our bignums are negative we need to 
  	# perform diffrent opearations for each last chunk of data
  	# since we decrement chunk size 
  	decq %r8
  	# get result pointer
	movq 8(%rsp), %rcx

	bignum_multiply_loop_outer:
		# int64 second_big_val = *(bignum_second + off)
		movq (%r10, %r12, CHUNK_SIZE), %rbx 
		# u64 result_index = i
		movq %r12, %r14
		
		bignum_multiply_loop_inner:
		    
			# int64 first_big_val = *(bignum_first + off)
			movq (%r9, %r13, CHUNK_SIZE), %rax
			mul %rbx
			
			# save result in memory
			addq %rax,   (%rcx, %r14, CHUNK_SIZE)
			adcq %rdx,  8(%rcx, %r14, CHUNK_SIZE)
			# if no carry flag set, omit carry propagation
			jnc bignum_multiply_without_carry
			# save flags
			pushf
			# prepare registers for carry operation
			# set start position
			movq %r14, %rdi
			# set loop counter
			movq %r15, %rsi
			# decrease loop counter by propagation offset 
			subq $2, %rsi
			# restore flags
			popf
			# propagte carry after last addition
			# while( i < BIGNUM_SIZE) 
			bignum_multiply_propagate_carry:
				adcq $0,  16(%rcx, %rdi, CHUNK_SIZE)
				incq %rdi
				decq %rsi
				jnc bignum_multiply_without_carry
				jg bignum_multiply_propagate_carry

		bignum_multiply_without_carry:

			# j++ 
			incq %r13
			# calculate result index
			incq %r14 
			# while( j < bignum_size-1 )
			cmpq %r13, %r8
			ja bignum_multiply_loop_inner
		# last data chunk multiplication requires special treatment
		# since it can be negative
		# more on Dr.Tomczak sildes mnozenie.pdf str 18

		movq (%r9, %r13, CHUNK_SIZE), %rax
		mul %rbx
		# save result in memory
		addq %rax,    (%rcx, %r14, CHUNK_SIZE)
		adcq %rdx,   8(%rcx, %r14, CHUNK_SIZE)

		# if no carry flag set, omit carry propagation
		jnc bignum_multiply_without_carry_2
		# save flags
		pushf
		# prepare registers for carry operation
		# set start position
		movq %r14, %rdi
		# set loop counter
		movq %r15, %rsi
		# decrease loop counter by propagation offset 
		subq $2, %rsi
		# restore flags
		popf
		# propagate carry after last addition
		# while( i < BIGNUM_SIZE) 
		bignum_multiply_propagate_carry_2:
			adcq $0,   16(%rcx, %rdi, CHUNK_SIZE)
			incq %rdi
			decq %rsi
			jnc bignum_multiply_without_carry_2
			jg bignum_multiply_propagate_carry_2
		
	bignum_multiply_without_carry_2:
		
	

		# check if number is negative
		#if( mask & first_bignum_negative || mask & second_bignum_negative )
		testq $0x1, 16(%rsp)

		jz bignum_multiply_positive_number
		
		# fix number if negative 	
		sub %rbx, 8(%rcx, %r14, CHUNK_SIZE)
		
		# if no carry flag set, omit borrow propagation
		jnc bignum_multiply_positive_number
		# save flags
		pushf
		# prepare registers for carry operation
		# set start position
		movq %r14, %rdi
		# set loop counter
		movq %r15, %rsi
		# decrease loop counter by propagation offset 
		subq $2, %rsi
		# restore flags
		popf

		# while( i < BIGNUM_SIZE) 
		bignum_multiply_propagate_borrow:
			sbbq $0,  16(%rcx, %rdi, CHUNK_SIZE)
			incq %rdi
			decq %rsi
			jnc bignum_multiply_positive_number
			jg bignum_multiply_propagate_borrow   
		
		bignum_multiply_positive_number:  
		#j =0
		movq $0, %r13
		# i++
		incq %r12 
		# while( i < bignum_size-1 )
		cmpq %r12, %r8
		ja bignum_multiply_loop_outer
	
	# in this moment
	# i = bignum_size - 1
	# j = 0
	# get prelast data chunk from second bignum 
	movq (%r10, %r12, CHUNK_SIZE), %rbx
	# set result index
	movq %r12, %r14
	
	# multiply it with first bignum
	bignum_multiply_loop_last_arg:
		

		movq (%r9, %r13, CHUNK_SIZE), %rax
		mul %rbx
		# save result in memory
		addq %rax,   (%rcx, %r14, CHUNK_SIZE)
		adcq %rdx,  8(%rcx, %r14, CHUNK_SIZE)
		# if no carry flag set, omit carry propagation
		jnc bignum_multiply_without_carry_3
		# save flags
		pushf
		# prepare registers for carry operation
		# set start position
		movq %r14, %rdi
		# set loop counter
		movq %r15, %rsi
		# decrease loop counter by propagation offset 
		subq $2, %rsi
		# restore flags
		popf
		# propagate carry after last addition
		# while( i < BIGNUM_SIZE) 
		bignum_multiply_propagate_carry_3:
			adcq $0,   16(%rcx, %rdi, CHUNK_SIZE)
			incq %rdi
			decq %rsi
			jnc bignum_multiply_without_carry_3
			jg bignum_multiply_propagate_carry_3
		
	bignum_multiply_without_carry_3:

		# check if number is negative
		#if( mask & first_bignum_negative || mask & second_bignum_negative )
		testq $SECOND_VALUE_SIGN_MASK, 16(%rsp)
		jz bignum_multiply_positive_number_2
		
		# fix number if negative 	
		sub %rbx, 8(%rcx, %r14, CHUNK_SIZE)

		# if no carry flag set, omit borrow propagation
		jnc bignum_multiply_positive_number_2
		# save flags
		pushf
		# prepare registers for carry operation
		# set start position
		movq %r14, %rdi
		# set loop counter
		movq %r15, %rsi
		# decrease loop counter by propagation offset 
		subq $2, %rsi
		# restore flags
		popf

		# while( i < BIGNUM_SIZE) 
		bignum_multiply_propagate_borrow_2:
			sbbq $0,  16(%rcx, %rdi, CHUNK_SIZE)
			incq %rdi
			decq %rsi
			jnc bignum_multiply_positive_number_2
			jg bignum_multiply_propagate_borrow_2  
		
	bignum_multiply_positive_number_2:  
		# calculate result index
		incq %r14
		# j++
		incq %r13
		# while( i < bignum_size-1 )
		cmpq %r13, %r8
		ja bignum_multiply_loop_last_arg

	
	# get last data chunk from first bignum 
	movq (%r9, %r8, CHUNK_SIZE), %rbx
	# get last data chunk from second bignum 
	movq (%r10, %r8, CHUNK_SIZE), %rax

	imul %rbx
	
	# save result in memory
	add %rax,   (%rcx, %r14, CHUNK_SIZE)
	adcq %rdx,  8(%rcx, %r14, CHUNK_SIZE)

	
	# prepare result for return 
	movq 8(%rsp), %rax
	movq %r8, %rdx
	# restore stack
	addq $40, %rsp
	pop %r15
	pop %r14
	pop %r13
	pop %r12
	pop %rbx
	movq %rbp, %rsp
	pop %rbp
	ret

# set given memory to 0 value
# rax contains begin address
# rbx memory size
zero_memory:
	# int64 i =0
	subq $8, %rsp
	movq $0, (%rsp)
	L0:
		# *(mem + i) = 0 
		movq (%rsp), %rcx
		movq $0, (%rax, %rcx, CHUNK_SIZE)
		# i++
		incq (%rsp)
		# size--
		decq %rbx
		jnz L0	

	addq $8, %rsp	
	ret

