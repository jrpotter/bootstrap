#ifndef _BOOTSTRAP_STRING_BUF_H
#define _BOOTSTRAP_STRING_BUF_H

#include <stdlib.h>

/**
 * @brief A dynamic character array.
 *
 * A `char*` wrapper. Appending `char`s or NUL-terminated strings allocates
 * additional space as needed.
 */
struct StringBuf {
  char *buf;
  // The length of @buf excluding `NUL`.
  size_t size;
  // The allocated size of @buf including `NUL`.
  size_t _capacity;
};

/**
 * Create a new `StringBuf` instance.
 *
 * @param capacity
 *  The initial size of the internal array (including the trailing `NUL`
 *  character). To avoid too many reallocations, aim to make this value large
 *  enough to accommodate the size the string is expected to eventually take.
 * @return
 *  A new `StringBuf` instance. The caller takes ownership of this value.
 *
 * @see string_buf_free
 */
struct StringBuf *string_buf_new(size_t capacity);

/**
 * Return the number of characters contained in the internal buffer.
 *
 * @param sb
 *  A valid pointer to a `StringBuf` instance.
 * @return
 *  The number of characters contained in the internal buffer.
 */
size_t string_buf_size(struct StringBuf *sb);

/**
 * Return the internal `NUL`-terminated string buffer.
 *
 * @param sb
 *  A valid pointer to a `StringBuf` instance.
 * @return
 *  The internally managed string.
 */
const char *string_buf_value(struct StringBuf *sb);

/**
 * Append a character to the end of a `StringBuf`.
 *
 * If appending would cause the internal buffer to overflow, reallocates the
 * internal array to accommodate.
 *
 * @param sb
 *  A valid pointer to a `StringBuf` instance.
 * @param c
 *  The `char` to append to the end of `sb`.
 */
void string_buf_cappend(struct StringBuf *sb, char c);

/**
 * Append a `NUL`-terminated string to the end of a `StringBuf`.
 *
 * If appending would cause the internal buffer to overflow, `realloc`s are
 * performed internally to accommodate.
 *
 * @param sb
 *  A valid pointer to a `StringBuf` instance.
 * @param s
 *  The `char*` to append to the end of `sb`.
 */
void string_buf_sappend(struct StringBuf *sb, const char s[static 1]);

/**
 * Convert a `StringBuf` instance into a `char*`.
 *
 * @param sb
 *  A valid pointer to a `StringBuf` instance.
 * @return
 *  A null pointer if `sb` is null. Otherwise a `NUL`-terminated string
 *  corresponding to the value of `sb`. The caller takes ownership of this
 *  value.
 */
const char *string_buf_convert(struct StringBuf *sb);

/**
 * Deallocate a previously allocated `StringBuf` instance.
 *
 * @param sb
 *  A valid pointer to a `StringBuf` instance.
 *
 * @see string_buf_new
 */
void string_buf_free(struct StringBuf *sb);

#endif /* _BOOTSTRAP_STRING_BUF_H */
