#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

// Size of bignum chunk is bytes
#define CHUNK_SIZE 8
// Size of bognum chunk in bits
#define CHUNK_SIZE_BITS 64

#define ERR_ZERO_DIV -11

typedef struct bignum_
{
	int64_t *bignum;
	int64_t bignum_size;
} bignum;

typedef struct bignum_divide_result_
{
	bignum result;
	bignum reminder;
}bignum_divide_result;

void bignum_fatal_error(const char *msg, int64_t errno);

extern void bignum_add(bignum, bignum);
extern void bignum_subtract(bignum, bignum);
extern void bignum_shift_left(bignum a, int64_t sw);
extern void bignum_shift_right(bignum a, int64_t sw);
extern void bignum_shift_left_64(bignum a, int64_t sw);
extern void bignum_shift_right_64(bignum a, int64_t sw);
extern bignum bignum_multiply(bignum first, bignum second);
extern bignum bignum_multiply_fixed(bignum first, bignum second);
extern bignum_divide_result bignum_divide(bignum, bignum);

void bignum_print(bignum b);

bool bignum_is_negative(bignum b1);
bool bignum_is_zero(bignum b1);
void bignum_copy(bignum b1, bignum b2);

bignum bignum_power(bignum num, int exponent);
bignum bignum_extend_twice(bignum b1);
void bignum_or_1(); // inserts 1 to lsb. (used when dividing)

void bignum_shift_chunk_left(bignum a, int64_t sw);
void bignum_shift_chunk_right(bignum a, int64_t sw);

void bignum_free(bignum b);
bignum bignum_make(int64_t size);
void bignum_alloc(bignum *b1, int64_t size);
void bignum_realloc(bignum *b1, int64_t newsize);
