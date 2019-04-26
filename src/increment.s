CHUNK_SIZE = 8
# bignum++
# rdi contains bignum ptr
# rsi contains bignum size
.text
.global bignum_increment
bignum_increment:
  addq $1, (%rdi)

  movq $1, %r8

  bignum_increment_propagate_carry:
    cmpq %r8, %rsi
    jbe bignum_increment_finish
    adcq $0, (%rdi, %r8, CHUNK_SIZE)
    incq %r8
    jnc bignum_increment_finish

  bignum_increment_finish:
  ret
