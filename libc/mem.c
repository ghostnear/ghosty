#include "mem.h"

void memcpy(uint8_t *dest, uint8_t *src, int32_t n)
{
    while((--n) >= 0)
        dest[n] = src[n];
}

void memset(uint8_t* dest, uint8_t val, int32_t len)
{
    while((--len) >= 0)
        dest[len] = val;
}
