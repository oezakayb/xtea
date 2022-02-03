#ifndef XTEA_V0_H_   /* Include guard */
#define XTEA_V0_H_
#include "xtea_v0.h"
#include "xtea_v1.h"
#include "xtea_v2.h"
#include <stdint.h>

void define_version(char* version, uint32_t* blocks, const uint32_t* keys);
#endif