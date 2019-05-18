CHUNK_SIZE = 8

.text
.global bignum_is_negative
.global bignum_copy
# bool bignum_is_negative(bignum b1);
bignum_is_negative:
  decq %rsi
  movq (%rdi, %rsi, CHUNK_SIZE), %rdx
  xor %rax,%rax
  test %rdx, %rdx
  sets %al
  ret

# safetly copies second operand to first operand.
# If second bignum does not have enough space for value
# value is truncated.
bignum_copy:
  movq %rsi, %r8
  cmp %rsi, %rcx
  cmovs %rcx, %r8
  decq %r8

  bignum_copy_copyloop:
    movq (%rdx, %r8, CHUNK_SIZE), %r9
    movq %r9, (%rdi, %r8, CHUNK_SIZE)

    decq %r8
    jns bignum_copy_copyloop

  cmp %rcx, %rsi
  jl bignum_copy_end

  bignum_copy_zeroextend_loop:
    # if two indexes meet, exit
    # TODO check if this work if rsi has smaller value
    # ie first argument is smaller
    cmp %rcx, %rsi
    jz bignum_copy_end

    decq %rsi
    movq $0,(%rdi, %rsi, CHUNK_SIZE)
    jmp bignum_copy_zeroextend_loop

  bignum_copy_end:
    ret
