#include "path.h"

#include <assert.h>
#include <string.h>

char *join_path_segments(size_t n, const char *segments[static n]) {
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
      joined[offset++] = '/';
    }
  }

  return joined;
}
