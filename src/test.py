
def isPrime(num):
  i = 2
  while(True):
    i_2 = i*i

    if(num % i == 0):
      return i_2

    if(i_2 >= num):
      return i_2
    i+=1

  


k = isPrime( 2305843009213693951 )

print(k)