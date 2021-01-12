//
// Created by amadeus on 1/12/21.
//

#ifndef REOS_STRING_H
#define REOS_STRING_H

/** Converts an integer to an ASCII-encoded string. */
void int_to_ascii(int n, char str[]);

void reverse(char s[]);

void backspace(char s[]);

void append(char s[], char n);

/** Returns the length of a null-terminated string. */
int strlen(char s[]);
/**
 * Compares two strings. Returns zero if all characters are equal, a negative value if s1 is less than s2, or a
 * positive value if s1 is greater than s2. */
int strcmp(char s1[], char s2[]);

#endif //REOS_STRING_H
