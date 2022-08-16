#include "ports.h"

// Read a byte from the specified port
u8 port_byte_in(u16 port)
{
    u8 result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

// Send a byte to the specified port
void port_byte_out(u16 port, u8 data)
{
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

// Read a word from the specified port
u16 port_word_in(u16 port)
{
    unsigned short result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

// Send a word to the specified port
void port_word_out(u16 port, u16 data)
{
    asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}
