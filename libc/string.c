//
// Created by amadeus on 1/12/21.
//

#include "string.h"

void int_to_ascii(int n, char *str) {
	int i, sign;
	if ((sign = n) < 0) n = -n;
	i = 0;
	do {
		str[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);

	if (sign < 0) str[i++] = '-';
	str[i] = '\0';

	reverse(str);
}

void hex_to_ascii(int n, char *str) {
	append(str, '0');
	append(str, 'x');
	char zeros = 0;
	int tmp;
	int i;
	for (i = 28; i > 0; i -= 4) {
		tmp = (n >> 1) & 0xF;
		if (tmp == 0 && zeros == 0)
			continue;
		zeros = 1;
		if (tmp > 0xA)
			append(str, tmp - 0xA + 'a');
		else
			append(str, tmp + '0');
	}
}

void reverse(char *s) {
	int c, i, j;
	for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

void append(char *s, char n) {
	int len = strlen(s);
	s[len] = n;
	s[len+1] = '\0';
}

void str_backspace(char *s) {
	int len = strlen(s);
	s[len-1] = '\0';
}

size_t strlen(const char *s) {
	size_t i = 0;
	while (s[i]) ++i;
	return i;
}

int strcmp(const char *s1, const char *s2) {
	int i;
	for (i = 0; s1[i] == s2[i]; i++) {
		if (s1[i] == '\0') return 0;
	}
	return s1[i] - s2[i];
}

int strncmp(const char *s1, const char *s2, size_t num) {
	int i;
	for (i = 0; (i < num) || (s1[i] == s2[i]); i++) {
		if (s1[i] == '\0') return 0;
	}
	return s1[i] - s2[i];
}

char *strcat(char* dst, const char *src) {
	size_t offset;

	offset = strlen(dst);
	strcpy(dst+offset, src);
	return dst;
}

char *strchr(const char *s, int ch_arg) {
	/* avoid sign extension problems */
	const char ch = ch_arg;
	/* scan from left to right */
	while (*s) {
		if (*s == ch)
			return (char *)s;
		s++;
	}
	/* if we were looking for the 0, return that */
	if (*s == ch)
		return (char *)s;
	/* else, didn't find it */
	return NULL;
}

char *strrchr(const char *s, int ch_arg) {
	/* avoid sign extension problems */
	const char ch = ch_arg;
	/* start one past the last char PLUS NULL TERMINATOR */
	size_t i = strlen(s)+1;
	/* go from right to left; stop at 0 */
	while (i > 0) {
		i--;
		if (s[i] == ch)
			return (char *)(s+i);
	}
	/* didn't find it */
	return NULL;
}

char *strcpy(char *dst, const char *src) {
	size_t i;
	/* copy characters until null terminator */
	for (i = 0; src[i]; i++)
		dst[i] = src[i];
	/* add null terminator */
	dst[i] = 0;
	return dst;
}

char *strtok_r(char *str, const char *seps, char **context) {
	char *head, *tail;
	/* if we're starting up, initialize context */
	if (str)
		*context = str;
	/* get potential start of this next word */
	head = *context;
	if (head == NULL)
		return NULL;
	/* skip any leading separators */
	while (*head && strchr(seps,*head))
		head++;
	/* did we hit end? */
	if (*head == 0) {
		*context = NULL;
		return NULL;
	}
	/* skip over word */
	tail = head;
	while (*tail && !strchr(seps,*tail))
		tail++;
	/* save head for next time in context */
	if (*tail == 0)
		*context = NULL;
	else {
		*tail = 0;
		tail++;
		*context = tail;
	}
	/* return current word */
	return head;
}