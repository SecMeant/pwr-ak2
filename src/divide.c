#include "bignum_common.h"

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

	bignum result = bignum_make(b1.bignum_size);
	// From now on divident is also current reminder
	// TODO make functions for creating divident and divisor
	// from b1 and b2 that extends and properly shifts
	bignum divident = bignum_extend_twice(b1);
	bignum divisor  = bignum_make(b1.bignum_size *2);
	bignum_copy(divisor, b2);

	int64_t shift = (b1.bignum_size * 64) - 1;
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

	// TODO this reallocs might not be a good idea
	bignum_realloc(&divident, divident.bignum_size / 2);
	bignum_free(divisor);

	bignum_divide_result ret;
	ret.result = result;
	ret.reminder = divident;
	return ret;
}
