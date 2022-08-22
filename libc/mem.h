#ifndef LIBC_MEM_H
#define LIBC_MEM_H

#include "../cpu/types.h"

void memcpy(uint8_t *dest, uint8_t *src, int32_t n);
void memset(uint8_t* dest, uint8_t val, int32_t len);

#endif
