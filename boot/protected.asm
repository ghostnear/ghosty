[bits 16]
switch_to_protected_mode:
    cli                               ; no interrupts
    lgdt [gdt_descriptor]             ; load gdt descriptor
    mov eax, cr0                      ;
    or eax, 0x1                       ; 32 bit mode in cr0
    mov cr0, eax                      ;
    jmp CODE_SEG:init_protected_mode  ; far mode jump to the protected mode init

[bits 32]
init_protected_mode:
    mov ax, DATA_SEG                  ;
    mov ds, ax                        ;
    mov ss, ax                        ;
    mov es, ax                        ;
    mov fs, ax                        ;
    mov gs, ax                        ; update segment registers
    mov ebp, 0x90000                  ;
    mov esp, ebp                      ; set up the stack at the top of the free space
    call KERNEL_OFFSET                ; give control to the kernel
    jmp $                             ; infinite loop (for safety)

; include the GDT
%include 'boot/gdt.asm'
