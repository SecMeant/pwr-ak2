#include "bignum_common.h"

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

     bignum_multiply_fixed_no_malloc(i,i, i_2);

     res = bignum_divide(num,i);

     if( bignum_is_zero( res.reminder ) ){
       bignum_free( res.result );
        bignum_free( res.reminder );
       return false;
     }

    bignum_free( res.result );
    bignum_free( res.reminder );
    bignum_print(i_2);
     if( bignum_greater_than(i_2, num) ){
       return true;
     }

     bignum_increment(i);
   }
}

bool fermat_primality_test(bignum num, size_t probes){

  for(size_t i = 0; i< probes; i++){
    
  }
}


bool bignums_are_equal(bignum lhs, bignum rhs){

  bignum_subtract(lhs,rhs);

  for(int64_t i =0 ; i < lhs.bignum_size; i++)
    if(lhs.bignum[i] != 0)
      return false;

  return true;
}

bool bignum_less_than(bignum lhs, bignum rhs){

  bignum_subtract(lhs,rhs);
  // bignum_print(lhs);
  for(int64_t i =lhs.bignum_size-1 ; i > 0; i++){
    if(lhs.bignum[i] < 0)
      return true;
    else if(lhs.bignum[i] != 0)
      return false;
  }
  return false;
}

bool bignum_greater_than(bignum lhs, bignum rhs){
  // int64_t sub_result;
  // for(int64_t i =lhs.bignum_size-1 ; i > 0; i++){
  //   sub_result =lhs.bignum[i] - rhs.bignum[i];
  //   if( sub_result > 0 )
  //     return true;
  //   else if (sub_result < 0)
  //     return false;
  // }
  bignum_subtract(lhs,rhs);


  return !bignum_is_negative(lhs);
}
