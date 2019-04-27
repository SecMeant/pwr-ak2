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
  movq %rcx,%rax # smaller arg
  jbe bignum_subtract_smallest_found
  movq %rsi,%rax # smaller arg

bignum_subtract_smallest_found:
  # clear carry flag
  clc
  # set index counter to 0
  movq $0, %r10
  # make divison with borrow
  bignum_subtract_L1:
    # arg2 = *(big_value_2 + off)
    movq (%rdx, %r10, WORD_SIZE), %r9
    # arg1 = arg2 - arg1
    sbbq %r9, (%rdi,%r10,WORD_SIZE)
    # increment index
    incq %r10
    decq %rsi
    # decrement
    decq %rax
    jnz bignum_subtract_L1

  bignum_subtract_propagate:
    decq %rsi 
    js bignum_subtract_finish
    sbbq $0, (%rdi,%r10,WORD_SIZE)
    incq %r10
  jc bignum_subtract_propagate 

bignum_subtract_finish:
  ret

