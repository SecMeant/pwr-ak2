#include "bcd_common.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

void bcd_bignum_shift_left(bcd_bignum b, int64_t sw)
{
  // Shift out.
  int64_t i = b.bignum_size -1;
  while(i >= b.bignum_size - sw)
  {
    b.bignum[i] = 0;
    --i;
  }

  for(int64_t i = b.bignum_size - 1; i >= 0; --i)
  {
    b.bignum[i+sw] = b.bignum[i];
  }

  for(int64_t i = sw; i >= 0; --i)
  {
    b.bignum[i] = 0;
  }
}

void bcd_bignum_shift_right(bcd_bignum b, int64_t sw)
{
  // Shift out.
  int64_t from = sw;
  int64_t to = from-sw;
  while(from < b.bignum_size)
  {
    b.bignum[to] = b.bignum[from];
    ++from;
    ++to;
  }

  while(to < b.bignum_size)
  {
    b.bignum[to] = 0;
    ++to;
  }
}

void bcd_bignum_print(bcd_bignum b)
{
  for(int64_t digit = b.bignum_size -1; digit >= 0; --digit)
  {
    printf("%hhi", b.bignum[digit]);
  }
  puts("");
}

void bcd_bignum_free(bcd_bignum b)
{
  free(b.bignum);
  b.bignum_size = 0;
}

void bcd_bignum_alloc(bcd_bignum *b1, int64_t size)
{
  b1->bignum = (uint8_t*) malloc(size * BCD_CHUNK_SIZE);
  b1->bignum_size = size;
}

bcd_bignum bcd_bignum_make(int64_t size)
{
  bcd_bignum ret;
  ret.bignum = (uint8_t*) malloc(size * BCD_CHUNK_SIZE);
  ret.bignum_size = size;
  assert(ret.bignum);
  return ret;
}

void bcd_bignum_realloc(bcd_bignum *b, int64_t newsize)
{
  b->bignum = (uint8_t*)realloc(b->bignum,
                               newsize * BCD_CHUNK_SIZE);
  if(newsize > b->bignum_size )
    memset(b->bignum + b->bignum_size, 0, newsize - b->bignum_size);
  
  b->bignum_size = newsize;
  assert(b->bignum);
}