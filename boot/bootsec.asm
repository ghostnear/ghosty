[org 0x7c00]              ; global offset where the BIOS places the boot sector
KERNEL_OFFSET equ 0x1000  ; offset where we place the kernel

mov sp, 0x9000                ;
mov bp, sp;                   ; set up the stack
mov [BOOT_DRIVE], dl          ; save the boot drive from the BIOS
mov bx, MESSAGE_REAL_MODE     ;
call print_string             ;
call print_nl                 ; print string to show we are in 16 bit mode
call kernel_load              ; load the kernel
call switch_to_protected_mode ; move to the protected mode
jmp $                         ; infinite loop (for safety)

; include code
%include 'boot/print.asm'
%include 'boot/protected.asm'
%include 'boot/kernel_load.asm'

; data
BOOT_DRIVE: db 0
MESSAGE_REAL_MODE: db 'Entering 16-bit mode...', 0

times 510-($-$$) db 0   ; byte padding so we are sure to have 512 bytes
dw 0xaa55               ; magic number as the last 2 bytes
