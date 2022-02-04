#ifndef XTEA_V1_H_   /* Include guard */
#define XTEA_V1_H_
#include <stdint.h>

void xtea_encrypt_block_v1(uint32_t blocks [2], const uint32_t keys [4]);
void xtea_decrypt_block_v1(uint32_t blocks [2], const uint32_t keys [4]);
void xtea_encrypt_block_v2(unsigned long sums [64], uint32_t blocks [2], const uint32_t keys [4]);
void xtea_decrypt_block_v2(unsigned long sums [64], uint32_t blocks [2], const uint32_t keys [4]);

#endif