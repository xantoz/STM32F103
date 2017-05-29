/**
 * (re-)implement some string handling functions, as we're not using any libc.
 */

#ifndef _STRING_
#define _STRING_

#include "types.h"

size_t strlen(const char *s);

#endif
