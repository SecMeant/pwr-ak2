#include "bignum_common.h"
#include <stdio.h>

void bignum_fatal_error(const char *msg, int64_t errno)
{
  fprintf(stderr, "BIGNUM FATAL ERROR! %s\n", msg);
  exit(errno);
}

void bignum_shift_left(bignum a, int64_t sw)
{
  int64_t chunk_shift = 0;

  if(sw >= 64)
  {
    // Why C doesnt know how to divide with modulo ;/
    // TODO This can be twice as fast in assembly
    chunk_shift = sw / 64;
    sw = sw % 64;
    bignum_shift_chunk_left(a, chunk_shift);
  }

  bignum_shift_left_64(a, sw);
}

void bignum_shift_right(bignum a, int64_t sw)
{
  int64_t chunk_shift = 0;

  if(sw >= 64)
  {
    // Why C doesnt know how to divide with modulo ;/
    // TODO This can be twice as fast in assembly
    chunk_shift = sw / 64;
    sw = sw % 64;
    bignum_shift_chunk_right(a, chunk_shift);
  }

  bignum_shift_right_64(a, sw);
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

bignum bignum_extend_twice(bignum b1)
{
  bignum ret;
  ret.bignum_size = b1.bignum_size * 2;
  ret.bignum = (int64_t*)malloc(ret.bignum_size * CHUNK_SIZE);
  bignum_copy(ret, b1);
  return ret;
}

bignum bignum_extend(bignum b, int64_t size)
{
  bignum ret;
  ret.bignum_size = b.bignum_size + size;
  ret.bignum = (int64_t*)calloc(CHUNK_SIZE, ret.bignum_size);
  bignum_copy(ret, b);
  return ret;
}

void bignum_or_1(bignum b1)
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

    effective_size = CHUNK_SIZE_BITS -1;
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
  int64_t ret = bitsize / CHUNK_SIZE_BITS;
  if(bitsize % CHUNK_SIZE_BITS) ret += 1;
  return ret;
}

void bignum_shift_chunk_left(bignum a, int64_t sw)
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

void bignum_shift_chunk_right(bignum a, int64_t sw)
{
  int64_t index = sw + 1;
  while(index < a.bignum_size)
  {
    a.bignum[index-sw] = a.bignum[index];
    ++index;
  }
}

void bignum_free(bignum b)
{
  free(b.bignum);
  b.bignum_size = 0;
}

void bignum_alloc(bignum *b1, int64_t size)
{
  b1->bignum = malloc(size * CHUNK_SIZE);
  b1->bignum_size = size;
}

bignum bignum_make(int64_t size)
{
  assert(size >= 0);

  bignum ret;
  ret.bignum = (int64_t*) calloc(CHUNK_SIZE, size);
  ret.bignum_size = size;
  assert(ret.bignum);
  return ret;
}

void bignum_realloc(bignum *b, int64_t newsize)
{
  b->bignum = (int64_t*)realloc(b->bignum,
                               newsize * CHUNK_SIZE);
  b->bignum_size = newsize;
  assert(b->bignum);
}
