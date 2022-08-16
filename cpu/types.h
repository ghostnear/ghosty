#ifndef CPU_TYPES_H
#define CPU_TYPES_H

// Define some standard types
typedef int i32;
typedef short i16;
typedef char i8;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned char bool;

// Operations to get values easier
#define low_16(v32) (u16)((v32) & 0xFFFF)
#define high_16(v32) (u16)(((v32) >> 16) & 0xFFFF)

#endif
