#include "xtea_v3.h"

void xtea_v3(uint32_t * v, const uint32_t * k, long N) {
    uint32_t y=v[0], z=v[1], DELTA=0x9e3779b9 ;
    if (N>0) {
/* coding */
        unsigned long limit=DELTA*N, sum=0 ;
        while (sum!=limit)
            y+= (z<<4 ^ z>>5) + z ^ sum + k[sum&3],
            sum+=DELTA,
            z+= (y<<4 ^ y>>5) + y ^ sum + k[sum>>11 &3] ;
    }
    else {
/* decoding */
        unsigned long sum=DELTA*(-N) ;
        while (sum)
            z-= (y<<4 ^ y>>5) + y ^ sum + k[sum>>11 &3],
            sum-=DELTA,
            y-= (z<<4 ^ z>>5) + z ^ sum + k[sum&3] ;
    }
    v[0]=y, v[1]=z ;
    return ;
}