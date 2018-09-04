#ifndef STRUTILS_H
#define STRUTILS_H

#include <stddef.h>

/**
 * Copy the src string to dest, writing a maximum of len charaters.
 *
 * Different to strncpy, the function ensures that the string in dest is alway
 * zero-terminated.
 */
char * strcopy(char * const dest, char const * const src, size_t len);

#endif /* STRUTILS_H */
