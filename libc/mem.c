#include "mem.h"

void memcpy(u8 *dest, u8 *src, i32 n)
{
    for(i32 i = 0; i < n; i++)
        dest[i] = src[i];
}

void memset(u8* dest, u8 val, i32 len)
{
    for(i32 i = 0; i < len; i++)
        dest[i] = val;
}
