/**
@file
@brief String-related utility functions.
*/

#ifndef _BOOTSTRAP_STRING_UTILS_H
#define _BOOTSTRAP_STRING_UTILS_H

#include <stdlib.h>

/**
@brief Concatenate the list of segments into a Unix-style path.

This function assumes a forward slash path separator (i.e. `/`).

@param n
 The number of segments to join together.
@param segments
 The path components to join together.
@return
 The concatenation of the path components via `/`. The caller takes ownership
 of this value.
*/
char *join(size_t n, const char *segments[static n], char sep);

#endif /* _BOOTSTRAP_STRING_UTILS_H */
