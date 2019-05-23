#include "bignum_common.h"
#include "string.h"

extern bignum bignum_multiply_precise(bignum first, bignum second,bignum res);
extern void bignum_add_inaa(bignum, bignum, bignum);

bignum bignum_multiply(bignum first, bignum second){
	int64_t first_max_bits = bignum_effective_width(first);
	int64_t second_max_bits = bignum_effective_width(second);
	// we need to round up the result
	int64_t chunks =  (first_max_bits + second_max_bits + 1) / BIGNUM_CHUNK_SIZE_BITS +1;

	bignum res = bignum_make(chunks);
	// just to siplify algorith, assume that first bignum is allway bigger than second
	if( first.bignum_size > second.bignum_size )
		bignum_multiply_precise(first, second, res);
	else
		bignum_multiply_precise(second, first, res);

	return res;
}

bignum bignum_multiply_fixed(bignum first, bignum second){

	int64_t chunks = first.bignum_size > second.bignum_size ? first.bignum_size : second.bignum_size; 
	
	bignum res = bignum_make(chunks);
	// just to siplify algorith, assume that first bignum is allway bigger than second
	if( first.bignum_size > second.bignum_size )
		bignum_multiply_precise(first, second, res);
	else
		bignum_multiply_precise(second, first, res);

	return res;
}

void bignum_multiply_inaa(bignum first, bignum second, bignum res){

	if( first.bignum_size > second.bignum_size )
		bignum_multiply_precise(first, second, res);
	else
		bignum_multiply_precise(second, first, res);
}


bignum bignum_add(bignum first, bignum second){
	int64_t first_max_bits = bignum_effective_width(first);
	int64_t second_max_bits = bignum_effective_width(second);
	int64_t max_bits = first_max_bits > second_max_bits ? first_max_bits : second_max_bits;  
	// we need to round up the result
	int64_t chunks =  (max_bits + 1) / BIGNUM_CHUNK_SIZE_BITS +1;
	bignum res = bignum_make(chunks);
	// just to siplify algorith, assume that first bignum is allway bigger than second
	bignum_copy(res, first);
	bignum_add_inp(res,second);
	return res;
}

bignum bignum_subtract(bignum first, bignum second){
	int64_t first_max_bits = bignum_effective_width(first);
	int64_t second_max_bits = bignum_effective_width(second);
	int64_t max_bits = first_max_bits > second_max_bits ? first_max_bits : second_max_bits;  
	// we need to round up the result
	int64_t chunks =  (max_bits) / BIGNUM_CHUNK_SIZE_BITS +1;
	bignum res = bignum_make(chunks);
	// just to siplify algorith, assume that first bignum is allway bigger than second
	bignum_copy(res, first);
	bignum_sub_inp(res,second);
	
	return res;
}
