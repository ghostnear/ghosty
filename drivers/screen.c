#include "ports.h"
#include "screen.h"

/* Kernel private API */
int get_cursor_offset();
void set_cursor_offset(int offset);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);
int print_char(char c, int col, int row, char attr);

/*
* Print a message at the specified position.
* If the coords are negative, we will be using the current offset.
*/
void kprint_at(char* message, int col, int row)
{
    int offset;
    // If any of the coords are negatibe, replace them with the current ones.
    if(col < 0 || row < 0)
    {
        offset = get_cursor_offset();
        row = (row < 0) ? get_offset_row(offset) : row;
        col = (col < 0) ? get_offset_col(offset) : col;
    }
    offset = get_offset(col, row);

    /* Loop through message and print it */
    int i = 0;
    while(message[i])
    {
        offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

// This is a particular case of kprint_at()
void kprint(char* message)
{
    kprint_at(message, -1, -1);
}

// Clear the screen by setting it to spaces.
void clear_screen()
{
  char *screen = VIDEO_ADDRESS;
  for(int i = 0; i < MAX_COLS * MAX_ROWS; i++)
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
int print_char(char c, int col, int row, char attr)
{
    // Get a pointer to the VRAM
    unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;

    /* Error control */
    if(!attr) attr = WHITE_ON_BLACK;
    if(col >= MAX_COLS || row >= MAX_ROWS) return -1;

    // Get offset if the coords are positive
    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    // Treat special chars separately
    switch(c)
    {
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

    // Update the offset and return it
    set_cursor_offset(offset);
    return offset;
}

/*
* Use the VGA ports to get the current cursor position
* 1. Ask for high byte of the cursor offset (data 14)
* 2. Ask for low byte (data 15)
*/
int get_cursor_offset()
{
    // Read high byte
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;

    // Read lower byte
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset &= port_byte_in(REG_SCREEN_DATA);

    // Return the offset (position * size of character cell)
    return offset * 2;
}

// Similar to getting but we only write data
void set_cursor_offset(int offset)
{
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

// TODO: maybe replace with macros
int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }
