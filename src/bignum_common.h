#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

// Size of bignum chunk is bytes
#define CHUNK_SIZE 8
#define BIGNUM_COMMON_SIZE 100
// Size of bognum chunk in bits
#define CHUNK_SIZE_BITS 64

#define ERR_ZERO_DIV -11

#define BIGNUM_MAX_STACK_ALLOC_SIZE 20

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

void bignum_add_inp(bignum, bignum);
void bignum_sub_inp(bignum, bignum);
void bignum_increment_inp(bignum a);
void bignum_shift_left_inp_64_inp(bignum a, int64_t sw);
void bignum_shift_right_inp_64_inp(bignum a, int64_t sw);
void bignum_shift_left_inp(bignum a, int64_t sw);
void bignum_shift_right_inp(bignum a, int64_t sw);
void bignum_shift_chunk_left_inp(bignum a, int64_t sw);
void bignum_shift_chunk_right_inp(bignum a, int64_t sw);

bignum bignum_multiply(bignum first, bignum second);
bignum bignum_multiply_fixed(bignum first, bignum second);
void bignum_multiply_inaa(bignum first, bignum second, bignum result);
bignum_divide_result bignum_divide(bignum, bignum);
bignum bignum_mod(bignum, bignum);

void bignum_print_chunks(bignum b);
void bignum_print(bignum b);

bool bignum_is_negative(bignum b1);
bool bignum_is_zero(bignum b1);
void bignum_copy(bignum b1, bignum b2);

bignum bignum_power(bignum num, int exponent);
bignum bignum_extend_twice(bignum b1);
bignum bignum_extend(bignum b, int64_t size);
void bignum_or_1_inp(bignum b); // inserts 1 to lsb. (used when dividing)

int64_t bignum_effective_width(bignum b);
int64_t bignum_bit_size_to_chunks(int64_t bitsize);

void bignum_free(bignum b);
bignum bignum_make(int64_t size);
void bignum_alloc_inp(bignum *b1, int64_t size);
void bignum_realloc_inp(bignum *b1, int64_t newsize);
bool bignums_are_equal(bignum a, bignum b);
bool bignum_less_than(bignum lhs, bignum rhs);
bool bignum_greater_than(bignum a, bignum b);
bool trial_test(bignum a);
