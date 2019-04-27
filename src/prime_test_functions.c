#include "bignum_common.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
bool trial_test(bignum num){
   bignum i;
   bignum_divide_result res;

   int64_t counter_array[BIGNUM_COMMON_SIZE] = {0x2};

   i.bignum_size = BIGNUM_COMMON_SIZE;
   i.bignum = counter_array;
   bignum i_2 = bignum_make(BIGNUM_COMMON_SIZE);
   int64_t i_2_array[BIGNUM_COMMON_SIZE];

   i_2.bignum = i_2_array;
   while( true ){

     bignum_multiply_inaa(i,i, i_2);

     res = bignum_divide(num,i);

     if( bignum_is_zero( res.reminder ) ){
       bignum_free( res.result );
       bignum_free( res.reminder );
       return false;
     }

     bignum_free( res.result );
     bignum_free( res.reminder );
     if( bignum_greater_than(i_2, num) ){
       return true;
     }

     bignum_increment_inp(i);
   }
}

bool fermat_primality_test(bignum p, size_t probes){
  // fermat test requires to make 
  bignum a = bignum_make(1);
  bignum const_1 = bignum_make(1);
  bignum p_copy = bignum_make(p.bignum_size);
  bignum res;
  int64_t *num_tmp;
  const_1.bignum[0] = 0x1;
  
  srand(time(0));

  bignum_copy(p_copy, p);
  bignum_sub_inp(p,const_1);

  for(size_t i = 0; i< probes; i++){
    a.bignum[0] = (int64_t)(rand() + 2) + (int64_t)(rand() + 2);  
    
    num_tmp= a.bignum; 
    a = bignum_gcd(p, a);
    free(num_tmp);
    const_1.bignum[0] = 1;
    if( !bignums_are_equal(const_1, a)){
      free(a.bignum);
      free(const_1.bignum);    
      return false;
    }

    res = bignum_power_mod_2(a, p, p_copy);
    const_1.bignum[0] = 1;
    if( !bignums_are_equal(const_1, res)){
      free(p_copy.bignum);
      free(const_1.bignum);
      free(res.bignum);
      return false;
    }
    free(res.bignum);
  }

  free(p_copy.bignum);
  free(const_1.bignum);
  return true;
}


bool bignums_are_equal(bignum lhs, bignum rhs){

  bignum_sub_inp(lhs,rhs);

  for(int64_t i =0 ; i < lhs.bignum_size; i++)
    if(lhs.bignum[i] != 0)
      return false;

  return true;
}

bool bignum_less_than(bignum lhs, bignum rhs){

  bignum_sub_inp(lhs,rhs);

  return bignum_is_negative(lhs);
}

bool bignum_greater_than(bignum lhs, bignum rhs){
 
  bignum_sub_inp(lhs,rhs);

  return !bignum_is_negative(lhs);
}
