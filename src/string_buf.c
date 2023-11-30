#include "string_buf.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

struct StringBuf *string_buf_new(size_t capacity) {
  struct StringBuf *sb = malloc(sizeof(struct StringBuf));
  sb->_capacity = (capacity == 0) ? 1 : capacity;
  sb->buf = malloc(sb->_capacity * sizeof(char));
  sb->buf[0] = 0;
  sb->size = 0;
  return sb;
}

size_t string_buf_size(struct StringBuf *sb) {
  assert(sb);
  return sb->size;
}

void string_buf_cappend(struct StringBuf *sb, char c) {
  assert(sb);

  if (sb->size + 1 + 1 > sb->_capacity) {  // size + NUL + c
    sb->_capacity *= 2;
    sb->buf = realloc((void *)sb->buf, sb->_capacity);
  }

  sb->buf[sb->size++] = c;
  sb->buf[sb->size] = 0;
}

void string_buf_sappend(struct StringBuf *sb, const char s[static 1]) {
  assert(sb);

  size_t slen = strlen(s);
  double goal = sb->size + 1 + slen;
  if (goal > sb->_capacity) {  // size + NUL + slen
    sb->_capacity *= pow(2, ceil(log2(goal / sb->_capacity)));
    sb->buf = realloc((void *)sb->buf, sb->_capacity);
  }

  for (const char *c = s; *c; ++c) {
    sb->buf[sb->size++] = *c;
  }
  sb->buf[sb->size] = 0;
}

const char *string_buf_cast(struct StringBuf *sb) {
  assert(sb);
  const char *buf = sb->buf;
  free(sb);
  return buf;
}

void string_buf_free(struct StringBuf *sb) {
  if (!sb) {
    return;
  }
  free((void *)sb->buf);
  free(sb);
  sb = 0;
}
