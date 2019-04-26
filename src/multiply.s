.extern malloc

CHUNK_SIZE = 8

.text
.global bignum_multiply_fixed

# first bignum argument multiply with second bignum argument
# return pointer to allocated memory
# rdi -> R8   contains pointer to first bignum argument
# rsi -> R9   contains information about size of first bignum
# rdx -> R10  contains pointer to second bignum argument
# rcx -> R8   contains information about size of second bignum

#     A B C
# x   D E F
# ------------
#       FC
#        FB
#      FA
#        EC
#      EB
#      ...
# +
# ------------
bignum_multiply_fixed:
  push %rbp
  movq %rsp, %rbp
  # save some registers
  push %rbx
  push %r12
  push %r13
  push %r14
  push %r15

  # allocate memory on stack
  subq $48, %rsp
  # save registrs
  movq %rdi, 16(%rsp)
  movq %rsi, 24(%rsp)
  movq %rdx, 32(%rsp)
  movq %rcx, 40(%rsp)

  # calculate proper size for multiplication result
  movq %rsi, %rax
  # allocate new space for result
  movq $8, %rbx
  mulq %rbx
  # allocate memory for result
  movq %rax, %rdi
  call malloc
  # restore registrs
  movq 16(%rsp), %r8   # restore rdi
  movq 24(%rsp), %rsi   # restore rsi
  movq 32(%rsp), %r10  # restore rdx
  movq 40(%rsp), %rcx  # restore rcx
  # save allocated memory pointer on stack
  movq %rax, 8(%rsp)
  # result = malloc(sizof(uint64_t) * b1_bignum_size)
  movq %rax, %r15
  # int i = 0
  movq $0, %r12
  # int j = 1
  movq $1, %r13
  # carry = 0
  movq $0, %rdi

  # expand first loop to init argument
bignum_multiply_init_result:
  # move first chunk to multiply
  movq (%r8), %rax
  mulq (%r10, %r12, CHUNK_SIZE)
  # init result
  movq %rax, (%r15, %r12,CHUNK_SIZE )

  # add carry from previous multiplication
  addq %rdi, (%r15, %r12,CHUNK_SIZE )
  # save new carry
  movq %rdx, %rdi

  jnc bignum_multiply_no_carry
  incq %rdi # carry++
bignum_multiply_no_carry:

  incq %r12
  cmpq %r12, %rsi
  jg bignum_multiply_init_result

  # prepand carry
  addq %rdi, (%r15, %r12,CHUNK_SIZE )
  # reset carry
  movq $0, %rdi
  decq %rsi


# multiply rest of the number
bignum_multiply_outter_loop:
  # i = 0
  movq $0, %r12
  # just to place result from current multiplication
  # on right place
  # tmp = j
  movq %r13, %r14
  cmpq $0,%rsi
  jbe bignum_multiply_finish

  bignum_multiply_inner_loop:

    movq (%r8, %r13, CHUNK_SIZE), %rax;  # get chunk form first bignum
    mulq (%r10, %r12, CHUNK_SIZE)        # get chunk from second bignum
    # add result
    addq %rax, (%r15, %r14,CHUNK_SIZE )
    jnc bignum_multiply_no_carry_3
    incq %rdx # carry++
  bignum_multiply_no_carry_3:
    # add carry
    addq %rdi, (%r15, %r14,CHUNK_SIZE )
    movq %rdx, %rdi

    jnc bignum_multiply_no_carry_4
    incq %rdi # carry++
  bignum_multiply_no_carry_4:
    incq %r12
    incq %r14
    cmpq %r12, %rsi
    jg bignum_multiply_inner_loop
  # prepand carry
  addq %rdi, (%r15, %r12,CHUNK_SIZE )
  # reset carry
  movq $0, %rdi

  incq %r13
  decq %rsi
  jmp bignum_multiply_outter_loop

bignum_multiply_finish:
   # prepare result for return
  movq 8(%rsp), %rax
  movq 24(%rsp), %rdx
  # restore stack and return
  addq $48, %rsp
  pop %r15
  pop %r14
  pop %r13
  pop %r12
  pop %rbx
  movq %rbp, %rsp
  pop %rbp
  ret
