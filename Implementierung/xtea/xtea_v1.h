#ifndef XTEA_V1_H_   /* Include guard */
#define XTEA_V1_H_
#include <stdint.h>

void xtea_encrypt_block_v1(uint32_t [2] blocks, const uint32_t [4] keys);
void xtea_decrypt_block_v1(uint32_t [2] blocks, const uint32_t [4] keys);
void xtea_encrypt_block_v2(const unsigned long [64] sums, uint32_t [2] blocks, const uint32_t [4] keys);
void xtea_decrypt_block_v2(const unsigned long [64] sums, uint32_t [2] blocks, const uint32_t [4] keys);

#endif