.extern malloc

CHUNK_SIZE = 8
INT64_MAX = 0x7FFFFFFFFFFFFFFF
FIRST_VALUE_SIGN_MASK = 0x1
SECOND_VALUE_SIGN_MASK = 0x2

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

  # int i = 0 
  movq $0, %r12
  # int j = 1
  movq $1, %r13
  # carry = 0
  movq $0, %rdi
  
  # expand first loop to init argument
bignum_multiply_init_result:
  # move first chunk to multiply
  movq (%rdi), %rax; 
  mulq (%rdx, %r12, CHUNK_SIZE)
  # init result 
  movq %rax, (%r8, %r12,CHUNK_SIZE )
  # add carry from previous multiplication
  addq %rdi, (%r8, %r12,CHUNK_SIZE ) 
  # save new carry
  movq %rdx, %rdi

  jno bignum_multiply_no_carry
  incq %rdi # carry++
bignum_multiply_no_carry:
  
  incq %r12
  cmpq %r12, %rsi
  jg bignum_multiply_init_result
  # prepand carry
  addq %rdi, (%r8, %r12,CHUNK_SIZE )
  # reset carry
  movq $0, %rdi 
  decq %rsi

# multiply rest of the number
bignum_multiply_outter_loop: 
  # i = j 
  movq %r13, %r12
  bignum_multiply_inner_loop:

    movq (%rdi, %r13, CHUNK_SIZE), %rax;  # get chunk form first bignum
    mulq (%rdx, %r12, CHUNK_SIZE)        # get chunk from second bignum
    # add result  
    addq %rax, (%r8, %r12,CHUNK_SIZE )
    # add carry
    addq %rdi, (%r8, %r12,CHUNK_SIZE ) 
    movq %rdx, %rdi

    jno bignum_multiply_no_carry_2
    incq %rdi # carry++
  bignum_multiply_no_carry_2:
    
    incq %r12
    cmpq %r12, %rsi
    jg bignum_multiply_inner_loop
  # prepand carry
  addq %rdi, (%r8, %r12,CHUNK_SIZE )
  # reset carry
  movq $0, %rdi  

  incq %r13
  decq %rsi
  cmpq %r13, %rcx
  jg bignum_multiply_outter_loop

   # prepare result for return 
  movq 8(%rsp), %rax
  movq 24(%rsp), %rdx
  # restore stack and return    
  addq $48, %rsp
  pop %r8
  pop %r14
  pop %r13
  pop %r12
  pop %rbx
  movq %rbp, %rsp
  pop %rbp
  ret
