#include "common.h"
#include <stdio.h>
bignum bignum_power(bignum num, int exponent){
	
	int64_t temp[8] = {0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
	// set init value
	bignum result;
	result.bignum_size = 8;
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


/* Iterative Function to calculate (x^y) in O(log y) */
int power(int x, unsigned int y) 
{ 
    int res = 1;     // Initialize result 
   
    while (y > 0) 
    { 
        // If y is odd, multiply x with result 
        if (y & 1) 
            res = res*x; 
   
        // n must be even now 
        y = y>>1; // y = y/2 
        x = x*x;  // Change x to x^2 
    } 
    return res; 
}
