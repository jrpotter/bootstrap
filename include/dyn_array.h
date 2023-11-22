#ifndef _SPEC_DYN_ARRAY_H
#define _SPEC_DYN_ARRAY_H

#include <stdlib.h>

struct DynArray {
  void **buf;
  // The size of @buf excluding `NUL`.
  size_t size;
  // The allocated size of @buf including `NUL`.
  size_t _capacity;
};

struct DynArray *dyn_array_new(size_t capacity);

size_t dyn_array_size(struct DynArray *a);

void dyn_array_push(struct DynArray *a, void *item);

void dyn_array_free(struct DynArray *a);

#endif /* _SPEC_DYN_ARRAY_H */
