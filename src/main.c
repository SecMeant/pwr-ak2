#include <stdint.h>
#include <stdlib.h>

typedef struct bingum_info{
	int64_t* val;
	uint64_t size;
} bignum;

extern int64_t* add_bignum(int64_t *first, int64_t *second, int64_t size);

extern bignum bignum_multiply(bignum first,bignum second);
extern void subtract_test(void);
int main()
{
	subtract_test();
	// bignum first, second;
	// first.size = 2;
	// first.val = (int64_t*)malloc(sizeof(uint64_t)*first.size);
	// first.val[0] = 0xffffffffffffffff;
	// first.val[1] = 0xffffffffffffffff;
	// second.size = 2;
	// second.val = (int64_t*)malloc(sizeof(uint64_t)*second.size);
	// second.val[0] = 0xffffffffffffffff;
	// second.val[1] = 0xffffffffffffffff;
		
	// bignum_multiply(first,second);
	return 0;
}
