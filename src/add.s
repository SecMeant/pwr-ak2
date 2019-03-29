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

  # if last loop ended without carry, return
  jnc add_bignum_end
  # if sizes are the same or first is smaller, return
  cmp %rcx, %rsi
  jle add_bignum_end

  # here carry must have existed on last add and b1.size > b2.size
  movq %r9, %r8
  # now r9 has index of first chunk from which
  # carry propagation should continue
  # r8 has loop count
  stc # restore carry, in this branch it must have happened
  add_bignum_propagate_add:
    adcq $0, (%rdi, %r9, CHUNK_SIZE)
    incq %r9
    decq %r8
    jnz add_bignum_propagate_add
  
  add_bignum_end:
    ret



