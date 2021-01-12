//
// Created by amadeus on 1/12/21.
//

#ifndef REOS_MEM_H
#define REOS_MEM_H

#ifndef void
#define void    void
#endif /* void */

#include <stdint.h>
#include "stddef.h"

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

/**
 * Copies the values of nbytes from the location pointed to by source directly to the memory block pointed to by dest.
 */
void *memcpy(void *dst, const void *src, size_t len);

/**
 * Sets the first len bytes of the block of memory pointed by ptr to the specified value.
 */
void *memset(void *ptr, uint8_t value, size_t len);

/**
 * Copies the values of num bytes from the location pointed by source to the memory block pointed by destination.<br>
 * <br>Copying takes place as if an intermediate buffer were used, allowing the destination and source to overlap.
 */
void *memmove(void *dst, const void *src, size_t len);

/**
 * Compares the first num bytes of the block of memory pointed by ptr1 to the first num bytes pointed by ptr2, returning
 * zero if they all match or a value different from zero representing which is greater if they do not.
 */
int memcmp(const void *ptr1, const void *ptr2, size_t len);

void *kmalloc(size_t size, size_t align, void *paddr);

#endif //REOS_MEM_H
