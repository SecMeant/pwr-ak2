#include <stdint.h>
#include <stdlib.h>

typedef struct bignum
{
	int64_t *bignum;
	int64_t bignum_size;
}bignum;

extern void add_bignum_fast(int64_t *first, int64_t *second, int64_t size);
extern void bignum_shift_left(struct bignum a, int64_t sw);
extern void bignum_shift_right(struct bignum a, int64_t sw);
extern bignum bignum_multiply(bignum first, bignum second);

void shift_test()
{
	int64_t num1[2] = {0x8000000000000000 ,0x0000000200000000};
	int64_t num2[2] = {0x8000000000000000 ,0x0003800000000000};
	struct bignum bignum_sh = {.bignum = num1, .bignum_size = 2};
	printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
	bignum_shift_left(bignum_sh, 4);
	printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
	bignum_shift_right(bignum_sh, 6);
	printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);

}


int main()
{
	shift_test();
	// bignum first, second;
	// first.size = 2;
	// first.bignum = (int64_t*)malloc(sizeof(uint64_t)*first.size);
	// first.val[0] = 0xffffffffffffffff;
	// first.val[1] = 0xffffffffffffffff;
	// second.size = 2;
	// second.val = (int64_t*)malloc(sizeof(uint64_t)*second.size);
	// second.val[0] = 0xffffffffffffffff;
	// second.val[1] = 0xffffffffffffffff;
		
	// bignum_multiply(first,second);
	return 0;
}
