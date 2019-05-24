WORD_SIZE = 8
.text

# from first bignum argument subtract second bignum argument
# rdi    contains pointer to first bignum argument
# rsi    contains information about size of first bignum
# rdx    contains pointer to second bignum argument
# rcx    contains information about size of second bignum
.global bignum_sub_inp
.type bignum_sub_inp @function
bignum_sub_inp:
  # find smaller number
  cmpq %rsi, %rcx
  movq %rcx,%rax # smaller arg
  jbe bignum_sub_inp_smallest_found
  movq %rsi,%rax # smaller arg

bignum_sub_inp_smallest_found:
  # clear carry flag
  clc
  # set index counter to 0
  movq $0, %r10
  # make divison with borrow
  bignum_sub_inp_L1:
    # arg2 = *(big_value_2 + off)
    movq (%rdx, %r10, WORD_SIZE), %r9
    # arg1 = arg2 - arg1
    sbbq %r9, (%rdi,%r10,WORD_SIZE)
    # increment index
    incq %r10
    decq %rsi
    # decrement
    decq %rax
    jnz bignum_sub_inp_L1

  bignum_sub_inp_propagate:
    decq %rsi 
    js bignum_sub_inp_finish
    sbbq $0, (%rdi,%r10,WORD_SIZE)
    incq %r10
  jc bignum_sub_inp_propagate

bignum_sub_inp_finish:
  ret


# from first bignum argument subtract scaled second bignum argument
# rdi    contains pointer to first bignum argument
# rsi    contains information about size of first bignum
# rdx    contains pointer to second bignum argument
# rcx    contains information about size of second bignum
# r8     contains scale factor in chunks (uint64_t)
#
.global bignum_sub_scaled_inp
.type bignum_sub_scaled_inp @function
bignum_sub_scaled_inp:
  # fix size of scaled second argument
  addq %r8, %rcx

  # find smaller number
  movq %rcx,%rax # rcx smaller?
  cmpq %rsi, %rcx # check
  cmovg %rsi,%rax # if not smaller = rsi

 # from now on use r8 as index to first bignum argument
 # earlier chunks would subtract 0 from itself, cuz second
 # argument is scaled.
bignum_sub_scaled_inp_smallest_found:
  # clear carry flag
  clc
  # Index for chunks from bignum from rdx (b2)
  # From scaled argument start form 0
  movq $0, %r10
  # make scaled subtraction with borrow
  bignum_sub_scaled_inp_L1:
    # arg2 = *(big_value_2 + off)
    movq (%rdx, %r10, WORD_SIZE), %r9
    # arg1 = arg2 - arg1
    sbbq %r9, (%rdi,%r8,WORD_SIZE)
    # TODO(all) too many index counters -- optimize?
    # increment index
    incq %r10
    incq %r8
    decq %rsi
    # decrement
    decq %rax
    jnz bignum_sub_scaled_inp_L1

  bignum_sub_scaled_inp_propagate:
    decq %rsi 
    js bignum_sub_scaled_finish
    sbbq $0, (%rdi,%r10,WORD_SIZE)
    incq %r10
  jc bignum_sub_scaled_inp_propagate 

bignum_sub_scaled_finish:
  ret
