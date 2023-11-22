#ifndef GEN_FLAKE_DYN_ARRAY_H
#define GEN_FLAKE_DYN_ARRAY_H

#include <stdlib.h>

struct DynArray;

struct DynArray *dyn_array_new(size_t capacity);

size_t dyn_array_size(struct DynArray *a);

void dyn_array_push(struct DynArray *a, void *item);

void dyn_array_free(struct DynArray *a);

#endif /* GEN_FLAKE_DYN_ARRAY_H */
