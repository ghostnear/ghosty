#ifndef CPU_TYPES_H
#define CPU_TYPES_H

  // Define the standard types
  #include <stdbool.h>
  #include <stdint.h>

  // Operations to get values easier
  #define low_16(v32) (uint16_t)((v32) & 0xFFFF)
  #define high_16(v32) (uint16_t)(((v32) >> 16) & 0xFFFF)

#endif
