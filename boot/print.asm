; prints the string at the adress [bx]
print_string:
    pusha
start:
    mov al, [bx]    ; save the byte to the printing register
    cmp al, 0       ; check if the byte pointed by bx is null
    je done         ; and jump accordingly
    mov ah, 0x0e    ; ah has the command code for printing
    int 0x10        ; send the command to the BIOS
    add bx, 1       ;
    jmp start       ; advance and go to the next step
done:
    popa
    ret

; prints a newline
print_nl:
    pusha
    mov ah, 0x0e    ; prepare the command
    mov al, 0x0a    ; newline char
    int 0x10        ; raise interrupt
    mov al, 0x0d    ; carriage return
    int 0x10        ; raise interrupt again
    popa
    ret

; prints a hex number
; receiving the data in 'dx'
print_hex:
    pusha
    mov cx, 0           ; index in the result string
hex_loop:
    cmp cx, 4           ;
    je end              ; end after 4 loops
    mov ax, dx          ; we will use 'ax' as our working register
    and ax, 0x000f      ; get only the last hex digit stored
    add al, 0x30        ; add '0' to make it an ASCII character
    cmp al, 0x39        ;
    jle step2           ; if <= 9 we can go directly to saving it
    add al, 7           ; if > 9 we add 7 to fix the offset

step2:
    mov bx, HEX_OUT + 5 ; get adress of the end of the string
    sub bx, cx          ; subtract the index
    mov [bx], al        ; set the value at that adress to our result
    ror dx, 4           ; rotate the number to get access to the next digit
                        ; ! digits are in the opposite order, that's why the index is subtracted instead of used !
    add cx, 1           ;
    jmp hex_loop        ; go to the next step

end:
    mov bx, HEX_OUT     ; move the adress of the string to bx
    popa
    call print_string   ; because the function prints the string at the adress of bx
    ret

; data
HEX_OUT:
    db '0x0000',0 ; reserve memory for our new string
