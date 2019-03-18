CHUNK_SIZE = 8
# bignum++
# rdi contains bignum ptr
# rsi contains bignum size
.text
.global bignum_increment
bignum_increment:
  addq $1, (%rdi)
  # index 
  movq $0, %r8

  bignum_increment_propagate_carry:
    adcq $0, 8(%rdi, %r8, CHUNK_SIZE)
    incq %rcx
    decq %rsi
    jnc bignum_increment_finish
    jg bignum_increment_propagate_carry

  bignum_increment_finish:
  ret
