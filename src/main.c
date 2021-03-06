#include <stdio.h>
#include <stdint.h>
#include "bignum_common.h"
#include "bcd_common.h"
#include "parse.h"

void perform_test(void);

void shift_test()
{
  int64_t num1[] = {0,0,3,0};
  bignum bignum_sh = {.bignum = num1, .bignum_size = 4};
  bignum_print(bignum_sh);
  bignum_right_64_inp(bignum_sh, 1);
  bignum_print(bignum_sh);
}

void add_test()
{
  int64_t num1[2] = {0x8000000000000000 ,0x0000000200000000};
  int64_t num2[2] = {0x8000000000000000 ,0x0003800000000000};
  bignum b1 = {.bignum = num1, .bignum_size = 2};
  bignum b2 = {.bignum = num2, .bignum_size = 2};
  printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
  printf("0x%016lx 0x%016lx\n\n", *(num2+1), *num2);
  bignum_add_inp(b1, b2);
  printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
  printf("0x%016lx 0x%016lx\n", *(num2+1), *num2);
}

void sub_test()
{
  int64_t num1[2] = {0xfffffffffffffffe ,0x7fffffffffffffff};
  int64_t num2[2] = {0x5454545454545454 ,0x3434343434343433};
  bignum b1 = {.bignum = num1, .bignum_size = 2};
  bignum b2 = {.bignum = num2, .bignum_size = 2};
  printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
  printf("0x%016lx 0x%016lx\n\n", *(num2+1), *num2);
  bignum_sub_inp(b1, b2);
  printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
  printf("0x%016lx 0x%016lx\n", *(num2+1), *num2);
  printf("Result negative: %li\n", (int64_t)bignum_is_negative(b1));
}

void copy_test()
{
  int64_t num1[2] = {0xfffffffffffffffe ,0x7fffffffffffffff};
  int64_t num2[2] = {0x5454545454545454 ,0x3434343434343433};
  bignum b1 = {.bignum = num1, .bignum_size = 2};
  bignum b2 = {.bignum = num2, .bignum_size = 2};
  printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
  printf("0x%016lx 0x%016lx\n\n", *(num2+1), *num2);
  bignum_copy(b1, b2);
  printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
  printf("0x%016lx 0x%016lx\n\n", *(num2+1), *num2);
  // Zero extend copy test
  b2.bignum_size = 1;
  bignum_copy(b1, b2);
  printf("0x%016lx 0x%016lx\n", *(num1+1), *num1);
  printf("0x%016lx 0x%016lx\n\n", *(num2+1), *num2);
}
void multiply_test(){

  bignum first, second;
  first.bignum_size = 4;
  first.bignum = (int64_t*)malloc(sizeof(uint64_t)*first.bignum_size);
  first.bignum[0] = 0x100000000;
  first.bignum[1] = 0x0;
  first.bignum[2] = 0x0;
  first.bignum[3] = 0x0;

  second.bignum_size = 4;
  second.bignum = (int64_t*)malloc(sizeof(uint64_t)*second.bignum_size);
  second.bignum[0] = 0x2;
  second.bignum[1] = 0x2;
  second.bignum[2] = 0x2;
  second.bignum[3] = 0x2;

  bignum res;
  res.bignum_size = 4;
  res.bignum = (int64_t*)malloc(sizeof(uint64_t)*res.bignum_size);
  res.bignum[0] = 0x0;
  res.bignum[1] = 0x0;
  res.bignum[2] = 0x0;
  res.bignum[3] = 0x0;
  // bignum_multiply_inaa(first,first,res);
  res = bignum_multiply_fixed(first,first);
  for(int i =0; i < 4; i++)
    printf( "%lx\n", (uint64_t)res.bignum[i] );

  free (first.bignum);
  free (second.bignum);
  free (res.bignum);

}

void power_test(){
  bignum b1 = bignum_load(stdin);
  bignum res = bignum_power(b1,12);
  bignum_print(res);
  free(res.bignum);

}

void power_test_mod(){
  bignum b1 = bignum_load(stdin);
  bignum b2 = bignum_load(stdin);

  bignum res = bignum_power_mod(b1,b2,123);
  bignum_print(res);
  free(b2.bignum);
  free(res.bignum);

}
void power_test_mod_2(){
  bignum b1 = bignum_load(stdin);
  bignum b2 = bignum_load(stdin);
  for(int i =0; i < 100; i++){
    bignum res = bignum_power_mod_bexp(b1,b2,b1);
    bignum_print(res);
    free(res.bignum);
  }

  free(b1.bignum);
  free(b2.bignum);
}

void bignum_increment_inp_test(){
  bignum first;
  first.bignum_size = 3;
  first.bignum = (int64_t*)malloc(sizeof(uint64_t)*first.bignum_size);
  first.bignum[0] = 0x0;
  first.bignum[1] = 0x0;
  first.bignum[2] = 0x0;
  uint64_t k =0;
  for (uint64_t i = 0; i < 0xfffffffffffffffe; ++i)
  {
    bignum_increment_inp(first);
    if(i == 0xffffffff*k){
      bignum_print(first);
      k++;
    }
  }
  for (uint64_t i = 0; i < 0xfffffffffffffffe; ++i)
  {
    bignum_increment_inp(first);
    bignum_print(first);
  }
  free(first.bignum);
}

void isPrime(){
  bignum b1 = bignum_load(stdin);

  bool prime = trial_test(b1);

  if( prime )
    printf("Number is prime\n");
  else
    printf("Number is not prime\n");

  bignum_free(b1);
}

void isPrime_fermat(){
    
  bignum b1 = bignum_load(stdin);

  bool prime = fermat_primality_test(b1, 10);

  bignum_free(b1);

  if( prime )
    printf("Number is prime\n");
  else
    printf("Number is not prime\n");

}

void isPrime_rm(){
    
  bignum b1 = bignum_load(stdin);

  bool prime = rabin_miller_test(b1, 10);

  bignum_free(b1);

  if( prime )
    printf("Number is prime\n");
  else
    printf("Number is not prime\n");

}


void div_test()
{
  FILE *f = fopen("bignums.txt", "rb");

  if(!f)
  {
    fprintf(stderr, "Could not open bignums.txt file.");
    return;
  }

  bignum b1 = bignum_load(f);
  bignum b2 = bignum_load(f);
  bignum_divide_result res = bignum_schoolbook_divide(b1,b2);
  bignum_print(res.result);
  bignum_print(res.reminder);
  bignum_free(res.reminder);
  bignum_free(res.result);
  bignum_free(b1);
  bignum_free(b2);
  fclose(f);
}

void bcd_div_test2()
{
  FILE *f = fopen("bignums.txt", "rb");
  bcd_bignum b1 = bcd_bignum_load(f);
  bcd_bignum b2 = bcd_bignum_load(f);
  bcd_bignum_print(b1);
  bcd_bignum_print(b2);
  bcd_bignum_divide_result res = bcd_bignum_divide(b1,b2);
  bcd_bignum_print(res.result);
  bcd_bignum_print(res.reminder);
  bcd_bignum_free(res.reminder);
  bcd_bignum_free(res.result);
  bcd_bignum_free(b1);
  bcd_bignum_free(b2);
  fclose(f);
}

void bcd_add_test()
{
  bcd_bignum b1 = bcd_bignum_make(6);
  bcd_bignum b2 = bcd_bignum_make(9);
  uint8_t num1_static[] = {9,9,9,9,9,9};
  uint8_t num2_static[] = {9,9,9,9,9,9,9,9,0};

  memcpy(b1.bignum, num1_static, b1.bignum_size);
  memcpy(b2.bignum, num2_static, b2.bignum_size);

  for(int i =0; i < 8; i++){
    printf("%x",b1.bignum[i] );
  }
  puts("");
  bcd_bignum_add_inp(b1, b2);

  for(int i =0; i < 9; i++){
    printf("%x",b1.bignum[i] );
  }
}


void bcd_sub_test()
{
  bcd_bignum b1 = bcd_bignum_make(9);
  bcd_bignum b2 = bcd_bignum_make(6);
  uint8_t num1_static[] = {1,8,8,4,2,0};
  uint8_t num2_static[] = {6,9,7,5,3,1,0,0,0};

  memcpy(b1.bignum, num2_static, b1.bignum_size);
  memcpy(b2.bignum, num1_static, b2.bignum_size);

  for(int i =0; i < 8; i++){
    printf("%x",b1.bignum[i] );
  }
  puts("");
  bcd_bignum_sub_inp(b1, b2);

  for(int i =0; i < 9; i++){
    printf("%x",b1.bignum[i] );
  }
}


void bcd_mul_test()
{
  bcd_bignum b1 = bcd_bignum_make(9);
  bcd_bignum b2 = bcd_bignum_make(6);
  uint8_t num1_static[] = {8,9,9,9,9,9};
  uint8_t num2_static[] = {7,9,9,9,9,9,9,9,9};

  memcpy(b1.bignum, num2_static, b1.bignum_size);
  memcpy(b2.bignum, num1_static, b2.bignum_size);

  for(int i =0; i < 9; i++){
    printf("%x",b1.bignum[i] );
  }
  puts("");

  for(int i =0; i < 6; i++){
    printf("%x",b2.bignum[i] );
  }
  puts("");

  bcd_bignum res =bcd_bignum_mul(b1, b2);

  for(int i =0; i < b1.bignum_size + b2.bignum_size; i++){
    printf("%x",res.bignum[i] );
  }
}

void bcd_mul_crop_test()
{
  bcd_bignum b1 = bcd_bignum_make(9);
  bcd_bignum b2 = bcd_bignum_make(6);
  uint8_t num1_static[] = {2,2,2,2,2,2};
  uint8_t num2_static[] = {2,2,2,2,2,2,2,2,2};

  memcpy(b1.bignum, num2_static, b1.bignum_size);
  memcpy(b2.bignum, num1_static, b2.bignum_size);

  for(int i =0; i < 9; i++){
    printf("%x",b1.bignum[i] );
  }
  puts("");

  for(int i =0; i < 6; i++){
    printf("%x",b2.bignum[i] );
  }
  puts("");

  bcd_bignum res = bcd_bignum_mul_fixed(b1, b2);

  for(int i = 0; i < b1.bignum_size; i++){
    printf("%x", res.bignum[i]);
  }
}


void bcd_power_test(){
  bcd_bignum first;
  first.bignum_size = BCD_COMMON_SIZE;
  first.bignum = (uint8_t*)malloc(sizeof(uint8_t)*first.bignum_size);
  memset(first.bignum, 0, BCD_COMMON_SIZE);
  first.bignum[0] = 0x2;

  bcd_bignum res = bcd_bignum_power(first,5000);

  for(int i =BCD_COMMON_SIZE-1; i >= 0; i--)
    printf( "%x", res.bignum[i]);
  free(first.bignum);
  free(res.bignum);
}


void bcd_div_test()
{
  FILE *f = fopen("bignums.txt", "rb");

  if(!f)
  {
    fprintf(stderr, "Could not open bignums.txt file.");
    return;
  }

  bcd_bignum b1 = bcd_bignum_load(f);
  bcd_bignum b2 = bcd_bignum_load(f);

  bcd_bignum_divide_result res = bcd_bignum_divide(b1, b2);
  bcd_bignum_print(res.result);
  bcd_bignum_print(res.reminder);

  bcd_bignum_free(b1);
  bcd_bignum_free(b2);
  bcd_bignum_free(res.result);
  bcd_bignum_free(res.reminder);
}

void bcd_isPrime(){
  uint8_t num1[BCD_COMMON_SIZE];
  memset(num1, 0, BCD_COMMON_SIZE);
  to_bcd_number(bcd_bignum_example, BIGNUM_BIG_ENDIAN, num1);

  for(int i = BCD_COMMON_SIZE-1; i >= 0; i--)
    printf( "%x", num1[i]);

  puts("");

  bcd_bignum b1 = {.bignum = num1, .bignum_size = BCD_COMMON_SIZE};

  bool prime = bcd_trial_test(b1);
  if( prime )
    printf("Number is prime\n");
  else
    printf("Number is not prime\n");
}

void precise_mul_test(){
  FILE *fptr = fopen("bignum_test", "rb");
  if(fptr == NULL){
    printf("Cant open file\n");
    return;
  }
  bignum a =bignum_load(fptr);
  bignum b =bignum_load(fptr);
  bignum_print(a);
  bignum_print(b);

  bignum res = bignum_multiply(a,b);
  
  bignum_print(res);
}

int fuzz_test(int argc, char **argv)
{
  if(argc < 2)
  {
    puts("Give me file");
    return -1;
  }

  FILE* f = fopen(argv[1], "rb");

  if(!f)
  {
    puts("Cant open file.");
    return -2;
  }

  bignum b1 = bignum_load(f);
  bignum b2 = bignum_load(f);

  bignum_add_inp(b1, b2);
  bignum_print(b1);
  bignum_free(b1);
  bignum_free(b2);

  fclose(f);
  return 0;
}

int main()
{
  perform_test();
}
