#ifndef DRIVER_PORTS_H
#define DRIVER_PORTS_H

  // External types
  #include "../cpu/types.h"

  // For bytes
  uint8_t port_byte_in(uint16_t port);
  void port_byte_out(uint16_t port, uint8_t data);

  // For words
  uint16_t port_word_in(uint16_t port);
  void port_word_out(uint16_t port, uint16_t data);

#endif
