#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

// Size of bignum chunk is bytes
#define CHUNK_SIZE 8
#define BIGNUM_COMMON_SIZE 8
typedef struct bignum_
{
	int64_t *bignum;
	int64_t bignum_size;
} bignum;

extern void bignum_add(bignum, bignum);
extern void bignum_subtract(bignum, bignum);
extern void bignum_increment(bignum a);
extern void bignum_shift_left(bignum a, int64_t sw);
extern void bignum_shift_right(bignum a, int64_t sw);

extern bignum bignum_modul(bignum,bignum);
extern bignum bignum_multiply(bignum first, bignum second);
extern bignum bignum_multiply_fixed(bignum first, bignum second);

bool bignum_is_negative(bignum b1);
void bignum_copy(bignum b1, bignum b2);

bignum bignum_power(bignum num, int exponent);


bool bignums_are_equal(bignum a, bignum b);
bool bignum_less_than(bignum lhs, bignum rhs);
bool bignum_greater_than(bignum a, bignum b);
bool trial_test(bignum a);