#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#define BCD_U8_MASK 0x0f
#define BCD_CHUNK_SIZE sizeof(uint8_t)
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

// add first bignum with sencod bignum 
// and save result in first bignum
// if neccessary function realloc first bignum to proper size
void bcd_bignum_add(bcd_bignum b1, bcd_bignum b2);

// from first bignum subtract sencond  
// and save result in first bignum
// if neccessary function realloc first bignum to proper size
void bcd_bignum_sub(bcd_bignum b1, bcd_bignum b2);

bcd_bignum bcd_bignum_mul(bcd_bignum b1, bcd_bignum b2);

// bcd_bignum_divide_result bcd_bignum_mul(bcd_bignum b1, bcd_bignum b2);

// bcd memory managment stuff
void bcd_bignum_free(bcd_bignum b);
bcd_bignum bcd_bignum_make(int64_t size);
void bcd_bignum_alloc(bcd_bignum *b1, int64_t size);
// realloc memory with given size
// if area is expaned set additional part of memory to 0
void bcd_bignum_realloc(bcd_bignum *b1, int64_t newsize);