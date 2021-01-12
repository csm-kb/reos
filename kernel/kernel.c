#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"

void kernel_main() {
	isr_install();
	irq_install();

	clear_screen();
	kprint("Type something, it will go through the kernel.\n"
	       "Type END to halt the CPU.\n> ");
}

void user_input(char *input) {
	if (strcmp(input, "END") == 0) {
		kprint("Stopping the CPU.\n");
		asm volatile("hlt");
	} else if (strcmp(input, "CLEAR") == 0) {
		clear_screen();
	}
	kprint("You said: ");
	kprint(input);
	kprint("\n> ");
}