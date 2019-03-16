#include "common.h"

bool trial_test(bignum num){
	 bignum condition, i;
	 int tab[] = {0x1};
	 condition.bignum = tab;
	 condition.bignum_size = 1;

	 i.bignum_size = BIGNUM_COMMON_SIZE;
	 i.bignum = {0x0};
	 for( bignum a = bignum_multiply_fixed(i,i); greater_operator(a, num); bignum_add(i) )
	 	if( bignum_equals(bignum_modul(num,a), condition) )
	 		return false;
	 return true;
	 
}

bool bignum_equals(bignum a, bignum b){
	uint64_t min,max;
	if(a.bignum_size < b.bignum_size){
		max = b;
		min = a;
	}
	else {
		max = a;
		min = b
	}

	for(int i =0 ; i < max; i++)
		if(i < min) 
			if(a.bignum[i] == b.bignum[i]) 
			return false;
		else

}	