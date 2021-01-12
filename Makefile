# Change this if your cross-compiler is somewhere else
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LD = /usr/local/i386elfgcc/bin/i386-elf-ld
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
# -g: Use debugging symbols in gcc
CFLAGS = -g

OBJDIR = obj
BINDIR = bin

KERNEL_OBJ = $(addprefix $(OBJDIR)/, kernel_entry.o)
OSIMG_BIN = $(addprefix $(BINDIR)/, bootsect.bin kernel.bin)

C_SRCS = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.c)
# Nice syntax for file extension replacement
TOBJ = $(C_SRCS:.c=.o) $(OBJDIR)/interrupt.o
OBJ = $(addprefix $(OBJDIR)/,$(notdir $(TOBJ)))

VPATH=boot:kernel:drivers:cpu:libc

# First rule is run by default
os-image: $(OSIMG_BIN)
	cat $^ > $@

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
$(BINDIR)/kernel.bin: $(KERNEL_OBJ) $(OBJ)
	$(LD) -o $@ -Ttext 0x1000 $^ --oformat binary

$(BINDIR)/%.bin: %.asm
	nasm $< -f bin -o $@

# Used for debugging purposes
kernel.elf: $(KERNEL_OBJ) $(OBJ)
	$(LD) -o $@ -Ttext 0x1000 $^

run: os-image
	qemu-system-i386 -fda os-image

# Open the connection to qemu and load our kernel-object file with symbols
debug: os-image kernel.elf
	qemu-system-i386 -s -fda os-image -d guest_errors,int &
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
$(OBJDIR)/%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -ffreestanding -c $< -o $@

$(OBJDIR)/%.o: %.asm
	nasm $< -f elf -o $@

clean:
	rm -rf "os-image" */*.bin */*.dis */*.o */*.elf