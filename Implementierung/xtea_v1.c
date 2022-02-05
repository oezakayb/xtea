#include "xtea_v1.h"

void xtea_encrypt_block_v1(uint32_t blocks [2], const uint32_t keys [4]){

    uint32_t sigma = 0x9E3779B9;
    uint32_t v0 = blocks[0];
    uint32_t v1 = blocks[1];
    unsigned long sum = 0;

    for (int i = 0; i < 1; ++i) {
        uint32_t vtemp1 = (v1 << 4);
        printf("%X\n", vtemp1);
        uint32_t vtemp2 = (v1 >> 5);
        printf("%X\n", vtemp2);
        vtemp1 = (vtemp1 ^ vtemp2);
        printf("%X\n", vtemp1);
        vtemp1 += v1;
        printf("%X\n", vtemp1);
        v0 += (vtemp1 ^ (sum + keys[sum & 3]));
        sum += sigma;
        v1 += ((((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + keys[(sum >> 11) & 3]));
    }

    blocks[0] = v0;
    blocks[1] = v1;

}

void xtea_decrypt_block_v1(uint32_t blocks [2], const uint32_t keys [4]){

    uint32_t sigma = 0x9E3779B9;
    uint32_t v0 = blocks[0];
    uint32_t v1 = blocks[1];
    long long sum = 0x278DDE6E40;

    for (int i = 0; i < 64; ++i) {
        v1 -= ((((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + keys[(sum >> 11) & 3]));
        sum -= sigma;
        v0 -= ((((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + keys[sum & 3]));
    }

    blocks[0] = v0;
    blocks[1] = v1;
}

void xtea_encrypt_block_v2(unsigned long sums [65], uint32_t blocks [2], const uint32_t keys [4]){

    uint32_t v0 = blocks[0];
    uint32_t v1 = blocks[1];

    for (int i = 0; i < 64; ++i) {
        v0 += ((((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sums[i] + keys[sums[i] & 3]));

        v1 += ((((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sums[i + 1] + keys[(sums[i + 1] >> 11) & 3]));
    }

    blocks[0] = v0;
    blocks[1] = v1;

}

void xtea_decrypt_block_v2(unsigned long sums [65], uint32_t blocks [2], const uint32_t keys [4]){

    uint32_t v0 = blocks[0];
    uint32_t v1 = blocks[1];

    for (int i = 64; i > 0; i--) { //for-loop counts backwards since the sums array has the greatest value at sums[64]
        v1 -= ((((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sums[i] + keys[(sums[i] >> 11) & 3]));

        v0 -= ((((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sums[i - 1] + keys[sums[i - 1] & 3]));
    }

    blocks[0] = v0;
    blocks[1] = v1;
}