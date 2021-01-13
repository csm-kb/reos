//
// Created by amadeus on 1/12/21.
//

#ifndef REOS_TYPES_H
#define REOS_TYPES_H

#define _128MB 0x8000000
#define _132MB 0x8400000
#define _136MB 0x8800000
#define _100MB 0x6400000
#define _8MB 0x800000
#define _4MB 0x400000
#define _8KB 0x2000
#define _4KB 0x1000
#define ASCII_DEL 0x7f
#define ASCII_E 0x45
#define ASCII_L 0x4c
#define ASCII_F 0x46
#define ASCII_NL 0x0A

/* make sure we have other types too */
#include <stddef.h>
#include <stdint.h>

#ifndef _SSIZE_T
#define _SSIZE_T
typedef long            ssize_t;
#endif /* _SSIZE_T */

#endif //REOS_TYPES_H
