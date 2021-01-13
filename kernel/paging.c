//
// Created by amadeus on 1/13/21.
//

#include "paging.h"
#include "../libc/function.h"

page_dir_t page_dir;

inline int check_pae() {
	int i;
	asm volatile(
	"mov $0x1, %%eax   \n\t"
	"cpuid             \n\t"
	"test $0x40, %%edx \n\t" /* check bit 6 (PAE), sets (ZF = zero flag) if PAE is not supported */
	"pushf             \n\t"
	"pop %%eax         \n\t"
	"push %%eax        \n\t"
	"popf              \n\t"
	"and $0x40, %%eax  \n\t" /* check bit 6 (ZF) */
	"movl %%eax, %0"         /* return value of ZF to 'i' */
	: "=r"(i)
	);
	return !i;
}

void init_paging() {
	page_table_t *ent1 = &page_dir[0];
	UNUSED(ent1);
}