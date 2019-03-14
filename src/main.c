#include <stdio.h>
#include <stdint.h>

extern void add_bignum_fast(int64_t *first, int64_t *second, int64_t size);

int main()
{
	int64_t num1[2] = {0x8000000000000000 ,0x0000000200000000};
	int64_t num2[2] = {0x8000000000000000 ,0x0003800000000000};
	add_bignum_fast(num1, num2, 2);
	printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
	return 0;
}
