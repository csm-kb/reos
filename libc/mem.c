//
// Created by amadeus on 1/12/21.
//

#include "mem.h"

void *memcpy(void *dst, const void *src, size_t len) {
	size_t i;
	/* memcpy does not support overlapping buffers, so always do it forwards. (Don't change this without adjusting
	 * memmove.) For speedy copying, optimize the common case where both pointers and the length are word-aligned,
	 * and copy word-at-a-time instead of byte-at-a-time. Otherwise, copy by bytes. The alignment logic below should
	 * be portable. We rely on the compiler to be reasonably intelligent about optimizing the divides and modulos
	 * out. Fortunately, it is.
         */
	if ((uintptr_t) dst % sizeof(long) == 0 && (uintptr_t) src % sizeof(long) == 0 && len % sizeof(long) == 0) {
		long *d = dst;
		const long *s = src;

		for (i = 0; i < len / sizeof(long); i++) {
			d[i] = s[i];
		}
	} else {
		char *d = dst;
		const char *s = src;

		for (i = 0; i < len; i++) {
			d[i] = s[i];
		}
	}

	return dst;
}

void *memset(void *ptr, uint8_t value, size_t len) {
	size_t i;
	if ((uintptr_t) ptr % sizeof(long) == 0 && len % sizeof(long) == 0) {
		long *d = ptr;

		for (i = 0; i < len / sizeof(long); i++) {
			d[i] = value;
		}
	} else {
		char *d = ptr;

		for (i = 0; i < len; i++) {
			d[i] = value;
		}
	}
}

void *memmove(void *dst, const void *src, size_t len) {
	size_t i;
	/* If the buffers don't overlap, it doesn't matter what direction we copy in. If they do, it does, so just
	 * assume they always do. We don't concern ourselves with the possibility that the region to copy might roll
	 * over across the top of memory, because it's not going to happen.
	 *
	 * If the destination is above the source, we have to copy back to front to avoid overwriting the data we want
	 * to copy.
	 *
	 *      dest:       dddddddd
	 *      src:    ssssssss   ^
	 *              |   ^  |___|
	 *              |___|
	 *
	 * If the destination is below the source, we have to copy front to back.
	 *
	 *      dest:   dddddddd
	 *      src:    ^   ssssssss
	 *              |___|  ^   |
	 *                     |___|
	 */

	if ((uintptr_t) dst < (uintptr_t) src) {
		/* As author/maintainer of libc, take advantage of the fact that we know memcpy copies forwards. */
		return memcpy(dst, src, len);
	}

	/* Copy by words in the common case. Look in memcpy for more information. */

	if ((uintptr_t) dst % sizeof(long) == 0 && (uintptr_t) src % sizeof(long) == 0 && len % sizeof(long) == 0) {
		long *d = dst;
		const long *s = src;

		/* The reason we copy index i-1 and test i>0 is that i is unsigned -- so testing i>=0 doesn't work. */
		for (i = len / sizeof(long); i > 0; i--) {
			d[i - 1] = s[i - 1];
		}
	} else {
		char *d = dst;
		const char *s = src;

		for (i = len; i > 0; i--) {
			d[i - 1] = s[i - 1];
		}
	}

	return dst;
}

int memcmp(const void *ptr1, const void *ptr2, size_t len) {
	uint32_t i;

	if ((uint64_t)ptr1 % sizeof(long) == 0 && (uint64_t)ptr2 % sizeof(long) == 0 && len % sizeof(long) == 0) {
		const long *d = ptr1;
		const long *s = ptr2;

		for (i = 0; i < len / sizeof(long); i++) {
			if (d[i] < s[i])
				return -1;
			else if (d[i] > s[i])
				return 1;
		}
		return 0;
	} else {
		const char *d = ptr1;
		const char *s = ptr2;

		for (i = 0; i < len; i++) {
			if (d[i] < s[i])
				return -1;
			else if (d[i] > s[i])
				return 1;
		}
		return 0;
	}
}

/* This should be computed at link time, but a hardcoded
 * value is fine for now. Remember that our kernel starts
 * at 0x1000 as defined on the Makefile */
uint32_t free_mem_addr = 0x10000;

void *kmalloc(size_t size, int align, void *paddr) {

}