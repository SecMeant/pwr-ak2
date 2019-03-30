#include "bcd_common.h"

void bcd_bignum_add(bcd_bignum b1, bcd_bignum b2){
	
	int64_t i;
	uint8_t carry = 0, *biggest_value;
	int64_t min_size, max_size; 
	// find smallest value
	if(b1.bignum_size < b2.bignum_size){
		min_size = b1.bignum_size;
		max_size = b2.bignum_size;
		biggest_value = b2.bignum;
	}else{
		min_size = b2.bignum_size;
		max_size = b1.bignum_size;
		biggest_value = b1.bignum;
	}
	// add bignums
	for (i = 0; i < min_size; ++i)
	{
		biggest_value[i] = b1.bignum[i] + b2.bignum[i] + carry;
		
		carry =0;
		if(biggest_value[i] > 9){
			biggest_value[i] = (biggest_value[i] + 6) & BCD_U8_MASK;
			carry = 1;
		}
	}

	// propagate carry
	while(carry && i < max_size ){
		biggest_value[i] = biggest_value[i] + carry;
		carry = 0;
		if(biggest_value[i] > 9){
			biggest_value[i] = (biggest_value[i] + 6) & BCD_U8_MASK;
			carry = 1;
		}
		i++;
	}
}

void bcd_bignum_subb(bcd_bignum b1, bcd_bignum b2){
	
}