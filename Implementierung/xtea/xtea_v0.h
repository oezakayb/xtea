#ifndef XTEA_V0_H_   /* Include guard */
#define XTEA_V0_H_
#include <stdint.h>

extern void xtea_encrypt_block(uint32_t [2] blocks, const uint32_t [4] keys);
extern void xtea_decrypt_block(uint32_t [2] blocks, const uint32_t [4] keys);

#endif