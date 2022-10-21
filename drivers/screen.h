#ifndef DRIVERS_SCREEN_H
#define DRIVERS_SCREEN_H

  // External types
  #include "../cpu/types.h"

  #define VIDEO_ADDRESS 0xb8000
  #define MAX_ROWS 25
  #define MAX_COLS 80

  // Screen IO ports
  #define REG_SCREEN_CTRL 0x3d4
  #define REG_SCREEN_DATA 0x3d5

  // Default color
  #define WHITE_ON_BLACK 0x0f

  // Kernel public API
  void clear_screen();
  void kprint_at(char* message, int32_t col, int32_t row);
  void kprint(char* message);
  void kprint_backspace();

#endif
