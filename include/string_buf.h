/**
@file
@brief Dynamic character arrays.
*/
#ifndef _BOOTSTRAP_STRING_BUF_H
#define _BOOTSTRAP_STRING_BUF_H

#include <stdlib.h>

/**
@brief A dynamic character array.

A `char*` wrapper that grows larger as needed. Supports appending individual
`char`s or NUL-terminated strings. If more space is needed during an append
operation, the capacity is doubled repeatedly until the requested data fits.
*/
struct StringBuf {
  /// The underlying `char*` pointer.
  char *buf;
  /// The size of @ref StringBuf.buf (excluding the `NUL` character).
  size_t size;
  /// The allocated size of @ref StringBuf.buf (including the `NUL` character).
  size_t _capacity;
};

/**
@brief Create a new @ref StringBuf instance.

@param capacity
 The initial size of the internal array (including the trailing `NUL`
 character). To avoid too many reallocations, aim to make this value large
 enough to accommodate the eventual size of the desired string.
@return
 A new @ref StringBuf instance. The caller takes ownership of this value.

@see string_buf_free
*/
struct StringBuf *string_buf_new(size_t capacity);

/**
@brief Returns the number of characters contained in the internal buffer
(excluding the NUL character).

@param sb
 A valid pointer to a @ref StringBuf instance.
@return
 The number of characters contained in the internal buffer.
*/
size_t string_buf_size(struct StringBuf *sb);

/**
@brief Appends a character to the end of the provided @ref StringBuf.

If appending would cause the internal buffer to overflow, doubles the capacity
of the internal array to accommodate.

@param sb
 A valid pointer to a @ref StringBuf instance.
@param c
 The `char` to append to the end of @p sb.
*/
void string_buf_cappend(struct StringBuf *sb, char c);

/**
@brief Appends a NUL-terminated string to the end of a @ref StringBuf.

If appending would cause the internal buffer to overflow, doubles the capacity
of the internal array the necessary number of times to accommodate.

@param sb
 A valid pointer to a @ref StringBuf instance.
@param s
 The `char*` to append to the end of @p sb.
*/
void string_buf_sappend(struct StringBuf *sb, const char s[static 1]);

/**
@brief Casts a @ref StringBuf instance into a `char*`.

This function frees the memory associated with @p sb.

@param sb
 A valid pointer to a @ref StringBuf instance.
@return
 A null pointer if @p sb is null. Otherwise a NUL-terminated string
 corresponding to the value of @p sb. The caller takes ownership of this value.
*/
const char *string_buf_cast(struct StringBuf *sb);

/**
@brief Deallocates a previously allocated @ref StringBuf instance.

@param sb
 A pointer to a @ref StringBuf instance. If null, this function is a no-op.

@see string_buf_new
*/
void string_buf_free(struct StringBuf *sb);

#endif /* _BOOTSTRAP_STRING_BUF_H */
