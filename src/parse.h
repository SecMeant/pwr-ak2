#pragma once

void terminate(const char * error_msg, int error_code);
int64_t bignum_parse_hex_nibble(char nibble);
void bignum_hex_atoi(int64_t *bignum, const char *buf, int chunk_count);
void bcd_bignum_hex_atoi(uint8_t *bcd_bignum, const char *buf, int size);
bignum bignum_load(FILE *file);
bcd_bignum bcd_bignum_load(FILE *file);
