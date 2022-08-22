#include "mem.h"

void memcpy(uint8_t *dest, uint8_t *src, int32_t n)
{
    for(int32_t i = 0; i < n; i++)
        dest[i] = src[i];
}

void memset(uint8_t* dest, uint8_t val, int32_t len)
{
    for(int32_t i = 0; i < len; i++)
        dest[i] = val;
}
