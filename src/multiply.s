.extern malloc

# define some variables
BIGNUM_CHUNK = 7 # amount of bignum chunks
CHUNK_SIZE = 8
INT64_MAX = 0x7FFFFFFFFFFFFFFF
FIRST_VALUE_SIGN_MASK = 0x1
SECOND_VALUE_SIGN_MASK = 0x1
# syscall requests
SYS_EXIT = 60

# buffer size
BUFFER_SIZE = 40 * 1
.text
big_value_1: # 7 lines * 64bit argument = 448bit 
  .8byte 0x2
  .8byte 0x2
  .8byte 0x2
  .8byte 0x2
  .8byte 0x2
  .8byte 0x2
  .8byte 0x2
big_value_2: # 7 lines * 64bit argument = 448bit
  .8byte 0x2
  .8byte 0x2
  .8byte 0x2
  .8byte 0x2
  .8byte 0x2
  .8byte 0x2
  .8byte 0x2
	
.global multiply_test
.global bignum_multiply

multiply_test:

	movq $BIGNUM_CHUNK, %r8
	movq big_value_1(%rip), %r9
	movq big_value_2(%rip), %r10
	call bignum_multiply
	pop %rax
	movq %rbp, %rsp
	movq $SYS_EXIT, %rax
	xor %rdi, %rdi
	syscall

# first bignum argument multiply with second bignum argument
# return pointer to allocated memory
# r8 should contain information about size of operands 
# r9 contains pointer to first bignum argument
# r10 constains pointer to second bignum argument
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
	# allocate memory on stack
	subq $32, %rsp
	# save registrs
	movq %r8, 16(%rsp)
	movq %r9, 24(%rsp)
	movq %r10, 32(%rsp)
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
			add %rax,   (%rcx, %r14, CHUNK_SIZE)
			adc %rdx,  8(%rcx, %r14, CHUNK_SIZE)
			# save carry after last addition
			adc $0,   16(%rcx, %r14, CHUNK_SIZE)
			##### JUST FOR TEST
			movq (%rcx, %r14, CHUNK_SIZE), %r15
			######### 
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
		add %rax,    (%rcx, %r14, CHUNK_SIZE)
		adc %rdx,   8(%rcx, %r14, CHUNK_SIZE)
		# save carry after last addition
		adc $0,   16(%rcx, %r14, CHUNK_SIZE)		
		# check if number is negative
		#if( mask & first_bignum_negative || mask & second_bignum_negative )
		testq $FIRST_VALUE_SIGN_MASK, 16(%rsp)
		jns bignum_multiply_positive_number
		
		# fix number if negative 	
		sub %rbx, 8(%rcx, %r14, CHUNK_SIZE)  
		sbb $0,  16(%rcx, %r14, CHUNK_SIZE)

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
		add %rax,   (%rcx, %r14, CHUNK_SIZE)
		adc %rdx,  8(%rcx, %r14, CHUNK_SIZE)
		# save carry after last addition
		adc $0,   16(%rcx, %r14, CHUNK_SIZE)		
		# check if number is negative
		#if( mask & first_bignum_negative || mask & second_bignum_negative )
		testq $SECOND_VALUE_SIGN_MASK, 16(%rsp)
		jns bignum_multiply_positive_number_2
		
		# fix number if negative 	
		sub %rbx, 8(%rcx, %r14, CHUNK_SIZE)  
		sbb $0,  16(%rcx, %r14, CHUNK_SIZE)

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
	adc %rdx,  8(%rcx, %r14, CHUNK_SIZE)


	# only for test
	incq %r8
	movq $0, %r14
	movq %r8, %rax
	movq $2, %rbx 
	mul %rbx
	# get result pointer
	movq 8(%rsp), %rdx
	# init loop counter
	movq %rax, %rcx
	bignum_multiply_test_loop:
		movq (%rdx, %r14, CHUNK_SIZE), %rbx
		incq %r14
		loop bignum_multiply_test_loop

	# restore stack
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

