[bits 16]
kernel_load:
    mov bx, MESSAGE_KERNEL_LOAD           ;
    call print_string                     ;
    call print_nl                         ; print the loading message to the screen
    mov bx, KERNEL_OFFSET                 ; read from disk and store in 0x1000
    mov dh, 16                            ; our future kernel will be larger, make this big
    mov dl, [BOOT_DRIVE]                  ; save the boot drive to the register
    call disk_load                        ; load the data from the disk to memory
    mov bx, MESSAGE_KERNEL_LOAD_COMPLETE  ;
    call print_string                     ;
    call print_nl                         ; print the complete message to the screen
    ret

; data
MESSAGE_KERNEL_LOAD: db 'Loading kernel...', 0
MESSAGE_KERNEL_LOAD_COMPLETE: db 'Kernel load complete!', 0

; include disk utilities
%include 'boot/disk.asm'
