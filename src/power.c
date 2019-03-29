#include "common.h"
#include <stdio.h>
bignum bignum_power(bignum num, int exponent){
  
  int64_t temp[BIGNUM_COMMON_SIZE] = {0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
  int64_t* tmp_res = NULL, *tmp_num = NULL;
  
  // set init value
  bignum result;
  result.bignum_size = BIGNUM_COMMON_SIZE;
  result.bignum = temp;

  while( exponent > 0){
    
    if(exponent & 1){
      result = bignum_multiply_fixed(result,num);
      if(tmp_res != NULL)
        free(tmp_res);
      tmp_res = result.bignum;
      
    }
    exponent = exponent>>1;

    num = bignum_multiply_fixed(num,num);
    if(tmp_num != NULL)
      free(tmp_num);
    tmp_num = num.bignum;

  }

  free(num.bignum);
  return result;
}

// bignum bignum_power_mod(bignum number, int exponent, int mod){

// }
