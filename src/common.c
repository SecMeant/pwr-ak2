#include "common.h"
#include <stdio.h>

void bignum_fatal_error(const char *msg, int64_t errno)
{
	fprintf(stderr, "BIGNUM FATAL ERROR! %s\n", msg);
	exit(errno);
}

void bignum_print(bignum b)
{
	int64_t index = b.bignum_size-1;
	while(index >= 0)
	{
		printf("0x%016lx ", b.bignum[index]);
		--index;
	}
	putchar('\n');
}

bool bignum_is_zero(bignum b1)
{
	int64_t index = b1.bignum_size - 1;
	while(index >= 0)
	{
		if(b1.bignum[index] != 0)
			return false;
		--index;
	}
	return true;
}

bignum bignum_extend_twice(bignum b1)
{
	bignum ret;
	ret.bignum_size = b1.bignum_size * CHUNK_SIZE * 2;
	ret.bignum = (int64_t*)malloc(ret.bignum_size);
	bignum_copy(ret, b1);
	return ret;
}

void bignum_or_1(bignum b1)
{
	if(b1.bignum_size == 0)
		return;
	b1.bignum[0] |= 1;
}

void bignum_free(bignum b)
{
	free(b.bignum);
	b.bignum_size = 0;
}

void bignum_alloc(bignum *b1, int64_t size)
{
	b1->bignum = malloc(size * CHUNK_SIZE);
	b1->bignum_size = size;
}

bignum bignum_make(int64_t size)
{
	bignum ret;
	ret.bignum = (int64_t*) malloc(size * CHUNK_SIZE);
	ret.bignum_size = size;
	assert(ret.bignum);
	return ret;
}
