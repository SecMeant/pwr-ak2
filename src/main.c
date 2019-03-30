#include <stdio.h>
#include <stdint.h>
#include "common.h"
#include "bcd_common.h"
void shift_test()
{
	int64_t num1[] = {0,0,3,0};
	bignum bignum_sh = {.bignum = num1, .bignum_size = 4};
	bignum_print(bignum_sh);
	bignum_shift_right_64(bignum_sh, 1);
	bignum_print(bignum_sh);
}

void add_test()
{
	int64_t num1[2] = {0x8000000000000000 ,0x0000000200000000};
	int64_t num2[2] = {0x8000000000000000 ,0x0003800000000000};
	bignum b1 = {.bignum = num1, .bignum_size = 2};
	bignum b2 = {.bignum = num2, .bignum_size = 2};
	printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
	printf("0x%016lx 0x%016lx\n\n", *(num2+1), *num2);
	bignum_add(b1, b2);
	printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
	printf("0x%016lx 0x%016lx\n", *(num2+1), *num2);
}

void sub_test()
{
	int64_t num1[2] = {0xfffffffffffffffe ,0x7fffffffffffffff};
	int64_t num2[2] = {0x5454545454545454 ,0x3434343434343433};
	bignum b1 = {.bignum = num1, .bignum_size = 2};
	bignum b2 = {.bignum = num2, .bignum_size = 2};
	printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
	printf("0x%016lx 0x%016lx\n\n", *(num2+1), *num2);
	bignum_subtract(b1, b2);
	printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
	printf("0x%016lx 0x%016lx\n", *(num2+1), *num2);
	printf("Result negative: %li\n", (int64_t)bignum_is_negative(b1));
}

void copy_test()
{
	int64_t num1[2] = {0xfffffffffffffffe ,0x7fffffffffffffff};
	int64_t num2[2] = {0x5454545454545454 ,0x3434343434343433};
	bignum b1 = {.bignum = num1, .bignum_size = 2};
	bignum b2 = {.bignum = num2, .bignum_size = 2};
	printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
	printf("0x%016lx 0x%016lx\n\n", *(num2+1), *num2);
	bignum_copy(b1, b2);
	printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
	printf("0x%016lx 0x%016lx\n\n", *(num2+1), *num2);
	// Zero extend copy test
	b2.bignum_size = 1;
	bignum_copy(b1, b2);
	printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
	printf("0x%016lx 0x%016lx\n\n", *(num2+1), *num2);
}
void multiply_test(){

	bignum first, second;
	first.bignum_size = 4;
	first.bignum = (int64_t*)malloc(sizeof(uint64_t)*first.bignum_size);
	first.bignum[0] = 0xfffffffffffffffe;
	first.bignum[1] = 0xffffffffffffffff;
	first.bignum[2] = 0xffffffffffffffff;
	first.bignum[3] = 0xffffffffffffffff;
	
	second.bignum_size = 4;
	second.bignum = (int64_t*)malloc(sizeof(uint64_t)*second.bignum_size);
	second.bignum[0] = 0xfffffffffffffffd;
	second.bignum[1] = 0xffffffffffffffff;
	second.bignum[2] = 0xffffffffffffffff;
	second.bignum[3] = 0xffffffffffffffff;

	bignum res = bignum_multiply_fixed(first,second);
	for(int i =0; i < 4; i++)
		printf( "%lx\n", (uint64_t)res.bignum[i] );
	free (first.bignum);
	free (second.bignum);
	free (res.bignum);

}

void power_test(){
	bignum first;
	first.bignum_size = BIGNUM_COMMON_SIZE;
	first.bignum = (int64_t*)malloc(sizeof(uint64_t)*first.bignum_size);
	first.bignum[0] = 0x2;
	first.bignum[1] = 0x0;
	first.bignum[2] = 0x0;
	first.bignum[3] = 0x0;
	first.bignum[4] = 0x0;
	first.bignum[5] = 0x0;
	first.bignum[6] = 0x0;
	first.bignum[7] = 0x0;

	bignum res = bignum_power(first,256);
	for(int i =0; i < BIGNUM_COMMON_SIZE; i++)
		printf( "%lx\n", (uint64_t)res.bignum[i] );

	free(first.bignum);
	free(res.bignum);


}
void bignum_increment_test(){
	bignum first;
	first.bignum_size = 3;
	first.bignum = (int64_t*)malloc(sizeof(uint64_t)*first.bignum_size);
	first.bignum[0] = 0xffffffffffffffff;
	first.bignum[1] = 0xffffffffffffffff;
	first.bignum[2] = 0x0;
	for (int i = 0; i < 100; ++i)
	{
		bignum_increment(first);
		bignum_print(first);		
	}
	free(first.bignum);

}

void isPrime(){
	int64_t num1[BIGNUM_COMMON_SIZE] = { 2305843009213693951ll, 0x0};
	
	bignum b1 = {.bignum = num1, .bignum_size = BIGNUM_COMMON_SIZE};

	bool prime = trial_test(b1);
	
	if( prime )
		printf("Number is prime\n");
	else
		printf("Number is not prime\n");

}

bignum_divide_result div_test()
{
	int64_t num1[8] = {0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff, 0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff, 0xffffffffffffffff, 0xff};
	int64_t num2[8] = {0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
 0xffffffffffffffff,0x7fffffffffffffff, 0x0};

	bignum b1 = {.bignum = num1, .bignum_size = 8};
	bignum b2 = {.bignum = num2, .bignum_size = 8};
	return bignum_divide(b1,b2);
}



void bcd_add_test()
{
	bcd_bignum b1 = bcd_bignum_make(6);
	bcd_bignum b2 = bcd_bignum_make(9);
	uint8_t num1_static[] = {1,0,0,0,0,0};
	uint8_t num2_static[] ={9,9,9,9,9,9,9,9,0};
	
	memcpy(b1.bignum, num1_static, b1.bignum_size);
	memcpy(b2.bignum, num2_static, b2.bignum_size);

	for(int i =0; i < 8; i++){
		printf("%x",b1.bignum[i] );	
	}
	puts("");
	bcd_bignum_add(b1, b2);

	for(int i =0; i < 9; i++){
		printf("%x",b1.bignum[i] );	
	}
}



int main()
{
	
	bcd_add_test();

	return 0;
}
