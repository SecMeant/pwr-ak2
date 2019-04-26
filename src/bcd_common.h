#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#define BCD_U8_MASK 0x0f
#define BCD_CHUNK_SIZE sizeof(uint8_t)
#define BCD_COMMON_SIZE 2048
#define BCD_ERR_ZERO_DIV -11

extern const char * bcd_bignum_example;

typedef enum ENDIANES_{
  LITTLE_ENDIAN,
  BIG_ENDIAN
}ENDIANES;

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

void bcd_bignum_shift_left(bcd_bignum b, int64_t sw);
void bcd_bignum_shift_right(bcd_bignum b, int64_t sw);
bool bcd_bignum_is_zero(bcd_bignum b);
void bcd_bignum_fatal_error(const char *msg, int64_t errno);
void bcd_bignum_copy(bcd_bignum dst, bcd_bignum src);
bcd_bignum bcd_bignum_extend(bcd_bignum b, int64_t size);
bcd_bignum bcd_bignum_extend_twice(bcd_bignum b);
bool bcd_bignum_is_negative(bcd_bignum b);
void bcd_bignum_or_1(bcd_bignum b);
bool bcd_trial_test(bcd_bignum b1);
int64_t bcd_bignum_effective_width(bcd_bignum b);

// sum first bignum with sencod bignum
// and save result in first bignum
// if neccessary function realloc first bignum to proper size
void bcd_bignum_add(bcd_bignum b1, bcd_bignum b2);

// from first bignum subtract sencond
// and save result in first bignum
// if neccessary function realloc first bignum to proper size
// TODO(all) change name to bcd_bignum_subtract for compatibility.
void bcd_bignum_sub(bcd_bignum b1, bcd_bignum b2);

// function allocates memory space enought to save whole result
// which means the result can be as big as sum of length of two bignums size
bcd_bignum bcd_bignum_mul(bcd_bignum b1, bcd_bignum b2);
bcd_bignum_divide_result bcd_bignum_divide(bcd_bignum b1, bcd_bignum b2);

// make multiplication and save result in allocated memory
// which size is as big as size of the larger factor
// NOTE the result can be cut
bcd_bignum bcd_bignum_mul_fixed(bcd_bignum b1, bcd_bignum b2);
// multiply bignums and save result in given argument
void bcd_bignum_mul_fixed_no_return(bcd_bignum b1, bcd_bignum b2, bcd_bignum result);
bcd_bignum bcd_bignum_power(bcd_bignum num, int exponent);

void bcd_bignum_add(bcd_bignum b1, bcd_bignum b2);
void bcd_increment(bcd_bignum b1);

void bcd_bignum_print(bcd_bignum b);
// bcd memory managment stuff
void bcd_bignum_free(bcd_bignum b);
bcd_bignum bcd_bignum_make(int64_t size);
void bcd_bignum_alloc(bcd_bignum *b1, int64_t size);
// realloc memory with given size
// if area is expaned set additional part of memory to 0
void bcd_bignum_realloc(bcd_bignum *b1, int64_t newsize);

// convert given number from ascii to N10
// mode describes the endianes of given number
// save result in dst table, the result is always little endian
void to_bcd_number(const char* number, ENDIANES mode, uint8_t* dst);


bool bcd_bignums_are_equal(bcd_bignum lhs, bcd_bignum rhs);
bool bcd_bignum_less_than(bcd_bignum lhs, bcd_bignum rhs);
bool bcd_bignum_greater_than(bcd_bignum lhs, bcd_bignum rhs);
