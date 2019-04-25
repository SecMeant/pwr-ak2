.extern malloc

CHUNK_SIZE = 8

.text
.global bignum_multiply_fixed_no_malloc

# first bignum argument multiply with second bignum argument
# return pointer to allocated memory
# rdi    contains pointer to first bignum argument
# rsi    contains first bignum size
# rdx    contains pointer to second bignum argument
# rcx    contains second bignum size
# r8     contains pointer to result
# r9     contains result size
bignum_multiply_fixed_no_malloc:
  push %rbp
  movq %rsp, %rbp
  # save some registers
  push %rbx
  push %r12
  push %r13
  push %r14
  push %r15

  movq %rdi, %r15
  movq %rdx, %r10
  
  cmpq %rsi, %r9
  cmovsq %r9, %rsi 
  # save allocated memory pointer on stack
  movq %rax, 8(%rsp)
  # int i = 0 
  movq $0, %r12
  # int j = 1
  movq $1, %r13
  # carry = 0
  movq $0, %rdi
  
  # expand first loop to init argument
bignum_multiply_no_malloc_init_result:
  # move first chunk to multiply
  movq (%r15), %rax; 
  mulq (%r10, %r12, CHUNK_SIZE)
  # init result 
  movq %rax, (%r8, %r12,CHUNK_SIZE )
  # add carry from previous multiplication
  addq %rdi, (%r8, %r12,CHUNK_SIZE ) 
  # save new carry
  movq %rdx, %rdi

  jnc bignum_multiply_no_malloc_no_carry
  incq %rdi # carry++
bignum_multiply_no_malloc_no_carry:
  
  incq %r12
  cmpq %r12, %rsi
  jg bignum_multiply_no_malloc_init_result
  # prepand carry
  addq %rdi, (%r8, %r12,CHUNK_SIZE )
  # reset carry
  movq $0, %rdi 
  decq %rsi


# multiply rest of the number
bignum_multiply_no_malloc_outter_loop: 
  # i = 0 
  movq $0, %r12
  # just to place result from current multiplication
  # on right place
  # tmp = j
  movq %r13, %r14
  cmpq $0, %rsi
  jbe bignum_multiply_no_malloc_finish

  bignum_multiply_no_malloc_inner_loop:

    movq (%r15, %r13, CHUNK_SIZE), %rax;  # get chunk form first bignum
    mulq (%r10, %r12, CHUNK_SIZE)        # get chunk from second bignum
    # add result  
    addq %rax, (%r8, %r14,CHUNK_SIZE )
    jnc bignum_multiply_no_malloc_no_carry_2
    incq %rdx # carry++
  bignum_multiply_no_malloc_no_carry_2:
    # add carry
    addq %rdi, (%r8, %r14,CHUNK_SIZE ) 
    movq %rdx, %rdi

    jnc bignum_multiply_no_malloc_no_carry_3
    incq %rdi # carry++
  bignum_multiply_no_malloc_no_carry_3:
    incq %r14
    incq %r12
    cmpq %r12, %rsi
    jg bignum_multiply_no_malloc_inner_loop
  # prepand carry
  addq %rdi, (%r8, %r12,CHUNK_SIZE )
  # reset carry
  movq $0, %rdi

  incq %r13
  decq %rsi
  jmp bignum_multiply_no_malloc_outter_loop

bignum_multiply_no_malloc_finish:
  pop %r15
  pop %r14
  pop %r13
  pop %r12
  pop %rbx
  movq %rbp, %rsp
  pop %rbp
  ret
