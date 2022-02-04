#ifndef VERSION_DEFINER_H_   /* Include guard */
#define VERSION_DEFINER_H_
#include "xtea_v0.h"
#include "xtea_v1.h"
#include "xtea_v3.h"
#include <stdint.h>
#include <stdbool.h>

void define_version(int version,  bool d, unsigned long sums [64], uint32_t blocks[2], const uint32_t keys[4]);
#endif