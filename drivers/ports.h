#ifndef DRIVER_PORTS_H
#define DRIVER_PORTS_H

  // External types
  #include "../cpu/types.h"

  // For bytes
  u8 port_byte_in(u16 port);
  void port_byte_out(u16 port, u8 data);

  // For words
  u16 port_word_in(u16 port);
  void port_word_out(u16 port, u16 data);

#endif
