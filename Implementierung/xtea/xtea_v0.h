#ifndef XTEA_V0_H_   /* Include guard */
#define XTEA_V0_H_
#include <stdint.h>

extern void xtea_encrypt_block(uint32_t blocks [2], const uint32_t keys [4]);
extern void xtea_decrypt_block(uint32_t blocks [2], const uint32_t keys [4]);

#endif