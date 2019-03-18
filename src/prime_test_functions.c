#include "common.h"

bool trial_test(bignum num){
	 bignum condition, i;
	 bignum_divide_result res;

	 int64_t condition_array[BIGNUM_COMMON_SIZE] = {0x0} ;
	 int64_t counter_array[BIGNUM_COMMON_SIZE] = {0x0};

	 condition.bignum = condition_array;
	 condition.bignum_size = 1;

	 i.bignum_size = BIGNUM_COMMON_SIZE;
	 i.bignum = counter_array;

	 for( bignum a = bignum_multiply_fixed(i,i); bignum_less_than(a, num); bignum_increment(i) )
	 	
	 	res = bignum_divide(num,a);
	 	if( bignums_are_equal(res.reminder, condition) )
	 		return false;
	 return true;
	 
}

bool bignums_are_equal(bignum lhs, bignum rhs){
	
	bignum_subtract(lhs,rhs);

	for(int64_t i =0 ; i < lhs.bignum_size; i++)
		if(lhs.bignum[i] != 0)
			return false;
	return true;
}

bool bignum_less_than(bignum lhs, bignum rhs){

	bignum_subtract(lhs,rhs);
	
	for(int64_t i =lhs.bignum_size-1 ; i > 0; i++){
		if(lhs.bignum[i] < 0)
			return true;
		else
			return false;
	}
	return false;
}

bool bignum_greater_than(bignum lhs, bignum rhs){
	bignum_subtract(lhs,rhs);
	
	for(int64_t i =lhs.bignum_size-1 ; i > 0; i++){
		if(lhs.bignum[i] > 0)
			return true;
		else
			return false;
	}
	return false;
}