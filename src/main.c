#include <stdio.h>
#include <stdint.h>
#include "common.h"

struct bignum
{
	int64_t *bignum;
	int64_t bignum_size;
};

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


int main()
{
	add_test();
	return 0;
}
