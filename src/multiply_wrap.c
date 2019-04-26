#include "bignum_common.h"
#include "string.h"

extern bignum bignum_multiply_precise(bignum first, bignum second,bignum res);

bignum bignum_multiply(bignum first, bignum second){
	int64_t first_max_bits = bignum_effective_width(first);
	int64_t second_max_bits = bignum_effective_width(second);
	// we need to round up the result
	int64_t chunks =  (first_max_bits + second_max_bits + 1) / CHUNK_SIZE_BITS + 1;

	bignum res = bignum_make(chunks);
	memset(res.bignum, 0, chunks*sizeof(int64_t));
	bignum_print(res);
	// just to siplify algorith, assume that first bignum is allway bigger than second
	if( first.bignum_size > second.bignum_size )
		bignum_multiply_precise(first, second, res);
	else
		bignum_multiply_precise(second, first, res);


	return res;
}
