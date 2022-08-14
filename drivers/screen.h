#ifndef DRIVERS_SCREEN_H
#define DRIVERS_SCREEN_H

  #define VIDEO_ADDRESS 0xb8000
  #define MAX_ROWS 25
  #define MAX_COLS 80

  // Screen IO ports
  #define REG_SCREEN_CTRL 0x3d4
  #define REG_SCREEN_DATA 0x3d5

  // Default color
  #define WHITE_ON_BLACK 0x0f

  // External types
  #include "../cpu/types.h"

  // Kernel public API
  void clear_screen();
  void kprint_at(i8* message, i32 col, i32 row);
  void kprint(i8* message);

#endif
