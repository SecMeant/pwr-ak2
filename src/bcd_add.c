#include "bcd_common.h"

void bcd_bignum_add(bcd_bignum b1, bcd_bignum b2){
  
  uint8_t carry = 0;
  // find smallest value
  if(b1.bignum_size < b2.bignum_size){
    bcd_bignum_realloc(&b1, b2.bignum_size);
  }

  // add bignums
  for (int64_t i = 0; i < b1.bignum_size; ++i)
  {
    b1.bignum[i] += b2.bignum[i] + carry;
    carry =0;
    if(b1.bignum[i] > 9){
      b1.bignum[i] = (b1.bignum[i] + 6) & BCD_U8_MASK;
      carry = 1;
    }
  }
}
