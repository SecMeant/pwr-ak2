#include "bcd_common.h"
#include <stdlib.h>
#include <assert.h>


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