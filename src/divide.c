#include "common.h"

int64_t max(int64_t a, int64_t b)
{
	if(a > b) return a;
	return b;
}

void divide(bignum b1, bignum b2)
{
	int64_t n = max(b1.bignum_size, b2.bignum_size);
	bignum result;
	result.bignum = malloc(n * CHUNK_SIZE * 2);
	assert(result.bignum);
	result.bignum_size = n;
	bignum_shift_left(b2, n);
	
	while(n>=0)
	{
		//if(bignum_is_negative(result))
					
	}
}
