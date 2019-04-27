CHUNK_SIZE = 8
# bignum++
# rdi contains bignum ptr
# rsi contains bignum size
.text
.global bignum_increment_inp
bignum_increment_inp:
  addq $1, (%rdi)

  movq $1, %r8

  bignum_increment_inp_propagate_carry:
    cmpq %r8, %rsi
    jbe bignum_increment_inp_finish
    adcq $0, (%rdi, %r8, CHUNK_SIZE)
    incq %r8
    jnc bignum_increment_inp_finish

  bignum_increment_inp_finish:
  ret
