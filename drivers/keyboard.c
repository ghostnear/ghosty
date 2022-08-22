#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "../kernel/kernel.h"
#include "../libc/bits/stdc.h"
#include "screen.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C

static int8_t key_buffer[256] = {0};

#define SC_MAX 57
const int8_t *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6",
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const int8_t sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

static void keyboard_callback(registers_t regs)
{
    /* The PIC leaves us the scancode in port 0x60 */
    uint8_t scancode = port_byte_in(0x60);

    if (scancode > SC_MAX) return;
    if (scancode == BACKSPACE) {
        strtrn(key_buffer);
        kprint_backspace();
    } else if (scancode == ENTER) {
        kprint("\n");
        on_user_input(key_buffer); /* kernel-controlled function */
        key_buffer[0] = '\0';
    } else {
        int8_t letter = sc_ascii[(int32_t)scancode];
        /* Remember that kprint only accepts char[] */
        int8_t str[2] = {letter, '\0'};
        strapp(key_buffer, letter);
        kprint(str);
    }
    UNUSED(regs);
}

void init_keyboard()
{
   register_interrupt_handler(IRQ1, keyboard_callback);
}
