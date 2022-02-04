#include "version_definer.h"

void define_version(int version, bool d, unsigned long sums [64], uint32_t blocks[2], const uint32_t keys[4]){
    switch(version){
        case 1:
            if(d == true){
                xtea_decrypt_block_v1(blocks, keys);
            }else{
                xtea_encrypt_block_v1(blocks, keys);
            }break;

        case 2:
            if(d == true){

                xtea_decrypt_block_v2(sums, blocks, keys);
            }else{
                xtea_encrypt_block_v2(sums, blocks, keys);
            }break;

        case 3:{

            long v [2] = {(long)blocks[0], (long)blocks[1]};
            long k [4] = {(long)keys[0], (long)keys[1], (long)keys[2], (long)keys[3]};

            if(d == true){
                xtea_v3(blocks, keys, -64);
            }else{
                xtea_v3(blocks, keys, 64);
            }break;
        }
        default:
            if(d == true){
                xtea_decrypt_block(blocks, keys);
            }else{
                xtea_encrypt_block(blocks, keys);
            }break;
    }
}