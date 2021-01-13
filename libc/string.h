//
// Created by amadeus on 1/12/21.
//

#ifndef REOS_STRING_H
#define REOS_STRING_H

#include <stddef.h>

/** Converts an integer to an ASCII-encoded string. */
void int_to_ascii(int n, char *str);
/** Converts a hexadecimal number to an ASCII-encoded string. */
void hex_to_ascii(int n, char *str);

void reverse(char *s);

void append(char *s, char n);

/** Returns the length of a null-terminated string. */
size_t strlen(const char *s);
/**
 * Compares two strings. Returns zero if all characters are equal, a negative value if s1 is less than s2, or a
 * positive value if s1 is greater than s2. */
int strcmp(const char *s1, const char *s2);
/**
 * Compares two strings, reading up to the specified num of bytes. Returns zero if all characters are equal, a negative
 * value if s1 is less than s2, or a positive value if s1 is greater than s2.
 */
int strncmp(const char *s1, const char *s2, size_t num);
/**
 * Appends one string to another. The array pointed to by dst must have enough room to support holding
 * itself plus the contents of src. Both strings must be null-terminated.
 */
char *strcat(char* dst, const char *src);
/**
 * Find the leftmost instance of a character in the given string.
 */
char *strchr(const char *s, int ch_arg);
/**
 * Find the rightmost instance of a character in the given string.
 */
char *strrchr(const char *s, int ch_arg);
/**
 * Copy the string pointed to by src to the string pointed to by dst.
 */
char *strcpy(char *dst, const char *src);
/**
 * Tokenize a string, splitting based on a list of separator characters.<br>Re-entrant version.<br>
 * The 'context' argument should point to a char* that is preserved between calls to strtok that wish to operate on
 * the same string.
 */
char *strtok_r(char *str, const char *seps, char **context);

#endif //REOS_STRING_H
