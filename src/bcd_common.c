#include "bcd_common.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

const char * bcd_bignum_example = "524287";

void bcd_bignum_fatal_error(const char *msg, int64_t errno)
{
  fprintf(stderr, "BCD BIGNUM FATAL ERROR! %s\n", msg);
  exit(errno);
}

void bcd_bignum_shift_left(bcd_bignum b, int64_t sw)
{
  int64_t i = b.bignum_size - sw;
  int64_t j = i + sw;
  while(i >=0)
  {
    if(j < b.bignum_size)
      b.bignum[j] = b.bignum[i];
    --i;
    --j;
  }

  while(j >= 0)
  {
    b.bignum[j] = 0;
    --j;
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

bool bcd_bignum_is_zero(bcd_bignum b)
{
  for(int64_t i = b.bignum_size - 1; i >= 0; --i)
  {
    if(b.bignum[i] != 0)
      return false;
  }

  return true;
}

void bcd_bignum_copy(bcd_bignum dst, bcd_bignum src)
{
  int64_t len = dst.bignum_size < src.bignum_size ? dst.bignum_size : src.bignum_size;

  for(int64_t i = len-1; i >= 0; --i)
  {
    dst.bignum[i] = src.bignum[i];
  }

  for(int64_t i = dst.bignum_size; i > len; ++len)
  {
    dst.bignum[len] = 0;
  }
}

bcd_bignum bcd_bignum_extend(bcd_bignum b, int64_t size)
{
  bcd_bignum ret;
  ret.bignum_size = b.bignum_size + size;
  ret.bignum = (uint8_t*)malloc(ret.bignum_size * BCD_CHUNK_SIZE);
  bcd_bignum_copy(ret, b);
  return ret;
}

bcd_bignum bcd_bignum_extend_twice(bcd_bignum b)
{
  return bcd_bignum_extend(b, b.bignum_size*2);
}

bool bcd_bignum_is_negative(bcd_bignum b)
{
  return b.bignum[b.bignum_size-1] > 4;
}

void bcd_bignum_or_1(bcd_bignum b)
{
  assert(b.bignum_size >= 0);
  b.bignum[0] |= 1;
}

int64_t bcd_bignum_effective_width(bcd_bignum b)
{
  for(int64_t i = b.bignum_size - 1; i >=0; --i)
  {
    if(b.bignum[i] != 0)
      return i+1;
  }
  return 0;
}

bcd_bignum_divide_result bcd_bignum_divide(bcd_bignum b1, bcd_bignum b2)
{
  if(bcd_bignum_is_zero(b2))
    bcd_bignum_fatal_error("Attempted division by 0.",
                            BCD_ERR_ZERO_DIV);

  int64_t shift = bcd_bignum_effective_width(b1) - bcd_bignum_effective_width(b2);
  const int64_t arg_width = shift+1;
  bcd_bignum_divide_result ret;
  bcd_bignum result = bcd_bignum_make(b1.bignum_size);
  bcd_bignum divident = bcd_bignum_extend(b1, arg_width);

  if(shift < 0)
    goto bcd_bignum_divide_return;

  bcd_bignum divisor = bcd_bignum_extend(b2, arg_width);
  bcd_bignum one = bcd_bignum_make(result.bignum_size);
  one.bignum[0] = 1;
  bcd_bignum_copy(divisor, b2);

  bcd_bignum_shift_left(divisor, shift);
  while(shift >= 0)
  {
    bcd_bignum_shift_left(result, 1);
    bcd_bignum_sub(divident, divisor);
    while(!bcd_bignum_is_negative(divident))
    {
      bcd_bignum_add(result, one);
      bcd_bignum_sub(divident, divisor);
    }
    bcd_bignum_add(divident, divisor);
    --shift;
    bcd_bignum_shift_right(divisor,1);
  }

  bcd_bignum_free(divisor);
  bcd_bignum_free(one);

bcd_bignum_divide_return:
  ret.result = result;
  ret.reminder = divident;
  return ret;
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
  ret.bignum = (uint8_t*) calloc(BCD_CHUNK_SIZE, size);
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

bool bcd_bignums_are_equal(bcd_bignum lhs, bcd_bignum rhs){
  
  bcd_bignum_sub(lhs,rhs);

  for(int64_t i =0 ; i < lhs.bignum_size; i++)
    if(lhs.bignum[i] != 0)
      return false;
  return true;
}

bool bcd_bignum_less_than(bcd_bignum lhs, bcd_bignum rhs){

  bcd_bignum_sub(lhs,rhs);

  return bcd_bignum_is_negative(lhs); 
}

bool bcd_bignum_greater_than(bcd_bignum lhs, bcd_bignum rhs){
  
  bcd_bignum_sub(lhs,rhs);

  return !bcd_bignum_is_negative(lhs);  
}
void bcd_increment(bcd_bignum b1){
  int8_t carry = 1; 
  int64_t i =0;

  while(carry &&  i < b1.bignum_size){
    b1.bignum[i] += carry;
    carry =0;
    if(b1.bignum[i] > 9){
      b1.bignum[i] = (b1.bignum[i] + 6) & BCD_U8_MASK;
      carry = 1;
      i++;
    }
  }
}

void to_bcd_number(const char* number, ENDIANES mode, uint8_t* dst){
  int64_t i =0;
  
  if(mode == LITTLE_ENDIAN){
    // decode value    
    while(number[i] != 0){
      dst[i] = number[i] -'0';
      i++;
    }

  }else{  
    int64_t k =0;
    // find last value
    while(number[i] != 0)i++;
    i--;    
    // decode value 
    while(i >= 0){
      dst[k] = number[i] -'0';
      k++;
      i--;
    }  
  }
}
