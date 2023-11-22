#include <assert.h>

#include "dyn_array.h"

struct DynArray {
  void **buf;
  // The size of @buf excluding `NUL`.
  size_t size;
  // The allocated size of @buf including `NUL`.
  size_t _capacity;
};

struct DynArray *dyn_array_new(size_t capacity) {
  struct DynArray *a = malloc(sizeof(struct DynArray));
  a->buf = calloc(capacity, sizeof(void *));
  a->size = 0;
  a->_capacity = capacity;
  return a;
}

size_t dyn_array_size(struct DynArray *a) {
  assert(a);

  return a->size;
}

void dyn_array_push(struct DynArray *a, void *item) {
  assert(a);

  if (a->size == a->_capacity) {
    // We assume reallocation will work.
    a->_capacity *= 2;
    a = realloc(a, sizeof(void *) * a->_capacity);
  }
  a->size += 1;
  a->buf[a->size] = item;
}

void dyn_array_free(struct DynArray *a) {
  assert(a);

  free(a->buf);
  free(a);
}
