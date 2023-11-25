/**
@file
@brief Path-related utility functions.
*/
#ifndef _BOOTSTRAP_PATH_H
#define _BOOTSTRAP_PATH_H

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
char *join_path_segments(size_t n, const char *segments[static n]);

#endif /* _BOOTSTRAP_PATH_H */
