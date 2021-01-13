//
// Created by amadeus on 1/13/21.
//

#ifndef REOS_PAGING_H
#define REOS_PAGING_H

#include <stdint.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE       4096 /* = 4 KB */
#endif /* PAGE_SIZE */

#define OUT_OF_PHYS_PAGES 1

void init_phys_alloc();



/**
 * Called from the kernel entry point. Initializes paging for the operating system.
 */
void init_paging();
/** Checks for Physical Address Extension (PAE). */
int check_pae();

/** Internal page type. A page pointer is 4 bytes, and describes a 4 kB chunk of memory. */
typedef uint32_t      page_t;
/** Internal page table type. Holds 1024 pages = 1024*4096 bytes (4 MB). */
typedef __attribute__((aligned(4096))) page_t          page_table_t[1024];
/** Internal page directory type. Holds 1024 page tables = 1024*1024*4096 bytes (4 GB). */
typedef __attribute__((aligned(4096))) page_table_t    page_dir_t[1024];

#endif //REOS_PAGING_H
