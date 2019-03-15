#include "common.h"

int64_t max(int64_t a, int64_t b)
{
	if(a > b) return a;
	return b;
}

void bignum_divide(bignum b1, bignum b2)
{
	if(bignum_is_zero(b2))
		bignum_fatal_error("Attempted division by 0.", ERR_ZERO_DIV);

	bignum result = bignum_make(b1.bignum_size);
	// From now on divident is also current reminder
	bignum divident = bignum_extend_twice(b1);
	bignum divisor  = bignum_extend_twice(b2);

	int64_t shift = b1.bignum_size - 1;
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
			break;

		bignum_shift_right(divident, 1);
		--shift;
	}
	
	if(bignum_is_negative(divident))
		bignum_add(divident, b2);

	bignum_print(result);
	bignum_print(divident);

	bignum_free(divident);
	bignum_free(divisor);
}
