WORD_SIZE = 8
.text
.global bignum_subtract

# from first bignum argument subtract second bignum argument
# rdi    contains pointer to first bignum argument
# rsi    contains information about size of first bignum
# rdx    contains pointer to second bignum argument
# rcx    contains information about size of second bignum
bignum_subtract:
  # find smaller number
  cmpq %rsi, %rcx
  movq %rcx,%rax
  jbe bignum_subtract_smallest_found
  movq %rsi,%rax

bignum_subtract_smallest_found:
	# clear carry flag
	clc
	# set index counter to 0
	movq $0, %r10
	# make divison with borrow
	bignum_subtract_L1:
    # arg1 = *(big_value_1 +off)
    movq (%rdi, %r10, WORD_SIZE), %r8
    # arg2 = *(big_value_2 + off)
    movq (%rdx, %r10, WORD_SIZE), %r9
    # arg2 = arg2 - arg1
    sbbq %r9, %r8
    # *big_value_2 = arg2 
    movq %r8, (%rdi,%r10,WORD_SIZE)
    # increment index
    incq %r10
    # decrement  
    decq %rax
    jnz bignum_subtract_L1

	ret
# from first bignum argument substract second bignum argument
# rax contains sizeof currently handle bignums
# rbx contains pointer to first bignum argument
# rcx constains pointer to second bignum argument
