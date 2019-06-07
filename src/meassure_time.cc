#include <fstream>
#include <random>
#include <chrono>
#include <cstring>
#include <iostream>

extern "C"
{
  #include "bignum_common.h"
  #include "parse.h"
  void perform_test(void); 
}

void perform_test(){
  // chrono measure variables
  using namespace std;
  using namespace std::chrono;
  high_resolution_clock::time_point t1;
  high_resolution_clock::time_point t2;
  uint64_t results[3];
  bool is_prime_res[3];

  const char *header = "Nazwa;Czy pierwsza;Liczba bitow;Czas";
  const char *format_str = "%s;%d;%d;%lu%\n";
  const char *fun_name_table[] = {"Trial divison", "Fermat primality test", "Miller-Rabin test"};

  char buff[1000];
  std::fstream fs ("wyniki_prime", std::fstream::out);
  srand(time(0));
  bignum b1;
  bignum b_zero = bignum_make(1);

  FILE *in_file = fopen("primes", "r");
  if(in_file == NULL)
    return;
  
  if( !fs.is_open() )
    return;

  fs << header;

  while(true){
    b1 = bignum_load(in_file);

    if(b1.bignum ==  NULL){
      fclose(in_file);
      fs.close();
      return;
    }

    std::memset(results, 0, sizeof results);
    
    for(int i =0 ; i < 100; i++){
      std::cout << "Trial divison"<<std::endl;
      t1 = high_resolution_clock::now();  
      is_prime_res[0] = trial_test(b1);      
      t2 = high_resolution_clock::now();
      results[0] += chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();

      std::cout << "Fermat primality test"<<std::endl;
      t1 = high_resolution_clock::now();            
      is_prime_res[1] = fermat_primality_test(b1,20);
      t2 = high_resolution_clock::now();
      results[1] += chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();
        
      std::cout << "Miller-Rabin test"<<std::endl;
      t1 = high_resolution_clock::now();            
      is_prime_res[2] = rabin_miller_test(b1,20);   
      t2 = high_resolution_clock::now();
      results[2] += chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();  
    }

    auto efective_width = bignum_effective_width(b1);
    for(int i =0; i< 3; i++){
      sprintf(buff, format_str, fun_name_table[i], is_prime_res[i], efective_width, results[i]/100);
      fs << buff;
    }
  }
}
