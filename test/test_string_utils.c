#include <stdio.h>
#include <string.h>

#include "string_utils.h"
#include "test_cases.h"

static int test_join_single() {
  const char *segments[] = {"abc"};
  char *joined = join(sizeof(segments) / sizeof(char *), segments, '/');
  if (strcmp(joined, "abc") != 0) {
    printf("joined != abc\n");
    return 1;
  }
  return 0;
}

static int test_join_multiple() {
  const char *segments[] = {"abc", "def", "ghi"};
  char *joined = join(sizeof(segments) / sizeof(char *), segments, '/');
  if (strcmp(joined, "abc/def/ghi") != 0) {
    printf("joined != abc/def/ghi\n");
    return 1;
  }
  return 0;
}

static int test_strcmp_ci() {
  const char *a1 = "aBcD";
  const char *a2 = "AbCd";
  if (strcmp_ci(a1, a2) != 0) {
    printf("strcmp_ci != 0\n");
    return 1;
  }

  const char *b1 = "aBcDe";
  const char *b2 = "AbCd";
  if (strcmp_ci(b1, b2) <= 0) {
    printf("strcmp_ci <= 0\n");
    return 1;
  }
  if (strcmp_ci(b2, b1) >= 0) {
    printf("strcmp_ci >= 0\n");
    return 1;
  }

  return 0;
}

static int test_trim_leading() {
  char a1[] = {0};
  char a2[] = {' ', ' ', ' ', 0};
  trim_leading(a1);
  trim_leading(a2);
  if (a1[0] != 0) {
    printf("trim leading empty string\n");
    return 1;
  }
  if (strcmp(a1, a2) != 0) {
    printf("trim leading whitespace string");
    return 1;
  }

  char b1[] = {'a', 'b', 'c', 'd', 'e', 'f', 0};
  char b2[] = {' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 0};
  trim_leading(b1);
  trim_leading(b2);
  if (strcmp(b1, b2) != 0) {
    printf("trim leading string\n");
    return 1;
  }

  char c1[] = {'a', 'b', 'c', 'd', 'e', 'f', ' ', ' ', ' ', 0};
  char c2[] = {'a', 'b', 'c', 'd', 'e', 'f', ' ', ' ', ' ', 0};
  trim_leading(c1);
  if (strcmp(c1, c2) != 0) {
    printf("trim leading ignore trailing\n");
    return 1;
  }

  return 0;
}

static int test_trim_trailing() {
  char a1[] = {0};
  char a2[] = {' ', ' ', ' ', 0};
  trim_trailing(a1);
  trim_trailing(a2);
  if (a1[0] != 0) {
    printf("trim trailing empty string\n");
    return 1;
  }
  if (strcmp(a1, a2) != 0) {
    printf("trim trailing whitespace string\n");
    return 1;
  }

  char b1[] = {'a', 'b', 'c', 'd', 'e', 'f', 0};
  char b2[] = {'a', 'b', 'c', 'd', 'e', 'f', ' ', ' ', ' ', 0};
  trim_trailing(b1);
  trim_trailing(b2);
  if (strcmp(b1, b2) != 0) {
    printf("trim trailing string\n");
    return 1;
  }

  char c1[] = {' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 0};
  char c2[] = {' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 0};
  trim_trailing(c1);
  if (strcmp(c1, c2) != 0) {
    printf("trim trailing ignore leading\n");
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
    {"join-single", test_join_single},
    {"join-multiple", test_join_multiple},
    {"strcmp-ci", test_strcmp_ci},
    {"trim-leading", test_trim_leading},
    {"trim-trailing", test_trim_trailing},
  };

  for (int i = 0; i < sizeof(cases) / sizeof(struct TestCase); ++i) {
    struct TestCase test_case = cases[i];
    if (strcmp(argv[1], test_case.name) == 0) {
      return test_case.func();
    }
  }

  return 1;
}
