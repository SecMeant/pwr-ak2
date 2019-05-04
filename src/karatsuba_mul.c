#include "bignum_common.h"
#include <stdio.h>

bignum karatsuba_mul_s(bignum b1, bignum b2){

	// this function make bingums equal size
	size_t n = adjustBignums(&b1, &b2);
	// printf("%llu\n", n);

	// base case to finish recursion
	if(n == 0) return bignum_make(1);
	if(n == 1) return bignum_multiply(b1,b2);
	// find subchunks 
	size_t fh = n/2; //first half of the bignums 
	size_t sh = n-fh; // second half of the bignums
	// printf("%llu\n", fh);
	// printf("%llu\n", sh);
	// divide bignum to smaller chunks

	bignum b1_left  = bignum_subchunk(b1, 0,  fh);
	bignum b1_right = bignum_subchunk(b1, fh, sh);
	// bignum_print(b1_left);
	// bignum_print(b1_right);
	// divide bignum to smaller chunks
	bignum b2_left  = bignum_subchunk(b2, 0,  fh);
	bignum b2_right = bignum_subchunk(b2, fh, sh);
	// bignum_print(b2_left);
	// bignum_print(b2_right);
	// puts("\n");

	// recursivly calculate the three products of input
	bignum P1 = karatsuba_mul_s(b1_left, b2_left);
	
	bignum P2 = karatsuba_mul_s(b1_right, b2_right);

	bignum b1_partial_sum = bignum_add(b1_left, b1_right);	
	bignum b2_partial_sum = bignum_add(b2_left, b2_right);	
	// bignum_print(b1_left);
	// bignum_print(b1_right);

	bignum P3 = karatsuba_mul_s(b1_partial_sum, b2_partial_sum);
	
	
	printf("P1\n");
	bignum_print(P1);
	printf("P2\n");
	bignum_print(P2);
	printf("P3\n");
	bignum_print(P3);
	
	bignum P3_shifted =  bignum_shift_left(P1,2*sh);
	
	printf("P3_shifted\n");
	bignum_print(P3_shifted);
	bignum P3_P2_P1 = bignum_subtract(P3, P1);
	printf("P3_P1\n");
	bignum_print(P3_P2_P1);
	bignum_sub_inp(P3_P2_P1, P2);
	printf("P3_P2_P1\n");
	bignum_print(P3_P2_P1);
	bignum_shift_left_inp(P3_P2_P1, sh);
	printf("P3_P2_P1 SHIFT\n");
	bignum_print(P3_P2_P1);
	bignum_add_inp(P3_shifted,P3_P2_P1);
	printf("P3_shifted  + P3_P2_P1	\n");
	bignum_print(P3_shifted);

	bignum_add_inp(P3_shifted, P2);
	printf("%llu\n", n);
	return P3_shifted;
}