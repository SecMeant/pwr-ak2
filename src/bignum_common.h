#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

// Size of bignum chunk is bytes
#define BIGNUM_CHUNK_SIZE 8
#define BIGNUM_COMMON_SIZE 10
// Size of bognum chunk in bits
#define BIGNUM_CHUNK_SIZE_BITS 64

#define BIGNUM_ERR_ZERO_DIV -11

#define BIGNUM_MAX_STACK_ALLOC_SIZE 20

#define bignum_swap(b1, b2) \
{ \
  bignum tmp = b1; \
  b1 = b2; \
  b2 = tmp; \
}

typedef int64_t* bignum_underlay_type;
typedef int64_t  bignum_underlay_size_type;

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
// Scales second argument and subtracts from first, without modifying second.
void bignum_sub_scaled_inp(bignum, bignum, int64_t chunk_sc);
void bignum_increment_inp(bignum a);
void bignum_shift_64_inp(bignum a, int64_t sw);
void bignum_right_64_inp(bignum a, int64_t sw);
void bignum_shift_left_inp(bignum a, int64_t sw);
void bignum_shift_left_inp_safe(bignum *a, int64_t sw);
void bignum_shift_right_inp(bignum a, int64_t sw);
void bignum_shift_right_inp_safe(bignum *a, int64_t sw);
void bignum_shift_chunk_left_inp(bignum a, int64_t sw);
void bignum_shift_chunk_right_inp(bignum a, int64_t sw);

bignum bignum_multiply(bignum first, bignum second);
bignum bignum_multiply_fixed(bignum first, bignum second);
void bignum_multiply_inaa(bignum first, bignum second, bignum result);
bignum_divide_result bignum_divide(bignum, bignum);
bignum_divide_result bignum_schoolbook_divide(bignum b1, bignum b2);
bignum bignum_mod(bignum, bignum);
bignum bignum_gcd(bignum, bignum);
void bignum_print_chunks(bignum b);
void bignum_print(bignum b);

bool bignum_is_negative(bignum b1);
bool bignum_is_zero(bignum b1);
void bignum_copy(bignum b1, bignum b2);
void bignum_init(void);

bignum bignum_power(bignum num, int exponent);
bignum bignum_power_mod(bignum num, bignum p, int exponent);
bignum bignum_power_mod_bexp(bignum num, bignum p, bignum exponent);

// Creates new bignum with size of bignum passed as argument + size in bits.
// Copies value of bignum given as argument to created one.
bignum bignum_extend(bignum from, int64_t size);

// Creates new bignum with size twice as big as one given as argument.
// Copies value of one given as argument.
bignum bignum_extend_twice(bignum b1);

// Resizees bignum to new size.
// Too big values are truncated.
// New chunks are zero initialized.
void bignum_resize_inp(bignum *b, int64_t new_chunk_size);

// Sets first bit in first chunk of bignum passed as arg.
void bignum_or_1_inp(bignum b);

// Retuns size in bits without front zeros of given bignum.
int64_t bignum_effective_width(bignum b);

// Returns size in bignum chunks from size in bits.
int64_t bignum_bit_size_to_chunks(int64_t bitsize);
// Resturn bignum witch bignum_size chunks % modulo; 
bignum bignum_rand(int64_t bignum_size, bignum modulo);
void bignum_free(bignum b);
bignum bignum_make(int64_t size);
void bignum_alloc_inp(bignum *b1, int64_t size);
bool bignums_are_equal(bignum a, bignum b);
bool bignum_less_than(bignum lhs, bignum rhs);
bool bignum_greater_than(bignum lhs, bignum rhs);


bool trial_test(bignum a);
bool fermat_primality_test(bignum p, size_t probes);
