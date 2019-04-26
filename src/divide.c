#include "bignum_common.h"
#include <string.h>

int64_t max(int64_t a, int64_t b)
{
  if(a > b) return a;
  return b;
}

bignum_divide_result bignum_divide(bignum b1, bignum b2)
{
  if(bignum_is_zero(b2))
    bignum_fatal_error("Attempted division by 0.",
                       ERR_ZERO_DIV);

  int64_t shift = bignum_effective_width(b1) - bignum_effective_width(b2);
  const int64_t arg_width =
    bignum_bit_size_to_chunks(shift+1) + b2.bignum_size;
  int64_t divisor_buffer[BIGNUM_MAX_STACK_ALLOC_SIZE];
  bignum result = bignum_make(bignum_bit_size_to_chunks(shift+1));
  bignum divident = bignum_extend(b1, 0);

  bignum divisor;
  divisor.bignum_size = b2.bignum_size + arg_width;
  if(divisor.bignum_size > BIGNUM_MAX_STACK_ALLOC_SIZE)
    divisor = bignum_make(divisor.bignum_size);
  else
    divisor.bignum = divisor_buffer;

  bignum_copy(divisor, b2);

  bignum_shift_left(divisor, shift);
  while(shift >= 0)
  {
    if(bignum_is_negative(divident))
      bignum_add(divident, divisor);
    else
      bignum_subtract(divident, divisor);

    bignum_shift_left(result, 1);
    if(!bignum_is_negative(divident))
      bignum_or_1(result);

    if(bignum_is_zero(divident))
    {
      bignum_shift_left(result, shift);
      break;
    }

    bignum_shift_right(divisor, 1);
    --shift;
  }

  if(bignum_is_negative(divident))
    bignum_add(divident, b2);

  bignum_divide_result ret;
  ret.result = result;
  ret.reminder = divident;
  return ret;
}
