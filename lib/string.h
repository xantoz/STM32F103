/**
 * (re-)implement some string handling functions, as we're not using any libc.
 */

#ifndef _STRING_
#define _STRING_

#include "types.h"

size_t strlen(const char *s);

size_t strlcpy(char *dst, const char *src, size_t size);
size_t strlcat(char *dst, const char *src, size_t size);

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);

#endif
