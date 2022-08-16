#ifndef LIBC_STRING_H
#define LIBC_STRING_H

#include "../cpu/types.h"

void itoa(i32 n, i8* str);

// String functions.
i32 strlen(i8* str);
i32 strcmp(i8* a, i8* b);
void strapp(i8* str, i8 ch);
void strrev(i8* str);
void strtrn(i8* str);
void strntrn(i8* str, i32 n);

#endif
