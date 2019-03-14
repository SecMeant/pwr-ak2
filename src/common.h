#pragma once
#include <stdint.h>

typedef struct bignum_
{
	int64_t *bignum;
	int64_t bignum_size;
} bignum;

extern void bignum_add(bignum, bignum);
extern void bignum_shift_left(bignum a, int64_t sw);
extern void bignum_shift_right(bignum a, int64_t sw);

