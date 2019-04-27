#include "bcd_common.h"

bcd_bignum bcd_bignum_mul(bcd_bignum b1, bcd_bignum b2){


  // this variables hold result of spliting the multiplication process
  int8_t reminder, quotient;
  int8_t carry = 0, mul_res;

  // allocate space for result which can take up to length of two factors sum
  bcd_bignum result = bcd_bignum_make( b1.bignum_size + b2.bignum_size );
  // reset result
  memset(result.bignum, 0 , result.bignum_size);

  int64_t i,j;
  // make multiplication
  for( i = 0; i < b1.bignum_size; i++){
    for( j = 0; j < b2.bignum_size; j++){
      mul_res = b1.bignum[i]*b2.bignum[j];

      // divide result by ten
      __asm__ volatile(
        "divq %%rcx;"
        : "=a"(quotient), "=d"(reminder)
        : "a"(mul_res), "d"(0), "c"(10)
        :
      );
      // add reminder to result
      result.bignum[i+j] += reminder + carry;
      carry = quotient;
      // check if addition caused overflow
      if(result.bignum[i+j] > 9){
         result.bignum[i+j] = (result.bignum[i+j]+6) & BCD_U8_MASK;
         carry++;
      }
    }
    // prepeand carry to result
    result.bignum[ i+ j] += carry;
    carry=0;
  }
  return result;
}


bcd_bignum bcd_bignum_mul_fixed(bcd_bignum b1, bcd_bignum b2){

  // this variables hold result of spliting the multiplication process
  int8_t reminder, quotient;
  int8_t carry = 0, mul_res;
  int64_t i,j;

  // allocate space for result which can take up to length of two factors sum
  bcd_bignum result = bcd_bignum_make( b1.bignum_size );

  // expand first loop to init result

  for( i = 0; i < b1.bignum_size; i++){
    mul_res = b1.bignum[i]*b2.bignum[0];
    // divide result by ten
    __asm__ volatile(
      "divq %%rcx;"
      : "=a"(quotient), "=d"(reminder)
      : "a"(mul_res), "d"(0), "c"(10)
      :
    );
    // add reminder to result
    result.bignum[i] =  reminder + carry;
    carry = quotient;
    // check if addition caused overflow
    if(result.bignum[i] > 9){
       result.bignum[i] = (result.bignum[i]+6) & BCD_U8_MASK;
       carry++;
    }
  }
  // prepeand carry to result
  result.bignum[i] += carry;
  carry=0;

  b1.bignum_size--;

  // make multiplication
  for( j = 1; j < b2.bignum_size; j++){
    for( i = 0; i < b1.bignum_size; i++){
      mul_res = b1.bignum[i]*b2.bignum[j];
      // divide result by ten
      __asm__ volatile(
        "divq %%rcx;"
        : "=a"(quotient), "=d"(reminder)
        : "a"(mul_res), "d"(0), "c"(10)
        :
      );
      // add reminder to result
      result.bignum[i+j] +=  reminder + carry;
      carry = quotient;
      // check if addition caused overflow
      if(result.bignum[i+j] > 9){
         result.bignum[i+j] = (result.bignum[i+j]+6) & BCD_U8_MASK;
         carry++;
      }
    }
    // prepeand carry to result
    result.bignum[i + j] += carry;
    carry=0;

    b1.bignum_size--;
  }
  return result;
}


void bcd_bignum_mul_inaa(bcd_bignum b1, bcd_bignum b2, bcd_bignum result){

  // this variables hold result of spliting the multiplication process
  int8_t reminder, quotient;
  int8_t carry = 0, mul_res;
  int64_t i,j;
  int64_t shorter_size = result.bignum_size <  b1.bignum_size ? result.bignum_size : b1.bignum_size;
  // expand first loop to overwrite last result
  for( i = 0; i < shorter_size; i++){
      mul_res = b1.bignum[i]*b2.bignum[0];
      // divide result by ten
      __asm__ volatile(
        "divq %%rcx;"
        : "=a"(quotient), "=d"(reminder)
        : "a"(mul_res), "d"(0), "c"(10)
        :
      );
      // add reminder to result
      result.bignum[i] =  reminder + carry;
      carry = quotient;
      // check if addition caused overflow
      if(result.bignum[i] > 9){
         result.bignum[i] = (result.bignum[i]+6) & BCD_U8_MASK;
         carry++;
      }
    }
    // prepeand carry to result
    result.bignum[i] += carry;
    carry=0;

    shorter_size--;


  // make multiplication
  for( j = 1; j < b2.bignum_size; j++){
    for( i = 0; i < shorter_size; i++){
      mul_res = b1.bignum[i]*b2.bignum[j];
      // divide result by ten
      __asm__ volatile(
        "divq %%rcx;"
        : "=a"(quotient), "=d"(reminder)
        : "a"(mul_res), "d"(0), "c"(10)
        :
      );
      // add reminder to result
      result.bignum[i+j] +=  reminder + carry;
      carry = quotient;
      // check if addition caused overflow
      if(result.bignum[i+j] > 9){
         result.bignum[i+j] = (result.bignum[i+j]+6) & BCD_U8_MASK;
         carry++;
      }
    }
    // prepeand carry to result
    result.bignum[i + j] += carry;
    carry=0;

    shorter_size--;
  }
}

