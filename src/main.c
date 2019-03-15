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

void div_test()
{
	int64_t num1[2] = {27,0};
	int64_t num2[2] = {3,0};
	bignum b1 = {.bignum = num1, .bignum_size = 2};
	bignum b2 = {.bignum = num2, .bignum_size = 2};
	bignum_divide(b1,b2);
}

int main()
{
	div_test();
	return 0;
}
