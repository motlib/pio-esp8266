#include "strutils.h"

#include <string.h>

char * strcopy(char * const dest, char const * const src, size_t len)
{
    strncpy(dest, src, len);
    
    /* ensure that the string is always terminated. */
    dest[len - 1] = '\0';

    return dest;
}
