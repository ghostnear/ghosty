; load 'dh' sectors from drive 'dl' into ES:BX
disk_load:
    pusha                 ;
    push dx               ; save registers to the stack as all registers are overwritten
    mov ah, 0x02          ; ah <- int 0x13 function. 0x02 = 'read'
    mov al, dh            ; al <- number of sectors to read (0x01 .. 0x80)
    mov cl, 0x02          ; cl <- sector (0x01 .. 0x11)
                          ; 0x01 is our boot sector, 0x02 is the first 'available' sector
    mov ch, 0x00          ; ch <- cylinder (0x0 .. 0x3FF, upper 2 bits in 'cl')
                          ; dl <- drive number. Our caller sets it as a parameter and gets it from BIOS
                          ; (0 = floppy, 1 = floppy2, 0x80 = hdd, 0x81 = hdd2)
    mov dh, 0x00          ; dh <- head number (0x0 .. 0xF)
                          ; [es:bx] <- pointer to buffer where the data will be stored
    int 0x13              ; caller sets it up for us, and it is actually the standard location for int 13h
    jc disk_error         ; if error (stored in the carry bit)
    pop dx                ; get dx back from the stack
    cmp al, dh            ;
    jne sectors_error     ; BIOS also sets 'al' to the # of sectors read. Check for issues.
    popa                  ; get back the state
    ret

disk_error:
    mov bx, DISK_ERROR    ;
    call print_string     ;
    call print_nl         ; print the disk error string
    mov dh, ah            ; ah = error code, dl = disk drive that dropped the error
    call print_hex        ; check out the code at http://stanislavs.org/helppc/int_13-1.html
    jmp disk_loop         ; infinite loop

sectors_error:
    mov bx, SECTORS_ERROR ;
    call print_string     ; print the message

disk_loop:
    jmp $                 ; infinite loop

DISK_ERROR: db "Disk read error! ", 0
SECTORS_ERROR: db "Incorrect number of sectors read! ", 0
