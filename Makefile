# Get all the files
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
C_HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)

V_SOURCES = $(wildcard kernel/*.v drivers/*.v cpu/*.v libc/*.v)

# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

# Change this if your cross-compiler is somewhere else
V = v
CC = i386-elf-gcc
GDB = i386-elf-gdb

# -g: Debugging symbols in gcc
C_FLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -m32
V_FLAGS = -os vinix -shared -gc none

# Default rule
os-image.bin: boot/bootsec.bin kernel.bin
	cat $^ > os-image.bin

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^

run: os-image.bin
	qemu-system-i386 -fda os-image.bin

# Open the connection to qemu and load our kernel-object file with symbols
debug: os-image.bin kernel.elf
	qemu-system-i386 -s -fda os-image.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.v
	${V} ${V_FLAGS} $< -o $@

%.o: %.c ${C_HEADERS}
	${CC} ${C_FLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o
