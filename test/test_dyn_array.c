#include <stdio.h>
#include <string.h>

#include "dyn_array.h"
#include "test_cases.h"

/*
A @DynArray with zero capacity can be instantiated and have entries pushed onto.
*/
static int test_zero_capacity() {
  struct DynArray *a = dyn_array_new(0);

  if (a->size != 0) {
    printf("a->size != 0\n");
    return 1;
  }
  if (a->_capacity != 1) {
    printf("a->_capacity != 1\n");
    return 1;
  }

  int *x = malloc(sizeof(int));
  dyn_array_push(a, x);
  if (a->size != 1) {
    printf("a->size != 1\n");
    return 1;
  }
  if (a->_capacity != 1) {
    printf("a->_capacity != 1\n");
    return 1;
  }

  int *y = malloc(sizeof(int));
  dyn_array_push(a, y);
  if (a->size != 2) {
    printf("a->size != 2\n");
    return 1;
  }
  if (a->_capacity != 2) {
    printf("a->_capacity != 2\n");
    return 1;
  }

  return 0;
}

/*
A @DynArray with nonzero capacity can be instantiated and have entries pushed
onto.
*/
static int test_nonzero_capacity() {
  struct DynArray *a = dyn_array_new(3);

  if (a->size != 0) {
    printf("a->size != 0\n");
    return 1;
  }
  if (a->_capacity != 3) {
    printf("a->_capacity != 3\n");
    return 1;
  }

  int *x = malloc(sizeof(int));
  int *y = malloc(sizeof(int));
  int *z = malloc(sizeof(int));
  dyn_array_push(a, x);
  dyn_array_push(a, y);
  dyn_array_push(a, z);
  if (a->size != 3) {
    printf("a->size != 3\n");
    return 1;
  }
  if (a->_capacity != 3) {
    printf("a->_capacity != 3\n");
    return 1;
  }

  int *w = malloc(sizeof(int));
  dyn_array_push(a, w);
  if (a->size != 4) {
    printf("a->size != 4\n");
    return 1;
  }
  if (a->_capacity != 6) {
    printf("a->_capacity != 6\n");
    return 1;
  }

  return 0;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Expected exactly one argument.");
    return 1;
  }

  struct TestCase cases[] = {
    {"zero-capacity", test_zero_capacity},
    {"nonzero-capacity", test_nonzero_capacity},
  };

  for (int i = 0; i < sizeof(cases) / sizeof(struct TestCase); ++i) {
    struct TestCase test_case = cases[i];
    if (strcmp(argv[1], test_case.name) == 0) {
      return test_case.func();
    }
  }

  return 1;
}
