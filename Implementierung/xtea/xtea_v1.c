#include "xtea_v1.h"

void xtea_encrypt_block_v1(uint32_t [2] blocks, const uint32_t [4] keys){

    uint32_t sigma = 0x9E3779B9;
    uint32_t v0 = blocks[0];
    uint32_t v1 = blocks[1];
    unsigned long sum = 0;

    for (int i = 0; i < 64; ++i) {
        v0 += ((((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + keys[sum & 3]));
        sum += sigma;
        v1 += ((((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + keys[(sum >> 11) & 3]));
    }

    blocks[0] = v0;
    blocks[1] = v1;

}

void xtea_decrypt_block_v1(uint32_t [2] blocks, const uint32_t [4] keys){

    uint32_t sigma = 0x9E3779B9;
    uint32_t v0 = blocks[0];
    uint32_t v1 = blocks[1];
    unsigned long sum = 0x278DDE6E40;

    for (int i = 0; i < 64; ++i) {
        v1 -= ((((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + keys[(sum >> 11) & 3]));
        sum -= sigma;
        v0 -= ((((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + keys[sum & 3]));
    }

    blocks[0] = v0;
    blocks[1] = v1;
}