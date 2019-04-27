#include "bignum_common.h"
#include <stdio.h>
#include <string.h>
bignum bignum_power(bignum num, int exponent){

  int64_t *tmp_num;
  
  // set init value
  bignum result = bignum_make(BIGNUM_COMMON_SIZE);
  result.bignum[0] = 0x1;

  while( exponent > 0){
    if(exponent & 1){
      tmp_num = result.bignum;
      result = bignum_multiply(result,num);
      free(tmp_num);
    }

    exponent = exponent>>1;
    tmp_num = num.bignum;
    num = bignum_multiply(num,num);
    free(tmp_num);
    tmp_num = num.bignum;
  }
  free(num.bignum);
  return result;

}


bignum bignum_power_mod(bignum num, bignum p, int exponent){

  int64_t *tmp_num = NULL;
 
  bignum result = bignum_make(BIGNUM_COMMON_SIZE);
  result.bignum[0] = 0x1;

  // update num
  bignum tmp = bignum_mod(num,p);
  free(num.bignum);
  num = tmp;
  
  while( exponent > 0){

    if(exponent & 1){
      tmp_num = result.bignum;
      result = bignum_multiply(result,num);
      free(tmp_num);      

      tmp = bignum_mod(result,p);
      free(result.bignum);
      result = tmp; 
    }

    exponent = exponent>>1;
    tmp_num = num.bignum;
    num = bignum_multiply(num,num);
    free(tmp_num);
    
    tmp = bignum_mod(num,p);
    free(num.bignum);
    num = tmp;
  }
  
  free(num.bignum);
  return result;
}
