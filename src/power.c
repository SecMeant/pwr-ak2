#include "bignum_common.h"
#include <stdio.h>
#include <string.h>
bignum bignum_power(bignum num, int exponent){

  int64_t *tmp_num;
  
  // set init value
  bignum result = bignum_make(1);
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
 
  bignum result = bignum_make(1);
  result.bignum[0] = 0x1;

  // update num
  bignum tmp = bignum_mod(num,p);
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

bignum bignum_power_mod_bexp(bignum num, bignum p, bignum exponent){

  int64_t *tmp_num = NULL;
 
  bignum result = bignum_make(1);
  bignum const_0 = bignum_make(1);
  result.bignum[0] = 0x1;

  // update num
  bignum tmp = bignum_mod(num,p);
  num = tmp;

  while( bignum_greater_than(exponent,const_0)){

    if(exponent.bignum[0] & 1){
      tmp_num = result.bignum;
      result = bignum_multiply(result,num);

      free(tmp_num);
      tmp = result;      
      result = bignum_mod(result,p);
      bignum_free(tmp);
    }

    bignum_shift_right_inp(exponent,1);
    
    tmp = num;
    num = bignum_multiply(num,num);
    bignum_free(tmp);
    
    tmp = num;
    num = bignum_mod(num,p);
    bignum_free(tmp);    
  }
  
  bignum_free(num);
  bignum_free(const_0);
  
  return result;
}
