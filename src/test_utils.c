#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#define CHUNK_SIZE 8
#define NIBBLE_SHIFT 4

static void terminate(const char * error_msg, int error_code)
{
	printf("FATAL ERROR! %s\n", error_msg);
	exit(error_code);
}

static void bignum_zero(int64_t *bignum, int chunk_count)
{
	assert(bignum);

	while(chunk_count > 0)
	{
		--chunk_count;
		*(bignum+chunk_count) = 0ll;
	}
}

static int64_t bignum_parse_hex_nibble(char nibble)
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
	else
	{
		putchar(nibble);
		terminate("Hex parse error, unexpected symbol.", -10);
	}

	return ret;
}

static void bignum_hex_atoi(int64_t *bignum, const char *buf, int chunk_count)
{
	bignum_zero(bignum, chunk_count);
	for(int j = chunk_count-1; j >= 0; --j)
	{
		for(int i = 15; i >= 0; --i)
		{
			bignum[j] |= bignum_parse_hex_nibble(buf[15-i]) << (4*i);
		}
	}
}

// size argument describes how big in chunks of 64 bignum is
int64_t* bignum_load(int fd, int size)
{
	// size for char buffer
	const int64_t buf_size = size * 16;

	// alloc char buffer
	char *buf = (char*) malloc(buf_size);

	// alloc bignum
	int64_t *bignum = (int64_t*) malloc(size*sizeof(int64_t));

	// read bignum
	int bytes_read = read(fd, buf, buf_size);
	
	// if read less than expected (input not zero padded)
	// than pad it
	if(bytes_read != buf_size)
	{
		const int shift = buf_size-bytes_read+1; // +1 to shift endline
		memmove(buf+shift, buf, bytes_read);
		memset(buf,'0', shift);
	}

	bignum_hex_atoi(bignum, buf, size);

	free(buf);
	return bignum;
}

int main(void)
{
	int64_t* num = bignum_load(1, 1);
	printf("Got: 0x%016lx\n", *num);

	free(num);
}
