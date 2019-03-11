#include <stdio.h>
#include <stdint.h>

extern void asm_main(void);
extern int64_t* add_bignum(int64_t *first, int64_t *second, int64_t size);

int main()
{
	int64_t num1[2] = {0x8000000000000000 ,0x0000000200000000};
	int64_t num2[2] = {0x8000000000000000 ,0x0003800000000000};
	int64_t *ret = add_bignum(num1, num2, 2);
	printf("0x%016lx 0x%016lx\n", *(ret+1), *ret);
	return 0;
}
