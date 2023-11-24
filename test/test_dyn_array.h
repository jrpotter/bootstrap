#ifndef _BOOTSTRAP_TEST_DYN_ARRAY
#define _BOOTSTRAP_TEST_DYN_ARRAY

#include "dyn_array.h"
#include "sput.h"

/*
A @DynArray with zero capacity can be instantiated and have entries pushed onto.
*/
static void test_dyn_array_zero_capacity() {
  struct DynArray *a = dyn_array_new(0);
  sput_fail_unless(a->size == 0, "a->size == 0");
  sput_fail_unless(a->_capacity == 1, "a->_capacity == 1");

  int *x = malloc(sizeof(int));
  dyn_array_push(a, x);
  sput_fail_unless(a->size == 1, "a->size == 1");
  sput_fail_unless(a->_capacity == 1, "a->_capacity == 1");

  int *y = malloc(sizeof(int));
  dyn_array_push(a, y);
  sput_fail_unless(a->size == 2, "a->size == 2");
  sput_fail_unless(a->_capacity == 2, "a->_capacity == 2");

  dyn_array_free(a);
}

/*
A @DynArray with nonzero capacity can be instantiated and have entries pushed
onto.
*/
static void test_dyn_array_nonzero_capacity() {
  struct DynArray *a = dyn_array_new(3);
  sput_fail_unless(a->size == 0, "a->size == 0");
  sput_fail_unless(a->_capacity == 3, "a->_capacity == 3");

  int *x = malloc(sizeof(int));
  int *y = malloc(sizeof(int));
  int *z = malloc(sizeof(int));
  dyn_array_push(a, x);
  dyn_array_push(a, y);
  dyn_array_push(a, z);
  sput_fail_unless(a->size == 3, "a->size == 3");
  sput_fail_unless(a->_capacity == 3, "a->_capacity == 3");

  int *w = malloc(sizeof(int));
  dyn_array_push(a, w);
  sput_fail_unless(a->size == 4, "a->size == 4");
  sput_fail_unless(a->_capacity == 6, "a->_capacity == 6");

  dyn_array_free(a);
}

#endif /* _BOOTSTRAP_TEST_DYN_ARRAY */
