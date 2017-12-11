#include "string.h"

size_t strlen(const char *s)
{
    size_t len = 0;
    while (*s++ != '\0')
        ++len;
    return len;
}

size_t strlcpy(char *dst, const char *src, size_t size)
{
    size_t res = 0;
    for (size_t i = 0; i < (size-1); ++i)
    {
        if (*src == '\0')
            break;
        *dst++ = *src++;
        ++res;
    }
    *dst = '\0';

    return res + strlen(src);
}

size_t strlcat(char *dst, const char *src, size_t size)
{
    size_t len = strlen(dst);
    size_t res = strlcpy(dst + len, src, size - len);
    return len + res;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    uint8_t *d = (uint8_t*)dest;
    uint8_t *s = (uint8_t*)src;
    for (size_t i = 0; i < n; ++i)
        *d++ = *s++;
    return dest;
}

void *memset(void *s, int c, size_t n)
{
    uint8_t *ss = s;
    uint8_t cc = (uint8_t)c;
    for (size_t i = 0; i < n; ++i)
        *ss++ = cc;
    return s;
}
