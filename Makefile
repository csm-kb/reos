# Change this if your cross-compiler is somewhere else
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LD = /usr/local/i386elfgcc/bin/i386-elf-ld
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
# -g: Use debugging symbols in gcc
CFLAGS = -Wall -g -lgcc -DPRINTF_DISABLE_SUPPORT_LONG_LONG
LDFLAGS =

ISODIR := iso
MULTIBOOT := $(ISODIR)/boot/os-image
MAIN := os-image

OBJDIR = obj
BINDIR = bin

KERNEL_OBJ = $(addprefix $(OBJDIR)/, kernel_entry.o)
OSIMG_BIN = $(addprefix $(BINDIR)/, bootsect.bin kernel.bin)

C_SRCS = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
S_SRCS = $(wildcard cpu/*.S boot/*.S)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.c)
# Nice syntax for file extension replacement
TOBJ = $(C_SRCS:.c=.o) $(S_SRCS:.S=.o) # $(OBJDIR)/interrupt.o
OBJ = $(addprefix $(OBJDIR)/,$(notdir $(TOBJ)))

VPATH=boot:kernel:drivers:cpu:libc

.PHONY: clean run

all: $(MAIN)

# First rule is run by default
#os-image: $(OSIMG_BIN)
$(MAIN): $(OBJDIR)/boot.o $(OBJ)
	$(CC) -ffreestanding -nostdlib -o '$(MULTIBOOT)' -T linker.ld $^ -lgcc
	grub-mkrescue -o '$@' '$(ISODIR)'

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
$(BINDIR)/kernel.bin: $(KERNEL_OBJ) $(OBJ)
	$(LD) $(LDFLAGS) -o $@ -Ttext 0x1000 $^ --oformat binary

$(BINDIR)/%.bin: %.asm
	nasm $< -f bin -o $@

run: $(MAIN)
	qemu-system-i386 -kernel $(MULTIBOOT)

# Generic rules for wildcards
# To make an object, always compile from its .c
$(OBJDIR)/%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -ffreestanding -c $< -o $@

$(OBJDIR)/%.o: %.asm
	nasm $< -f elf -o $@

$(OBJDIR)/%.o: %.S $(HEADERS)
	#as -32 $< -o $@
	$(CC) -ffreestanding -nostdlib -c $< -o $@

clean:
	rm -rf "$(MAIN)" */*.bin */*.dis */*.o */*.elf
	rm -f '$(MULTIBOOT)'