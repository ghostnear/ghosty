#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../cpu/timer.h"
#include "../libc/bits/stdc.h"

void kernel_main()
{
    // Kernel setup
    isr_install();
    irq_install();

    // MOTD
    kprint("\n");
    kprint("Welcome to Ghosty!\n");
    kprint("Type something, it will go through the kernel!\n");
    kprint("Typing END will halt the CPU.\n> ");
}

void on_user_input(char *input)
{
    // Stop the CPU on END
    if(!strcmp(input, "END"))
    {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }
    else if(!strcmp(input, "CLEAR"))
    {
        clear_screen();
    }
    else
    {
        kprint("Unrecognized input: ");
        kprint(input);
        kprint("\n> ");
    }
}
