CHUNK_SIZE = 8

.text
/*
  void bignum_shift_left_inp(struct bignum a, int64_t sw);
  rdi = int64_t *bignum
  rsi = int64_t bignum_size
  rdx = int64_t sw
  CAUTION: This function should only be used internally!
           It is able only to shift if shift range is
           smaller than size of chunks in bits.
           For bigger shifts, chunk shifting for part
           of the job should be used!
*/
.global bignum_shift_left_inp_64_inp
bignum_shift_left_inp_64_inp:
  # bits shifted out of previous chunk
  xor %r9, %r9
  # temp register for shifting out bits
  xor %r8, %r8
  # shift amount must be in cl register
  mov %rdx, %rcx
  # bignum index
  xor %rdx, %rdx

  bignum_shift_left_inp_shift_loop:
    # fetch bignum chunk
    mov (%rdi, %rdx, CHUNK_SIZE), %r10

    # shift out bits to temp register
    shld %cl, %r10, %r8

    # shift current chunk
    shl %cl, %r10

    # "shift in" bits from previous chunk
    or %r9, %r10

    # save result
    mov %r10, (%rdi, %rdx, CHUNK_SIZE)

    # move current shifted out bits to previous chnk bits
    # next iteartion of loop will use it to fill next chunks
    # bits
    mov %r8, %r9
    xor %r8, %r8

    # TODO i feel like this index counting can be done with
    #      just one register. To be optimized.
    # index++
    incq %rdx
    # while(bignum_size > 0)
    decq %rsi
    jnz bignum_shift_left_inp_shift_loop

  ret

/*
  void bignum_shift_left_inp(struct bignum a, int64_t sw);
  rdi = int64_t *bignum
  rsi = int64_t bignum_size
  rdx = int64_t sw
  CAUTION: This function should only be used internally!
           It is able only to shift if shift range is
           smaller than size of chunks in bits.
           For bigger shifts, chunk shifting for part
           of the job should be used!
*/
.global bignum_shift_right_inp_64_inp
bignum_shift_right_inp_64_inp:
  # shift amount must be in cl register
  mov %rdx, %rcx

  # shift first chunk, its special case
  # because first chunk will not shift out
  # any bits
  mov (%rdi), %r8
  shr %cl, %r8
  mov %r8, (%rdi)

  # index
  mov $1, %r9

  # TODO this probably can be optimized
  bignum_shift_right_inp_shiftloop:
  # while(bignum_size >= index)
  cmp %r9, %rsi
  jle bignum_shift_right_inp_return

  # load current chunk
  mov (%rdi, %r9, CHUNK_SIZE), %r10

  # buffer for storing shifted out bits
  # need to be cleared each time
  xor %r11, %r11

  # shift out bits
  shrd %cl, %r10, %r11

  # proper shift right of current chunk
  shr %cl, %r10

  # save shifted result
  mov %r10, (%rdi, %r9, CHUNK_SIZE)

  # or shifted out bits into previous chunk
  or %r11, -CHUNK_SIZE(%rdi, %r9, CHUNK_SIZE)

  incq %r9
  jmp bignum_shift_right_inp_shiftloop

  bignum_shift_right_inp_return:
    ret

