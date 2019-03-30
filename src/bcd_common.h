#include <stdint.h>
#pragma once
#define BCD_U8_MASK 0x0f
// we will use unpacked bcd technique
typedef struct bcd_bignum_
{
	uint8_t *bignum;
	int64_t bignum_size;
} bcd_bignum;

typedef struct bcd_bignum_divide_result_
{
	bcd_bignum result;
	bcd_bignum reminder;
}bcd_bignum_divide_result;


void bcd_bignum_add(bcd_bignum b1, bcd_bignum b2);
// void bcd_bignum_sub(bcd_bignum b1, bcd_bignum b2);
// bcd_bignum bcd_bignum_mul(bcd_bignum b1, bcd_bignum b2);
// bcd_bignum_divide_result bcd_bignum_mul(bcd_bignum b1, bcd_bignum b2);

