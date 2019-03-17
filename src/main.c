#include <stdio.h>
#include <stdint.h>
#include "common.h"

void shift_test()
{
	int64_t num1[] = {0,0,3,0};
	bignum bignum_sh = {.bignum = num1, .bignum_size = 4};
	bignum_print(bignum_sh);
	bignum_shift_right(bignum_sh, 1);
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
	first.bignum_size = 8;
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

}
void power_test(){
	bignum first;
	first.bignum_size = 8;
	first.bignum = (int64_t*)malloc(sizeof(uint64_t)*first.bignum_size);
	first.bignum[0] = 0xfffffffffffffffd;
	first.bignum[1] = 0x5454545454545454;
	first.bignum[2] = 0x3434343434343433;
	first.bignum[4] = 0;
	first.bignum[5] = 0;
	first.bignum[6] = 0;
	first.bignum[7] = 0;

	bignum res = bignum_power(first,2);
	for(int i =0; i < 8; i++)
		printf( "%lx\n", (uint64_t)res.bignum[i] );
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

int main()
{
	bignum_divide_result res = div_test();
	bignum_print(res.result);
	bignum_print(res.reminder);
	bignum_free(res.result);
	bignum_free(res.reminder);
	return 0;
}
