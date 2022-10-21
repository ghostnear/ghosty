#ifndef LIBC_STRING_H
#define LIBC_STRING_H

  #include "../cpu/types.h"

  void itoa(int32_t n, char* str);

  // String functions.
  int32_t strlen(char* str);
  int32_t strcmp(char* a, char* b);
  void strapp(char* str, char ch);
  void strrev(char* str);
  void strtrn(char* str);
  void strntrn(char* str, int32_t n);

#endif
