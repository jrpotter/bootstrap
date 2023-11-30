#include "string_utils.h"

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

char *join(size_t n, const char *segments[static n], char sep) {
  assert(n > 0);

  size_t length = 0;
  for (int i = 0; i < n; ++i) {
    length += strlen(segments[i]);
  }
  length += n - 1;  // Path separators.

  size_t offset = 0;
  char *joined = calloc(1, length + 1);

  for (int i = 0; i < n; ++i) {
    const size_t segment_len = strlen(segments[i]);
    memcpy(joined + offset, segments[i], segment_len);
    offset += segment_len;
    if (i < n - 1) {
      joined[offset++] = sep;
    }
  }

  return joined;
}

int strcmp_ci(const char *s1, const char *s2) {
  int i = 0;
  size_t s1_len = strlen(s1);
  size_t s2_len = strlen(s2);
  for (; i < (s1_len < s2_len ? s1_len : s2_len); ++i) {
    if (toupper(s1[i]) < toupper(s2[i])) {
      return -1;
    } else if (toupper(s1[i]) > toupper(s2[i])) {
      return 1;
    }
  }
  if (s1_len == s2_len) {
    return 0;
  }
  return s1_len < s2_len ? -1 : 1;
}

void trim_leading(char *s) {
  int count = 0;
  for (const char *c = s; isspace(*c); ++c) {
    count++;
  }
  if (count == 0) {
    return;
  }
  // Shift elements down.
  size_t len = strlen(s);
  for (int i = 0; i < len - count + 1; ++i) {
    s[i] = s[i + count];
  }
}

void trim_trailing(char *s) {
  int last = -1;
  size_t len = strlen(s);
  for (int i = 0; i < len; ++i) {
    if (!isspace(s[i])) {
      last = i;
    }
  }
  if (last == -1) {
    s[0] = 0;
  } else if (last < len - 1) {
    s[last + 1] = 0;
  }
}
