#include "common.h"
#include <stdio.h>
bignum bignum_power(bignum num, int exponent){
	
	int64_t temp[BIGNUM_COMMON_SIZE] = {0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
	// set init value
	bignum result;
	result.bignum_size = BIGNUM_COMMON_SIZE;
	result.bignum = temp;

	while( exponent > 0){
		
		if(exponent & 1)
			result = bignum_multiply_fixed(result,num);
		
		exponent = exponent>>1;
		num = bignum_multiply_fixed(num,num);
	}
	return result;
}

// bignum bignum_power_mod(bignum number, int exponent, int mod){

// }
