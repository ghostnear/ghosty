#include "../drivers/screen.h"
//#include "../cpu/isr.h"
//#include "../cpu/idt.h"

void kernel_main()
{
    // Kernel setup
    //isr_install();
    clear_screen();

    /* Test the interrupts */
    //__asm__ __volatile__("int $2");
    //__asm__ __volatile__("int $3");

    // Code
    kprint("Hello World!");
}
