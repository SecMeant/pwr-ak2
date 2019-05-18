#include "bignum_common.h"
#include <string.h>
#include <stdio.h>

#include "parse.h"

int64_t max(int64_t a, int64_t b)
{
  if(a > b) return a;
  return b;
}

bignum_divide_result bignum_divide(bignum b1, bignum b2)
{
  if(bignum_is_zero(b2))
    bignum_fatal_error("Attempted division by 0.",
                       BIGNUM_ERR_ZERO_DIV);

  int64_t shift = bignum_effective_width(b1) - bignum_effective_width(b2);
  const int64_t arg_width =
    bignum_bit_size_to_chunks(shift+1) + b2.bignum_size;
  int64_t divisor_buffer[BIGNUM_MAX_STACK_ALLOC_SIZE];
  bignum result = bignum_make(bignum_bit_size_to_chunks(shift+1));
  bignum divident = bignum_extend(b1, 0);

  bignum divisor;
  divisor.bignum_size = b2.bignum_size + arg_width;
  
  if(divisor.bignum_size > BIGNUM_MAX_STACK_ALLOC_SIZE){
    divisor = bignum_make(divisor.bignum_size);
  }
  else{
    divisor.bignum = divisor_buffer;
  }

  bignum_copy(divisor, b2);

  bignum_shift_left_inp(divisor, shift);
  while(shift >= 0)
  {
    if(bignum_is_negative(divident))
      bignum_add_inp(divident, divisor);
    else
      bignum_sub_inp(divident, divisor);

    bignum_shift_left_inp(result, 1);
    if(!bignum_is_negative(divident))
      bignum_or_1_inp(result);

    if(bignum_is_zero(divident))
    {
      bignum_shift_left_inp(result, shift);
      break;
    }

    bignum_shift_right_inp(divisor, 1);
    --shift;
  }

  if(bignum_is_negative(divident))
    bignum_add_inp(divident, b2);
  
  if(divisor.bignum != divisor_buffer)
    bignum_free(divisor);
  
  bignum_divide_result ret;
  ret.result = result;
  ret.reminder = divident;
  return ret;
}

bignum bignum_mod(bignum divident, bignum divisor)
{
  bignum_divide_result b = bignum_divide(divident, divisor);
  bignum_free(b.result);

  return b.reminder;
}

bignum bignum_gcd(bignum b1, bignum b2)
{
  bignum b1_p = bignum_extend(b1, b1.bignum_size);
  bignum b2_p = bignum_extend(b2, b2.bignum_size);

  if(bignum_greater_than(b1_p, b2_p))
  {
    bignum_swap(b1_p, b2_p);
  }

  bignum reminder;
  while(!bignum_is_zero(b2_p))
  {
    reminder = bignum_mod(b1_p, b2_p);
    bignum_free(b1_p);
    b1_p = b2_p;
    b2_p = reminder;
  }

  bignum_free(b2_p);
  return b1_p;
}

// ****** BIGNUM SCHOOLBOOK DIVISION IMPLEMENTATION AND HELPERS *******
// B/2 = (2**32) / 2
static const uint32_t BETA_OVER_2 = 1 << 31;

static bool bignum_divide_is_normalized(bignum b1)
{
  int64_t chunk = b1.bignum[b1.bignum_size-1];

  // If upper word is not zero, check upper word for normalization
  if(chunk & 0xffffffff00000000)
    chunk >>= 32;

  return chunk & 0x80000000;
}

// Normalizes arguments for division
static void bignum_divide_normalize_pair_inaa
(bignum b1, bignum b2, bignum *nb1, bignum *nb2)
{
  int64_t effw = bignum_effective_width(b2);
  effw = bignum_bit_size_to_chunks(effw);

  bignum rb1 = bignum_make(effw);
  bignum_copy(rb1, b2);

  int64_t shift = 0;
  // TODO(holz) maybe calculate it and do it in one run
  while(!bignum_divide_is_normalized(rb1))
  {
    // Shift doesnt have to be safe, there is guarantee that shift
    // wont shift out some bits -- earlier number will be normalized.
    bignum_shift_left_inp(rb1, 1);
    ++shift;
  }

  bignum rb2 = bignum_extend(b1, 0);
  bignum_shift_left_inp_safe(&rb2, shift);

  *nb2 = rb1;
  *nb1 = rb2;
}

// CAUTION: word_index must be in range <0, bignum size * 2>
static int64_t bignum_divide_get_word(bignum b1, int64_t word_index)
{
  assert(b1.bignum_size * 2 > word_index);
  return ((int32_t*)b1.bignum)[word_index] & 0xffffffff;
}

// CAUTION: word_index must be in range <0, bignum size * 2>
void static bignum_divide_set_word(bignum b1, int64_t word_index, int64_t word)
{
  assert(b1.bignum_size * 2 > word_index);
  ((int32_t*)b1.bignum)[word_index] = word;
}

static int64_t bignum_effective_word_width(bignum b1)
{
  int64_t effw = bignum_effective_width(b1);
  effw = bignum_bit_size_to_chunks(effw);
  int64_t mse_chunk = b1.bignum[effw-1];

  if(mse_chunk & 0xffffffff00000000)
    return effw*2;
  else
    return effw*2 - 1;
}

// Base used when dividing
#define DIV_BASE 32

bignum_divide_result bignum_schoolbook_divide(bignum b1, bignum b2)
{
  bignum divident, divisor;
  bignum_divide_normalize_pair_inaa(b1, b2, &divident, &divisor);

  puts("====");
  bignum_print(b1);
  bignum_print(b2);
  bignum_print(divident);
  bignum_print(divisor);
  puts("====");

  const int64_t A_word_size = bignum_effective_word_width(divident);
  const int64_t B_word_size = bignum_effective_word_width(divisor);
  const int64_t m = A_word_size - B_word_size;
  printf("m=%li\n", m);

  bignum quotient = bignum_make(divident.bignum_size);

  // TODO if A >= B*(Beta**m) ...
  if(bignum_divide_get_word(divident, A_word_size-1) >=
     bignum_divide_get_word(divisor, B_word_size-1))
  bignum mul_res = bignum_make(b1.bignum_size+1);
  bignum restore_shift = bignum_make(m/2);
  for(int64_t j = m -1; j >= 0; --j)
  {
    // ****** DEBUG PRINT ******
    printf("Divident: ");
    bignum_print(divident);
    printf("Divisor: ");
    bignum_print(divisor);
    // *************************

    uint64_t chunkA = (bignum_divide_get_word(divident, B_word_size+j) << 32) |
                     (bignum_divide_get_word(divident, B_word_size+j-1));

    uint64_t chunkB = bignum_divide_get_word(divisor, B_word_size-1);
    uint32_t q = chunkA / chunkB;
    printf("a=%lx b=%lx\n", chunkA, chunkB);
    printf("j=%li q=%x\n", j,q);
    if(q > BETA_OVER_2)
      q = BETA_OVER_2;

    bignum_divide_set_word(quotient, B_word_size+j, q);
    bignum_multiply_inaa(quotient, divisor, mul_res);

    // ****** DEBUG PRINT ******
    printf("Quotient: ");
    bignum_print(quotient);
    printf("Divisor: ");
    bignum_print(divisor);
    printf("Mul result: ");
    bignum_print(mul_res);
    // *************************

    while(1){}

    bignum_sub_inp(divident, mul_res);
    while(bignum_is_negative(divident))
    {
      puts("neg");
      --q;
      bignum_copy(restore_shift, divisor);
      bignum_shift_left_inp_safe(&restore_shift, 32*j);
      bignum_add_inp(divident, restore_shift);
    }
    printf("partial result=%x\n", q);
  }
  bignum_free(mul_res);
  bignum_free(restore_shift);
  bignum_free(quotient);

  puts("");

  bignum_divide_result b = {divident, divisor};
  return b;
}


