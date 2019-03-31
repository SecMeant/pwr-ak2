#include "bcd_common.h"
#include <stdlib.h>
bcd_bignum bcd_bignum_power(bcd_bignum num, int exponent){
	
	uint8_t temp[BCD_POWER_INIT_SIZE];
	memset(temp, 0, BCD_POWER_INIT_SIZE);

	uint8_t* tmp_res = NULL, *tmp_num = NULL;
	temp[0] = 0x1;
	// set init value
	bcd_bignum result;
	result.bignum_size = BCD_POWER_INIT_SIZE;
	result.bignum = temp;

	while( exponent > 0){
		
		if(exponent & 1){
			result = bcd_bignum_mul_fixed(result,num);
			if(tmp_res != NULL)
				free(tmp_res);
			tmp_res = result.bignum;
			
		}
		exponent = exponent>>1;

		num = bcd_bignum_mul_fixed(num,num);
		if(tmp_num != NULL)
			free(tmp_num);
		tmp_num = num.bignum;

	}
	free(num.bignum);
	return result;
}
