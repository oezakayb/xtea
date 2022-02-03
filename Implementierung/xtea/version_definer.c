#include "version_definer.h"

void define_version(int version, bool d, uint32_t [2] blocks, const uint32_t [4] keys){
    switch(version){
        case 1:
            if(d == true){
                xtea_decrypt_block_v1(blocks, keys);
            }else{
                xtea_encrypt_block_v1(blocks, keys);
            }break;

        case 2:
            long [2] v = {blocks[0], blocks[1]};
            long [4] k = {keys[0], keys[1], keys[2], keys[3]};

            if(d == true){
                xtea_v2(v, k, -64)
            }else{
                xtea_v2(v, k, 64)
            }break;

        default:
            if(d == true){
                xtea_decrypt_block(blocks, keys);
            }else{
                xtea_encrypt_block(blocks, keys);
            }break;
    }
}