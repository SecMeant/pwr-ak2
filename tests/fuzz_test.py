#!/usr/bin/python3
import os
import subprocess
import sys
from random import randrange

success = 0

try:
  number_generator = os.environ['crypt_num_gen']
except:
  print('ERROR! Set crypt_num_gen to number generator that should be used.')
  exit(-1)

# length is tuple of sizes of bignums
def generate_pair_op(output_file_name : str, op : str, lengths : tuple, base : int = 16):
  global number_generator
  avail_digits = '0123456789'

  if base == 16:
    avail_digits += 'abcdef'

  with open(output_file_name, 'w') as output_file:
    subprocess.call([number_generator, avail_digits, str(lengths[0])], stdout=output_file)
    subprocess.call([number_generator, avail_digits, str(lengths[1])], stdout=output_file)
    output_file.write('{}\n\n'.format(op))

def bignum_test(input_file_name : str, base : int = 16) -> bool:
  with open(input_file_name) as input_file:
    b1 = int(input_file.readline(), base)
    b2 = int(input_file.readline(), base)
    op = input_file.readline().strip()

    if op == 'add':
      res = (b1 + b2,)
    elif op == 'sub':
      res = (b1 - b2,)
    elif op == 'mul':
      res = (b1 * b2,)
    elif op == 'div':
      res = (b1 // b2, b1 % b2)

    crypt = subprocess.Popen(['/home/holz/etc/github/pwr-ak2/crypt', os.path.abspath(input_file_name)], stdout=subprocess.PIPE)
  crypt.wait()
  for i in range(0, res.__len__()):
    crypt_result = crypt.stdout.readline()
    crypt_result = int(crypt_result, 16)
    if res[i] != crypt_result:
      print("Test Failed\r")
      print("Result: ", hex(res[i]))
      print("Crypt: ", hex(crypt_result))
      return False

  global success
  success += 1
  sys.stdout.write("Success rate: {}\r".format(success));
  return True


if __name__ == '__main__':
  file_name_dat = '/home/holz/etc/github/pwr-ak2/bignums.txt'

  test_ok = True
  while(test_ok):
    first_arg_len = randrange(234, 360)
    sec_arg_len = randrange(45, 78)
    generate_pair_op(file_name_dat, 'div', (first_arg_len, sec_arg_len))
    test_ok = bignum_test(file_name_dat)
