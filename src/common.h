#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

// Size of bignum chunk is bytes
#define CHUNK_SIZE 8

typedef struct bignum_
{
	int64_t *bignum;
	int64_t bignum_size;
} bignum;

extern void bignum_add(bignum, bignum);
extern void bignum_subtract(bignum, bignum);
extern void bignum_shift_left(bignum a, int64_t sw);
extern void bignum_shift_right(bignum a, int64_t sw);

bool bignum_is_negative(bignum b1);
void bignum_copy(bignum b1, bignum b2);
