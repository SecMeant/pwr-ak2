#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include "bignum_common.h"
#include "bcd_common.h"

ssize_t getline(char **, size_t *, FILE *);

void terminate(const char * error_msg, int error_code)
{
  printf("FATAL ERROR! %s\n", error_msg);
  exit(error_code);
}

int64_t bignum_parse_hex_nibble(char nibble)
{
  int64_t ret;

  if(nibble >= '0' && nibble <= '9')
  {
    ret = nibble - '0';
  }
  else if(nibble >= 'A' && nibble <= 'F')
  {
    ret = nibble - 'A' + 10;
  }
  else if(nibble >= 'a' && nibble <= 'f')
  {
    ret = nibble - 'a' + 10;
  }
  else
  {
    terminate("Hex parse error, unexpected symbol.", -10);
  }

  return ret;
}

void bignum_hex_atoi
(bignum b, const char *buf, int buf_size)
{
  const char *buf_reverse = buf + buf_size - 1;
  for(int ch_count = 0; ch_count < b.bignum_size; ch_count++)
  {
    for(int byte_offset = 0; byte_offset < 16; byte_offset++)
    {
      b.bignum[ch_count] |= bignum_parse_hex_nibble(*buf_reverse) << (byte_offset * 4);

      if(buf_reverse == buf)
        return;

      --buf_reverse;
    }
  }
}

void bcd_bignum_hex_atoi(uint8_t *bcd_bignum, const char *buf, int size)
{
  const char *buf_reverse = buf + size - 1;
  for(int i = 0; i < size; ++i)
  {
    int digit = *buf_reverse - '0';
    --buf_reverse;
    bcd_bignum[i] = digit;
  }
}

static bool is_decimal_digit(char c)
{
  return (c >= '0' && c <= '9');
}

static bool is_hex_digit(char c)
{
  if(is_decimal_digit(c))
    return true;

  if(c >= 'A' && c <= 'F')
    return true;

  if(c >= 'a' && c <= 'f')
    return true;

  return false;
}

// returns true if buffer contains chars convertable to bignum
static bool bignum_verify_ascii(const char *buffer, size_t size)
{
  --size;
  while(size != 0)
  {
    if(!is_hex_digit(buffer[size])){
      printf("%i\n", (int)buffer[size]);
      return false;
    }

    --size;
  }
  return true;
}

static bool bcd_bignum_verify_ascii(const char *buffer, size_t size)
{
  --size;
  while(size != 0)
  {
    if(!is_decimal_digit(buffer[size]))
      return false;

    --size;
  }
  return true;
}

bignum bignum_load(FILE *file)
{
  assert(file);

  char *number_ascii = NULL;
  size_t number_size = 0;
  bignum ret = {NULL, 0};

  number_size = getline(&number_ascii, &number_size, file) - 1;
  if(!bignum_verify_ascii(number_ascii, number_size))
  {
    puts("NOT GOOD NUMBER");
    printf("%s\n", number_ascii);
    goto bignum_parse_cleanup;
  }

  ret = bignum_make(((number_size-1) / 16) + 1);
  bignum_hex_atoi(ret, number_ascii, number_size);

  bignum_parse_cleanup:
  free(number_ascii);
  return ret;
}

bcd_bignum bcd_bignum_load(FILE *file)
{
  assert(file);

  char *number_ascii = NULL;
  size_t number_size = 0;
  bcd_bignum ret = {NULL, 0};

  number_size = getline(&number_ascii, &number_size, file) - 1;
  if(!bcd_bignum_verify_ascii(number_ascii, number_size))
  {
    puts("NOT GOOD NUMBER");
    goto bcd_bignum_parse_cleanup;
  }

  ret = bcd_bignum_make(number_size);
  bcd_bignum_hex_atoi(ret.bignum, number_ascii, number_size);

  bcd_bignum_parse_cleanup:
  free(number_ascii);
  return ret;
}
