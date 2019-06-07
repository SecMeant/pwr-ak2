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

bool rabin_miller_test(bignum p, size_t n){
  bignum p_copy = bignum_make(p.bignum_size);
  bignum_copy(p_copy, p);
  bignum d =  bignum_make(p.bignum_size);
  bignum_copy(d, p);
  size_t s = 0;
  bignum x, a;

  bignum const_0 = bignum_make(1);
  bignum const_1 = bignum_make(1);
  bignum const_2 = bignum_make(1);
  bignum const_4 = bignum_make(1);

  const_1.bignum[0] = 0x1;
  const_2.bignum[0] = 0x2;
  const_4.bignum[0] = 0x4;

  bignum_sub_inp(d, const_1);
  bignum p_sub_1 = bignum_make(d.bignum_size); 
  bignum_copy(p_sub_1, d);
  size_t j;

  bignum_divide_result tmp = bignum_schoolbook_divide(d, const_2);
  while( bignums_are_equal(const_0, tmp.reminder) ){
    d = tmp.result;
    s++;
    tmp = bignum_schoolbook_divide(d, const_2);
  }
  // just for rand scope
  bignum_sub_inp(p_copy, const_4);

  for(size_t i =0; i < n ; i++){
    a = bignum_rand(p.bignum_size, p_copy);
    a.bignum[0] += 2;
    
    x = bignum_power_mod_bexp(a, p, d);
    if(bignums_are_equal(const_1, x) || bignums_are_equal(x, p_sub_1))
      continue;
    
    j = 1;
    while( j < s && !bignums_are_equal(x, p_sub_1) ){
      x = bignum_power_mod(x,p,2);
      if( bignums_are_equal(const_1, x)){
        bignum_free(const_0);
        bignum_free(const_1);
        bignum_free(const_2);
        bignum_free(const_4);
        bignum_free(p_copy);
        bignum_free(d);
        bignum_free(p_sub_1);
        return false;
      }
      j++;
    }
    if( !bignums_are_equal(x,p_sub_1) ){
        bignum_free(const_0);
        bignum_free(const_1);
        bignum_free(const_2);
        bignum_free(const_4);
        bignum_free(p_copy);
        bignum_free(d);
        bignum_free(p_sub_1);
      return false;
    }
  }
  bignum_free(const_0);
  bignum_free(const_1);
  bignum_free(const_2);
  bignum_free(const_4);
  bignum_free(p_copy);
  bignum_free(d);
  bignum_free(p_sub_1);  
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
