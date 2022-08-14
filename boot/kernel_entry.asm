[bits 32]
[extern kernel_main]  ; mark kernel_main as external because it is not implemented here.
call kernel_main      ; call it
jmp $                 ; infinite loop in case of any issues occuring
