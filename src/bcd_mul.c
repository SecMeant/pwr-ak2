#include "bcd_common.h"

bcd_bignum bcd_bignum_mul(bcd_bignum b1, bcd_bignum b2){
   
  int64_t max_size, min_size;
  uint8_t *bigger_num, *shorter_num;
  // this variables hold result of spliting the multiplication process
  int8_t reminder, quotient;
  int8_t carry = 0, mul_res;

  if(b1.bignum_size < b2.bignum_size){
    max_size = b2.bignum_size;
    min_size = b1.bignum_size;
    bigger_num = b2.bignum;
    shorter_num = b1.bignum;
  }else{
    max_size = b1.bignum_size;
    min_size = b2.bignum_size;
    bigger_num = b1.bignum;
    shorter_num = b2.bignum;
  }

  // allocate space for result which can take up to length of two factors sum 
  bcd_bignum result = bcd_bignum_make( max_size + min_size );
  // reset result
  memset(result.bignum, 0 , result.bignum_size);
  
  int64_t i,j;
  // make multiplication
  for( i = 0; i < max_size; i++){
    for( j = 0; j < min_size; j++){
      mul_res = bigger_num[i]*shorter_num[j]; 

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
    result.bignum[ i+ j] += carry;
    carry=0; 
  }
  return result;
}