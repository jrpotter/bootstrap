/**
@file
@brief Dynamic `void*` arrays.
*/
#ifndef _BOOTSTRAP_DYN_ARRAY_H
#define _BOOTSTRAP_DYN_ARRAY_H

#include <stdlib.h>

/**
@brief A dynamic array of generic pointers.

A `void*` wrapper that grows larger as needed. If more space is needed during an
append operation, the capacity of the internal array is doubled.
*/
struct DynArray {
  /// The underlying `void*` pointer.
  void **buf;
  /// The size of @ref DynArray.buf.
  size_t size;
  // The allocated size of @ref DynArray.buf.
  size_t _capacity;
};

/**
@brief Create a new @ref DynArray instance.

@param capacity
 The initial size of the internal array. To avoid too many reallocations, aim to
 make this value large enough to accommodate the eventual size of the buffer.

@see dyn_array_free
*/
struct DynArray *dyn_array_new(size_t capacity);

/**
@brief Returns the number of items contained in the internal buffer.

@param a
 A valid pointer to a @ref DynArray instance.
@return
  The number of items contained in the internal buffer.
*/
size_t dyn_array_size(struct DynArray *a);

/**
@brief Appends a new item onto the end of the internal @ref DynArray.buf.

This function takes ownership of @p item and will attempt to `free` the
parameter when the @ref DynArray is `free`'d. For this reason, only provide
entries that have been allocated on the heap.

@param a
 A valid pointer to a @ref DynArray instance.
@param item
 A valid pointer to a heap-allocated object.

@see dyn_array_free
*/
void dyn_array_push(struct DynArray *a, void *item);

/**
@brief Deallocates a previously allocated @ref DynArray instance.

@param a
 A pointer to a @ref DynArray instance. If null, this function is a no-op.

@see dyn_array_new
*/
void dyn_array_free(struct DynArray *a);

#endif /* _BOOTSTRAP_DYN_ARRAY_H */
