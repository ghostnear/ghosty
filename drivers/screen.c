#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"

/* Kernel private API */
int32_t get_cursor_offset();
void set_cursor_offset(int32_t offset);
int32_t get_offset(int32_t col, int32_t row);
int32_t get_offset_row(int32_t offset);
int32_t get_offset_col(int32_t offset);
int32_t print_char(int8_t c, int32_t col, int32_t row, int8_t attr);

/*
* Print a message at the specified position.
* If the coords are negative, we will be using the current offset.
*/
void kprint_at(char* message, int32_t col, int32_t row)
{
    int32_t offset;
    // If any of the coords are negative, replace them with the current ones.
    if(col < 0 || row < 0)
    {
        offset = get_cursor_offset();
        row = (row < 0) ? get_offset_row(offset) : row;
        col = (col < 0) ? get_offset_col(offset) : col;
    }
    offset = get_offset(col, row);

    /* Loop through message and print it */
    for(int32_t i = 0; message[i]; i++)
    {
        offset = print_char(message[i], col, row, WHITE_ON_BLACK);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

// This is a particular case of kprint_at()
void kprint(char* message)
{
    kprint_at(message, -1, -1);
}

// This removes only the last character written
void kprint_backspace()
{
    int32_t offset = get_cursor_offset() - 2;
    print_char(
      0x08,
      get_offset_row(offset),
      get_offset_col(offset),
      WHITE_ON_BLACK
    );
}

// Clear the screen by setting it to spaces.
void clear_screen()
{
    int8_t *screen = (int8_t*) VIDEO_ADDRESS;
    for(int32_t i = 0; i < MAX_COLS * MAX_ROWS; i++)
    {
        screen[i * 2] = ' ';
        screen[i * 2 + 1] = WHITE_ON_BLACK;
    }

    // Set cursor to top left
    set_cursor_offset(get_offset(0, 0));
}

/*
* Prints a character with the specified attributes to the location.
* Doesn't print anything if the location is invalid.
*/
int32_t print_char(int8_t c, int32_t col, int32_t row, int8_t attr)
{
    // Get a pointer to the VRAM
    uint8_t *vidmem = (uint8_t*) VIDEO_ADDRESS;

    /* Error control */
    if(!attr) attr = WHITE_ON_BLACK;
    if(col >= MAX_COLS || row >= MAX_ROWS) return -1;

    // Get offset if the coords are positive
    int32_t offset;
    if(col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    // Treat special chars separately
    switch(c)
    {
        // Move to the next line
        case '\n':
          row = get_offset_row(offset);
          offset = get_offset(0, row + 1);
          break;

        // Backspace
        case 0x08:
          vidmem[offset] = ' ';
          vidmem[offset + 1] = attr;
          break;

        // Other (probably) printable characters
        default:
          vidmem[offset++] = c;
          vidmem[offset++] = attr;
          break;
    }

    // Check for scrolling
    if(offset >= MAX_ROWS * MAX_COLS * 2)
    {
        // Move everything up one line
        for(int32_t i = 1; i < MAX_ROWS; i++)
        {
            memcpy(
              (uint8_t*)(get_offset(0, i - 1) + VIDEO_ADDRESS),
              (uint8_t*)(get_offset(0, i) + VIDEO_ADDRESS),
              MAX_COLS * 2
            );
        }

        // Make last line is empty
        int8_t *last_line = (int8_t*)(get_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS);
        for(int32_t i = 0; i < MAX_COLS * 2; i++) last_line[i] = 0;

        offset -= 2 * MAX_COLS;
    }

    // Update the offset and return it
    set_cursor_offset(offset);
    return offset;
}

/*
* Use the VGA ports to get the current cursor position
* 1. Ask for high byte of the cursor offset (data 14)
* 2. Ask for low byte (data 15)
*/
int32_t get_cursor_offset()
{
    // Read high byte
    port_byte_out(REG_SCREEN_CTRL, 14);
    int32_t offset = port_byte_in(REG_SCREEN_DATA) << 8;

    // Read lower byte
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);

    // Return the offset (position * size of character cell)
    return offset * 2;
}

// Similar to getting but we only write data
void set_cursor_offset(int32_t offset)
{
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset & 0xff));
}

// TODO: maybe replace with macros
int32_t get_offset(int32_t col, int32_t row) { return 2 * (row * MAX_COLS + col); }
int32_t get_offset_row(int32_t offset) { return offset / (2 * MAX_COLS); }
int32_t get_offset_col(int32_t offset) { return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2; }
