#include <stdio.h>
#include <stdint.h>
#include "common.h"

void shift_test()
{
	int64_t num1[2] = {0x8000000000000000 ,0x0000000200000000};
	bignum bignum_sh = {.bignum = num1, .bignum_size = 2};
	printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
	bignum_shift_left(bignum_sh, 4);
	printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
	bignum_shift_right(bignum_sh, 6);
	printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
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
	first.bignum_size = BIGNUM_COMMON_SIZE;
	first.bignum = (int64_t*)malloc(sizeof(uint64_t)*first.bignum_size);
	first.bignum[0] = 0xfffffffffffffffe;
	first.bignum[1] = 0xffffffffffffffff;
	first.bignum[2] = 0xffffffffffffffff;
	first.bignum[3] = 0xffffffffffffffff;
	
	second.bignum_size = BIGNUM_COMMON_SIZE;
	second.bignum = (int64_t*)malloc(sizeof(uint64_t)*second.bignum_size);
	second.bignum[0] = 0xfffffffffffffffd;
	second.bignum[1] = 0xffffffffffffffff;
	second.bignum[2] = 0xffffffffffffffff;
	second.bignum[3] = 0xffffffffffffffff;

	bignum res = bignum_multiply_fixed(first,second);
	for(int i =0; i < 4; i++)
		printf( "%lx\n", (uint64_t)res.bignum[i] );
}

void power_test(){
	bignum first;
	first.bignum_size = BIGNUM_COMMON_SIZE;
	first.bignum = (int64_t*)malloc(sizeof(uint64_t)*first.bignum_size);
	first.bignum[0] = 0xfffffffffffffffd;
	first.bignum[1] = 0x5454545454545454;
	first.bignum[2] = 0x3434343434343433;
	first.bignum[4] = 0;
	first.bignum[5] = 0;
	first.bignum[6] = 0;
	first.bignum[7] = 0;

	bignum res = bignum_power(first,2);
	for(int i =0; i < BIGNUM_COMMON_SIZE; i++)
		printf( "%lx\n", (uint64_t)res.bignum[i] );
}


int main()
{
	power_test();
	return 0;
}
