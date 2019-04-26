.extern malloc

CHUNK_SIZE = 8

.text
.global bignum_multiply_precise

# first bignum argument multiply with second bignum argument
# this function expect that bignum result struct will be large enught to 
# place a result
# first argument have to be larger than second
# rdi    contains pointer to first bignum argument
# rsi    contains first bignum size
# rdx    contains pointer to second bignum argument
# rcx    contains second bignum size
# r8     contains pointer to result
# r9     contains result size
bignum_multiply_precise:
  # save some registers
  push %r12
  push %r13
  push %r14
  push %r15

  movq %rdi, %r15
  movq %rdx, %r10
  
  # int i = 0
  movq $0, %r12
  # int j = 1
  movq $0, %r13
  # carry = 0
  movq $0, %rdi
  

# multiply rest of the number
bignum_multiply_precise_outter_loop: 
  # i = 0 
  movq $0, %r12
  # just to place result from current multiplication
  # on right place
  # tmp = j
  movq %r13, %r14
  cmpq $0, %rsi
  jbe bignum_multiply_precise_finish

  bignum_multiply_precise_inner_loop:

    movq (%r15, %r13, CHUNK_SIZE), %rax;  # get chunk form first bignum
    movq (%r10, %r12, CHUNK_SIZE), %rbx   # get chunk from second bignum
    mulq (%r10, %r12, CHUNK_SIZE)        # get chunk from second bignum
    # add result  
    addq %rax, (%r8, %r14,CHUNK_SIZE )
    jnc bignum_multiply_precise_no_carry_2
    incq %rdx # carry++
  bignum_multiply_precise_no_carry_2:
    # add carry
    addq %rdi, ( %r8, %r14, CHUNK_SIZE ) 
    movq %rdx, %rdi

    jnc bignum_multiply_precise_no_carry_3
    incq %rdi # carry++
  bignum_multiply_precise_no_carry_3:
    incq %r14
    incq %r12
    cmpq %r12, %rcx
    jg bignum_multiply_precise_inner_loop
  # prepand carry
  addq %rdi, (%r8, %r14,CHUNK_SIZE )
  # reset carry
  movq $0, %rdi

  incq %r13
  decq %rsi
  jmp bignum_multiply_precise_outter_loop

bignum_multiply_precise_finish:
  pop %r15
  pop %r14
  pop %r13
  pop %r12
  ret
