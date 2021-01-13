# Declare constants used for creating a multiboot header.
#define ASM 1
#include "../drivers/multiboot.h"
#include "../cpu/x86.h"

.set STACK_SIZE, 0x4000

# The linker script specifies _start as the entry point to the kernel and the
# bootloader will jump to this position once the kernel has been loaded.
	.text
	.globl _start
_start:
	# Make sure interrupts are off
	cli
	jmp multiboot_entry
	.align 4

multiboot_header:
	.long MULTIBOOT_HEADER_MAGIC
    .long MULTIBOOT_HEADER_FLAGS
    .long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

    .long 0
    .long 0
    .long 0
    .long 0
    .long 0

    .long 0
    .long 1024
    .long 768
    .long 0

multiboot_entry:
	# Load the GDT
	lgdt gdt_desc_ptr

	# Load the IDT
	push %eax
	push %edx
	push %ecx

	call init_interrupts

	pop %ecx
	pop %edx
	pop %eax

	# Load CS with new descriptor value
	ljmp $KERNEL_CS, $keep_going

keep_going:
	# Initialize the stack
	movl $(stack + STACK_SIZE), %esp

	# Set up the rest of the segment selector registers
    movw    $KERNEL_DS, %cx
    movw    %cx, %ss
    movw    %cx, %ds
    movw    %cx, %es
    movw    %cx, %fs
    movw    %cx, %gs

	# reset EFLAGS
	pushl $0
	popf

	# push pointer to Multiboot info struct
	pushl %ebx
	# push magic value
	pushl %eax

	# jump to C entry point
	call kernel_main

	hlt
.Lhang:
	jmp .Lhang

# Set the size of the _start symbol to the current location '.' minus its start.
# This is useful when debugging or when you implement call tracing.
.size _start, . - _start

.comm stack, STACK_SIZE
