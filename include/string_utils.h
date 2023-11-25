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

/**
@brief Perform case insensitive string comparison.

This function operates like `strcmp` except comparison ignores case.

@param s1
 C string to be compared.
@param s2
 C string to be compared.
@return
 An integral value indicating the relationship between the strings:
 - `< 0`: the first character (converted uppercase) that does not match has a
   lower value in @p s1 than in @p s2.
 - `= 0`: the contents of both strings are equal.
 - `> 0`: the first character (converted uppercase) that does not match has a
   greater value in @p s1 than in @p s2.
*/
int strcmp_ci(const char *s1, const char *s2);

#endif /* _BOOTSTRAP_STRING_UTILS_H */
