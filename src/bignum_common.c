#include "bignum_common.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void bignum_fatal_error(const char *msg, int64_t errno)
{
  fprintf(stderr, "BIGNUM FATAL ERROR! %s\n", msg);
  exit(errno);
}

void bignum_sub_scaled_total_inp(bignum b1, bignum b2, int64_t bit_sc)
{
  // Equivalent to division by 64
  int64_t chunk_sc = bit_sc >> 6;

  // Equivalent to reminder from division by 64
  bit_sc &= ((1 << 6)-1);

  bignum_sub_scaled_inp(b1, b2, chunk_sc, bit_sc);
}

void bignum_shift_left_inp(bignum a, int64_t sw)
{
  int64_t chunk_shift = 0;

  if(sw >= 64)
  {
    // Equivalent: chunk_shift = sw / 64;
    chunk_shift = sw >> 6;

    // Equivalent: sw = sw % 64
    sw &= ((1<<6)-1);

    bignum_shift_chunk_left_inp(a, chunk_shift);
  }

  bignum_shift_64_inp(a, sw);
}

void bignum_shift_left_inp_safe(bignum *a, int64_t sw)
{
  int64_t space_left = 
    a->bignum_size * BIGNUM_CHUNK_SIZE_BITS - bignum_effective_width(*a);

  if(sw > space_left)
    bignum_resize_inp(a, a->bignum_size + bignum_bit_size_to_chunks(sw-space_left));

  bignum_shift_left_inp(*a, sw);
}

void bignum_shift_right_inp(bignum a, int64_t sw)
{
  int64_t chunk_shift = 0;

  if(sw >= 64)
  {
    // Equivalent: chunk_shift = sw / 64;
    chunk_shift = sw >> 6;

    // Equivalent: sw = sw % 64
    sw &= ((1<<6)-1);

    bignum_shift_chunk_right_inp(a, chunk_shift);
  }

  bignum_right_64_inp(a, sw);
}

void bignum_shift_right_inp_safe(bignum *a, int64_t sw)
{
  // Shifting right is fine, dont have to check anything
  bignum_shift_right_inp(*a, sw);
}

void bignum_print_chunks(bignum b)
{
  int64_t index = b.bignum_size-1;
  while(index >= 0)
  {
    printf("0x%016lx ", b.bignum[index]);
    --index;
  }
  putchar('\n');
}

void bignum_print(bignum b)
{
  int64_t index = b.bignum_size-1;
  printf("0x");
  while(index >= 0)
  {
    printf("%016lx", b.bignum[index]);
    --index;
  }
  putchar('\n');
}

bool bignum_is_zero(bignum b1)
{
  int64_t index = b1.bignum_size - 1;
  while(index >= 0)
  {
    if(b1.bignum[index] != 0)
      return false;
    --index;
  }
  return true;
}

bignum bignum_extend(bignum b, int64_t size)
{
  bignum ret;
  ret.bignum_size = b.bignum_size + size;
  ret.bignum = (int64_t*)calloc(BIGNUM_CHUNK_SIZE, ret.bignum_size);
  bignum_copy(ret, b);
  return ret;
}

bignum bignum_extend_twice(bignum b1)
{
  bignum ret;
  ret.bignum_size = b1.bignum_size * 2;
  ret.bignum = (int64_t*)malloc(ret.bignum_size * BIGNUM_CHUNK_SIZE);
  bignum_copy(ret, b1);
  return ret;
}

void bignum_resize_inp(bignum *b, int64_t new_chunk_size)
{
  assert(new_chunk_size > 0);

  if(new_chunk_size == b->bignum_size)
  { return; }

  b->bignum = realloc(b->bignum, new_chunk_size * BIGNUM_CHUNK_SIZE);
  assert(b->bignum);

  if(new_chunk_size > b->bignum_size)
  {
    memset(b->bignum+b->bignum_size, 0, 
           (new_chunk_size - b->bignum_size) * BIGNUM_CHUNK_SIZE);
  }

  b->bignum_size = new_chunk_size;
}

void bignum_or_1_inp(bignum b1)
{
  if(b1.bignum_size == 0)
    return;
  b1.bignum[0] |= 1;
}

int64_t bignum_effective_width(bignum b)
{
  int64_t effective_size = 0;
  for(int64_t  chunk = b.bignum_size -1; chunk >= 0; --chunk)
  {
    if(b.bignum[chunk] == 0)
      continue;

    effective_size = BIGNUM_CHUNK_SIZE_BITS -1;
    while(effective_size >= 0)
    {
      if(b.bignum[chunk] >> effective_size != 0)
        return effective_size + 64 * chunk + 1;
      --effective_size;
    }
  }

  return 0;
}

int64_t bignum_bit_size_to_chunks(int64_t bitsize)
{
  int64_t ret = bitsize / BIGNUM_CHUNK_SIZE_BITS;
  if(bitsize % BIGNUM_CHUNK_SIZE_BITS) ret += 1;
  return ret;
}

void bignum_shift_chunk_left_inp(bignum a, int64_t sw)
{
  int64_t index = a.bignum_size - sw - 1;
  while(index >=0)
  {
    int64_t chunk = a.bignum[index];
    a.bignum[index] = 0;

    if(index + sw < a.bignum_size)
    {
      a.bignum[index+sw] = chunk;
    }

    --index;
  }
}

void bignum_shift_chunk_right_inp(bignum a, int64_t sw)
{
  // Start from shift, earlier values will be shifted out anyway
  int64_t index = sw;

  // Proper shifting part
  while(index < a.bignum_size)
  {
    a.bignum[index-sw] = a.bignum[index];
    ++index;
  }

  // Fall back to last valid index
  --index;

  // Get index of lastly correctly shifted value
  sw = index-sw;

  // Shifting in zeros
  while(index != sw)
  {
    a.bignum[index] = 0;
    --index;
  }
}

void bignum_free(bignum b)
{
  free(b.bignum);
  b.bignum_size = 0;
}

void bignum_alloc_inp(bignum *b1, int64_t size)
{
  b1->bignum = malloc(size * BIGNUM_CHUNK_SIZE);
  b1->bignum_size = size;
}

bignum bignum_make(int64_t size)
{
  assert(size >= 0);

  bignum ret;
  ret.bignum = (int64_t*) calloc(BIGNUM_CHUNK_SIZE, size);
  ret.bignum_size = size;
  assert(ret.bignum);
  return ret;
}

void bignum_realloc_inp(bignum *b, int64_t newsize)
{
  b->bignum = (int64_t*)realloc(b->bignum,
                               newsize * BIGNUM_CHUNK_SIZE);
  b->bignum_size = newsize;
  assert(b->bignum);
}

bignum bignum_rand(int64_t bignum_size, bignum modulo){
  bignum b1 = bignum_make(bignum_size);
  srand(time(0));
  while(--bignum_size)
    b1.bignum[bignum_size] = (int64_t)rand() | (((int64_t)( rand())<<31));
  
  bignum res = bignum_mod(b1, modulo);
  bignum_free(b1);
  return res;
}
