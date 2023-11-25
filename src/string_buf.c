#include "string_buf.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

struct StringBuf *string_buf_new(size_t capacity) {
  struct StringBuf *sb = malloc(sizeof(struct StringBuf));
  sb->buf = calloc(capacity, sizeof(char));
  sb->size = 0;
  sb->_capacity = capacity;
  return sb;
}

size_t string_buf_size(struct StringBuf *sb) {
  assert(sb);
  return sb->size;
}

void string_buf_cappend(struct StringBuf *sb, char c) {
  assert(sb);

  if (sb->_capacity) {
    sb->_capacity *= 2;
  } else {
    sb->_capacity = 2;
  }

  sb->buf = realloc((void *)sb->buf, sb->_capacity);
  sb->buf[sb->size++] = c;
  sb->buf[sb->size] = 0;
}

void string_buf_sappend(struct StringBuf *sb, const char s[static 1]) {
  assert(sb);

  double goal = sb->size + strlen(s) + 1;
  double denom = sb->_capacity ? sb->_capacity : 1;
  double scale = pow(2, ceil(log2(goal / denom)));

  if (sb->_capacity) {
    sb->_capacity *= scale;
  } else {
    sb->_capacity = scale;
  }

  sb->buf = realloc((void *)sb->buf, sb->_capacity);
  for (const char *i = s; *i; ++i) {
    sb->buf[sb->size++] = *i;
  }
  sb->buf[sb->size] = 0;
}

const char *string_buf_convert(struct StringBuf *sb) {
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
