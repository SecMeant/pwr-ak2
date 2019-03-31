#include "bcd_common.h"
#include <stdlib.h>
bool bcd_trial_test(bcd_bignum num){
	bcd_bignum i;
	bcd_bignum_divide_result res;

	uint8_t counter_array[BCD_COMMON_SIZE];
	memset(counter_array, 0 , BCD_COMMON_SIZE);
	counter_array[0]= 0x2;
	i.bignum_size = BCD_COMMON_SIZE;
	i.bignum = counter_array;
	bcd_bignum i_2;

	 while( true ){

	 	i_2 = bcd_bignum_mul_fixed(i,i);
	 	
	 	res = bcd_bignum_divide(num,i);

	 	if( bcd_bignum_is_zero( res.reminder ) ){
		 	bcd_bignum_free( res.result );
			bcd_bignum_free( res.reminder );
	 		free( i_2.bignum );
	 		return false;
	 	}
	 	
		bcd_bignum_free( res.result );
		bcd_bignum_free( res.reminder );
	 	// bcd_bignum_print(i_2);
	 	
	 	if( bcd_bignum_greater_than(i_2, num) ){
	 		free( i_2.bignum );
	 		return true;
	 	}
		free( i_2.bignum );
	 	bcd_increment(i);
	 }
}

