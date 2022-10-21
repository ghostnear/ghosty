global _start:
[bits 32]

_start:
    [extern kernel_main]  ; mark kernel_main as external because it is not implemented here.
    call kernel_main      ; call it
    jmp $                 ; infinite loop in case of any issues occuring
