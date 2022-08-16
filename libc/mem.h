#ifndef LIBC_MEM_H
#define LIBC_MEM_H

#include "../cpu/types.h"

void memcpy(u8 *dest, u8 *src, i32 n);
void memset(u8* dest, u8 val, i32 len);

#endif
