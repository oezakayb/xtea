#ifndef XTEA_V3_H_   /* Include guard */
#define XTEA_V3_H_
#include <stdint.h>

extern void xtea_encrypt_block_v3(unsigned long sums [65], uint32_t blocks [2], const uint32_t keys [4]);
extern void xtea_decrypt_block_v3(unsigned long sums [65], uint32_t blocks [2], const uint32_t keys [4]);

#endif