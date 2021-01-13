#include "../cpu/isr.h"
#include "../cpu/x86.h"
#include "../cpu/i8259.h"
#include "../drivers/screen.h"
#include "../drivers/multiboot.h"
#include "kernel.h"
#include "paging.h"
#include "../libc/string.h"
#include "../libc/io.h"
#include "../libc/function.h"

void kernel_main(unsigned long magic, unsigned long addr) {
	multiboot_info_t *mbi;

	clear();

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("Invalid magic number: %#x", (unsigned) magic);
		return;
	}

	mbi = (multiboot_info_t *) addr;
	/* print flags */
	printf("flags = %#x\n", (unsigned) mbi->flags);
	/* are mem_* valid? */
	if (CHECK_BIT (mbi->flags, 0))
		printf ("mem_lower = %uKB, mem_upper = %uKB\n",
		        (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

	/* Is boot_device valid? */
	if (CHECK_BIT (mbi->flags, 1))
		printf ("boot_device = %#x\n", (unsigned) mbi->boot_device);

	/* Is the command line passed? */
	if (CHECK_BIT (mbi->flags, 2))
		printf ("cmdline = %s\n", (char *) mbi->cmdline);

//	if (CHECK_BIT (mbi->flags, 3)) {
//		int mod_count = 0;
//		int i;
//		module_t* mod = (module_t*)mbi->mods_addr;
//		while(mod_count < mbi->mods_count) {
//			FILESYSLOC = (unsigned int)mod->mod_start;
//			printf("Module %d loaded at address: 0x%#x\n", mod_count, (unsigned int)mod->mod_start);
//			printf("Module %d ends at address: 0x%#x\n", mod_count, (unsigned int)mod->mod_end);
//			printf("First few bytes of module:\n");
//			for(i = 0; i<16; i++) {
//				printf("0x%x ", *((char*)(mod->mod_start+i)));
//			}
//			printf("\n");
//			mod_count++;
//			mod++;
//		}
//	}
	/* Bits 4 and 5 are mutually exclusive! */
	if (CHECK_BIT (mbi->flags, 4) && CHECK_BIT (mbi->flags, 5))
	{
		printf ("Both bits 4 and 5 are set.\n");
		return;
	}

//	/* Is the section header table of ELF valid? */
//	if (CHECK_FLAG (mbi->flags, 5))
//	{
//		elf_section_header_table_t *elf_sec = &(mbi->elf_sec);
//
//		printf ("elf_sec: num = %u, size = 0x%#x,"
//		        " addr = 0x%#x, shndx = 0x%#x\n",
//		        (unsigned) elf_sec->num, (unsigned) elf_sec->size,
//		        (unsigned) elf_sec->addr, (unsigned) elf_sec->shndx);
//	}

	/* Are mmap_* valid? */
	if (CHECK_BIT (mbi->flags, 6))
	{
		memory_map_t *mmap;

		printf ("mmap_addr = %#x, mmap_length = 0x%x\n",
		        (unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
		for (mmap = (memory_map_t *) mbi->mmap_addr;
		     (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
		     mmap = (memory_map_t *) ((unsigned long) mmap
		                              + mmap->size + sizeof (mmap->size)))
			printf (" size = %x,     base_addr = %#x%x\n"
			        "     type = %x,  length    = %#x%x\n",
			        (unsigned) mmap->size,
			        (unsigned) mmap->base_addr_high,
			        (unsigned) mmap->base_addr_low,
			        (unsigned) mmap->type,
			        (unsigned) mmap->length_high,
			        (unsigned) mmap->length_low);
	}

	/* Construct an LDT entry in the GDT */
	{
		seg_desc_t the_ldt_desc;
		the_ldt_desc.granularity    = 0;
		the_ldt_desc.opsize         = 1;
		the_ldt_desc.reserved       = 0;
		the_ldt_desc.avail          = 0;
		the_ldt_desc.present        = 1;
		the_ldt_desc.dpl            = 0x0;
		the_ldt_desc.sys            = 0;
		the_ldt_desc.type           = 0x2;

		SET_LDT_PARAMS(the_ldt_desc, &ldt, ldt_size);
		ldt_desc_ptr = the_ldt_desc;
		lldt(KERNEL_LDT);
	}

	/* Construct a TSS entry in the GDT */
	{
		seg_desc_t the_tss_desc;
		the_tss_desc.granularity    = 0;
		the_tss_desc.opsize         = 0;
		the_tss_desc.reserved       = 0;
		the_tss_desc.avail          = 0;
		the_tss_desc.seg_lim_19_16  = TSS_SIZE & 0x000F0000;
		the_tss_desc.present        = 1;
		the_tss_desc.dpl            = 0x0;
		the_tss_desc.sys            = 0;
		the_tss_desc.type           = 0x9;
		the_tss_desc.seg_lim_15_00  = TSS_SIZE & 0x0000FFFF;

		SET_TSS_PARAMS(the_tss_desc, &tss, tss_size);

		tss_desc_ptr = the_tss_desc;

		tss.ldt_segment_selector = KERNEL_LDT;
		tss.ss0 = KERNEL_DS;
		tss.esp0 = 0x800000;
		ltr(KERNEL_TSS);
	}

	/* Initialize devices, memory, filesystem, enable device interrupts on PIC, etc... */
	/* PIC */
	i8259_init();
	/* Disable interrupts - previous cli() called in boot.S */
	sti();

	printf("Type something, it will go through the kernel.\n"
	       "Type END to halt the CPU.\n> ");
}

void user_input(char *input) {
	if (strcmp(input, "END") == 0) {
		printf("Stopping the CPU.\n");
		asm volatile("hlt");
	} else if (strcmp(input, "CLEAR") == 0) {
		clear();
	} else if (strcmp(input, "CPAE") == 0) {
		int ret = check_pae();
		char digs[32] = {0};
		int_to_ascii(ret, &digs[0]);
		printf("PAE Enabled: %s", digs);
	}
	printf("\n> ");
}