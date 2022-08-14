#include "ports.h"
#include "screen.h"

/* Kernel private API */
i32 get_cursor_offset();
void set_cursor_offset(i32 offset);
i32 get_offset(i32 col, i32 row);
i32 get_offset_row(i32 offset);
i32 get_offset_col(i32 offset);
i32 print_char(i8 c, i32 col, i32 row, i8 attr);

/*
* Print a message at the specified position.
* If the coords are negative, we will be using the current offset.
*/
void kprint_at(i8* message, i32 col, i32 row)
{
    i32 offset;
    // If any of the coords are negatibe, replace them with the current ones.
    if(col < 0 || row < 0)
    {
        offset = get_cursor_offset();
        row = (row < 0) ? get_offset_row(offset) : row;
        col = (col < 0) ? get_offset_col(offset) : col;
    }
    offset = get_offset(col, row);

    /* Loop through message and print it */
    for(int i = 0; message[i]; i++)
    {
        offset = print_char(message[i], col, row, WHITE_ON_BLACK);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

// This is a particular case of kprint_at()
void kprint(i8* message)
{
    kprint_at(message, -1, -1);
}

// Clear the screen by setting it to spaces.
void clear_screen()
{
  i8 *screen = (i8*) VIDEO_ADDRESS;
  for(i32 i = 0; i < MAX_COLS * MAX_ROWS; i++)
  {
      screen[i * 2] = ' ';
      screen[i * 2 + 1] = WHITE_ON_BLACK;
  }

  // Set cursor to top left
  set_cursor_offset(get_offset(0, 0));
}

// Copy bytes from one source to another
void memory_copy(i8* src, i8* dest, i32 num)
{
    for(i32 i = 0; i < num; i++)
      dest[i] = src[i];
}

/*
* Prints a character with the specified attributes to the location.
* Doesn't print anything if the location is invalid.
*/
i32 print_char(i8 c, i32 col, i32 row, i8 attr)
{
    // Get a pointer to the VRAM
    u8 *vidmem = (u8*) VIDEO_ADDRESS;

    /* Error control */
    if(!attr) attr = WHITE_ON_BLACK;
    if(col >= MAX_COLS || row >= MAX_ROWS) return -1;

    // Get offset if the coords are positive
    i32 offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    // Treat special chars separately
    switch(c)
    {
        // Unprintable, ignore
        case 0:
          break;

        // Move to the next line
        case '\n':
          row = get_offset_row(offset);
          offset = get_offset(0, row + 1);
          break;

        // Default - print
        default:
          vidmem[offset++] = c;
          vidmem[offset++] = attr;
          break;
    }

    // Check for scrolling
    if(offset >= MAX_ROWS * MAX_COLS * 2)
    {
        // Move everything up one line
        for(i32 i = 1; i < MAX_ROWS; i++)
        {
            memory_copy(
              (i8*)(get_offset(0, i) + VIDEO_ADDRESS),
              (i8*)(get_offset(0, i - 1) + VIDEO_ADDRESS),
              MAX_COLS * 2
            );
        }

        // Make last line is empty
        i8 *last_line = (i8*)(get_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS);
        for(i32 i = 0; i < MAX_COLS * 2; i++) last_line[i] = 0;

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
i32 get_cursor_offset()
{
    // Read high byte
    port_byte_out(REG_SCREEN_CTRL, 14);
    i32 offset = port_byte_in(REG_SCREEN_DATA) << 8;

    // Read lower byte
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset &= port_byte_in(REG_SCREEN_DATA);

    // Return the offset (position * size of character cell)
    return offset * 2;
}

// Similar to getting but we only write data
void set_cursor_offset(i32 offset)
{
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (u8)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (u8)(offset & 0xff));
}

// TODO: maybe replace with macros
i32 get_offset(i32 col, i32 row) { return 2 * (row * MAX_COLS + col); }
i32 get_offset_row(i32 offset) { return offset / (2 * MAX_COLS); }
i32 get_offset_col(i32 offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }
