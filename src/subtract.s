# define some variables
BIGNUM_CHUNK = 7 # amount of bignum chunks
WORD_SIZE = 8
# syscall requests
SYS_READ = 0
SYS_WRITE = 1
SYS_EXIT = 60
# standrad I/O streams
STDIN = 0
STDOUT = 1
STDER = 2
# buffer size
BUFFER_SIZE = 40 * 1

.text
.global subtract_test
.global substract

big_value_1: # 7 lines * 64bit argument = 448 
  .8byte 0x1
  .8byte 0x2
  .8byte 0x3
  .8byte 0x4
  .8byte 0x5
  .8byte 0x6
  .8byte 0x7
big_value_2: # 7 lines * 64bit argument = 448
  .8byte 0x0
  .8byte 0x2
  .8byte 0x3
  .8byte 0x4
  .8byte 0x5
  .8byte 0x6
  .8byte 0x8

subtract_test:
	
	movq $BIGNUM_CHUNK, %rsi
  movq $BIGNUM_CHUNK, %rcx
	leaq big_value_1(%rip), %rdi
	leaq big_value_2(%rip), %rdx

	call bignum_subtract
	pop %rax
	movq %rbp, %rsp
	movq $SYS_EXIT, %rax
	xor %rdi, %rdi
	syscall

# from first bignum argument subtract second bignum argument
# rdi    contains pointer to first bignum argument
# rsi    contains information about size of first bignum
# rdx    contains pointer to second bignum argument
# rcx    contains information about size of second bignum
bignum_subtract:
  # find smaller number
  cmpq %rsi, %rcx
  movq %rcx,%rax
  jbe bignum_subtract_smallest_found
  movq %rsi,%rax

bignum_subtract_smallest_found:
	# clear carry flag
	clc
	# set index counter to 0
	movq $0, %r10
	# make divison with borrow
	bignum_subtract_L1:
    # arg1 = *(big_value_1 +off)
    movq (%rdi, %r10, WORD_SIZE), %r8
    # arg2 = *(big_value_2 + off)
    movq (%rdx, %r10, WORD_SIZE), %r9
    # arg2 = arg2 - arg1
    sbbq %r9, %r8
    # *big_value_2 = arg2 
    movq %r8, (%rdi,%r10,WORD_SIZE)
    # increment index
    incq %r10
    # decrement  
    decq %rax
    jnz bignum_subtract_L1

	ret
# from first bignum argument substract second bignum argument
# rax contains sizeof currently handle bignums
# rbx contains pointer to first bignum argument
# rcx constains pointer to second bignum argument
