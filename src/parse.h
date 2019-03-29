#pragma once

void terminate(const char * error_msg, int error_code);
int64_t bignum_parse_hex_nibble(char nibble);
void bignum_hex_atoi(int64_t *bignum, const char *buf,
                     int chunk_count);
bignum bignum_load(FILE *file);
