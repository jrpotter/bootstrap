#include "dyn_array.h"

#include <assert.h>

struct DynArray *dyn_array_new(size_t capacity) {
  struct DynArray *a = malloc(sizeof(struct DynArray));
  a->_capacity = (capacity == 0) ? 1 : capacity;
  a->buf = malloc(a->_capacity * sizeof(void *));
  a->buf[0] = 0;
  a->size = 0;
  return a;
}

size_t dyn_array_size(struct DynArray *a) {
  assert(a);
  return a->size;
}

void dyn_array_push(struct DynArray *a, void *item) {
  assert(a);
  assert(item);
  if (a->size == a->_capacity) {
    a->_capacity *= 2;
    a->buf = realloc(a->buf, sizeof(void *) * a->_capacity);
  }
  a->buf[a->size] = item;
  a->size += 1;
}

void dyn_array_free(struct DynArray *a) {
  if (!a) {
    return;
  }
  for (int i = 0; i < a->size; ++i) {
    free(a->buf[i]);
  }
  free(a->buf);
  free(a);
  a = 0;
}
