#include "bcd_common.h"
#include <stdlib.h>
bool bcd_trial_test(bcd_bignum num){
	bcd_bignum i;
	bcd_bignum i_2;
  bcd_bignum_divide_result res;

  uint8_t counter_array[BCD_COMMON_SIZE];
  uint8_t i_2_array[BCD_COMMON_SIZE];

  memset(counter_array, 0 , BCD_COMMON_SIZE);
  memset(i_2_array, 0, BCD_COMMON_SIZE);
  
  counter_array[0]= 0x2;

  i.bignum_size = BCD_COMMON_SIZE;
  i.bignum = counter_array;
  i_2.bignum_size = BCD_COMMON_SIZE;
  i_2.bignum = i_2_array;
  

	 while( true ){
	 	bcd_bignum_mul_fixed_no_return(i,i,i_2);
	 	
	 	res = bcd_bignum_divide(num,i);

	 	if( bcd_bignum_is_zero( res.reminder ) ){
		 	bcd_bignum_free( res.result );
			bcd_bignum_free( res.reminder );
	 		return false;
	 	}
	 	
		bcd_bignum_free( res.result );
		bcd_bignum_free( res.reminder );
	 	// bcd_bignum_print(i_2);
	 	
	 	if( bcd_bignum_greater_than(i_2, num) ){
	 		return true;
	 	}
	 	bcd_increment(i);
	 }
}

