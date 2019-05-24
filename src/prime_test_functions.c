#include "bignum_common.h"
#include <time.h>
#include <math.h>
bool trial_test(bignum num){
   bignum_divide_result res;

   bignum i = bignum_make(sqrt(num.bignum_size)+1);
   bignum i_2 = bignum_make(sqrt(num.bignum_size)+1);
   i.bignum[0] = 0x2;

   while( true ){

     bignum_multiply_inaa(i, i, i_2);

     // res = bignum_schoolbook_divide(num,i);
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
  bignum a;
  bignum b;
  bignum const_1 = bignum_make(1);
  bignum p_copy = bignum_make(p.bignum_size);
  bignum res;
  const_1.bignum[0] = 0x1;

  bignum_copy(p_copy, p);
  bignum_sub_inp(p_copy, const_1);

  for(size_t i = 0; i< probes; i++){
    do{
      a = bignum_rand(p.bignum_size, p);
      a.bignum[0] += 2;
    }while( bignums_are_equal(a, p) );

    b = bignum_gcd(p, a);
    
    if( !bignums_are_equal(const_1, b)){
      bignum_free(a);
      bignum_free(p_copy);
      bignum_free(const_1);
      bignum_free(b);
      return false;
    }

    bignum_free(b);
    res = bignum_power_mod_bexp(a, p, p_copy);
    // bignum_print(res);
    if(!bignums_are_equal(const_1, res)){
      bignum_free(a);
      bignum_free(p_copy);
      bignum_free(const_1);
      bignum_free(res);
      return false;
    }

    bignum_free(a);
    bignum_free(res);
  }

  bignum_free(p_copy);
  bignum_free(const_1);
  
  return true;
}

bool bignums_are_equal(bignum lhs, bignum rhs){
  bignum longer, shorter;  
  if(lhs.bignum_size > rhs.bignum_size){
    longer = lhs;
    shorter = rhs;
  }else{
    longer = rhs;
    shorter = lhs;
  }
  int64_t i;

  for( i = longer.bignum_size - 1; i > shorter.bignum_size; i--)
    if(longer.bignum[i] != 0)
      return false;

  for( ; i >=0 ; i--)
    if( longer.bignum[i] != shorter.bignum[i] )
      return false;

  return true;
}


bool bignum_less_than(bignum lhs, bignum rhs)
{
  int64_t lhs_eff_width = bignum_effective_width(lhs);
  int64_t rhs_eff_width = bignum_effective_width(rhs);

  if(lhs_eff_width != rhs_eff_width)
  {
    return lhs_eff_width < rhs_eff_width;
  }

  for(int64_t i = bignum_bit_size_to_chunks(lhs_eff_width)-1 ; i >= 0; i--)
  {
    if( lhs.bignum[i] == rhs.bignum[i] )
      continue;

    if( (uint64_t) lhs.bignum[i] < (uint64_t) rhs.bignum[i] )
      return true;

    return false;
  }

  return false;
}

bool bignum_greater_than(bignum lhs, bignum rhs)
{
  int64_t lhs_eff_width = bignum_effective_width(lhs);
  int64_t rhs_eff_width = bignum_effective_width(rhs);

  if(lhs_eff_width != rhs_eff_width)
  {
    return lhs_eff_width > rhs_eff_width;
  }

  for(int64_t i = bignum_bit_size_to_chunks(lhs_eff_width)-1 ; i >= 0; i--)
  {
    if( lhs.bignum[i] == rhs.bignum[i] )
      continue;

    if( (uint64_t) lhs.bignum[i] > (uint64_t) rhs.bignum[i] )
      return true;

    return false;
  }
  return false;
}
