#include <stdio.h>
#include <string.h>

#include "string_buf.h"
#include "test_cases.h"

static int test_sappend() {
  struct StringBuf *sb = string_buf_new(0);
  string_buf_sappend(sb, "hello world");
  string_buf_sappend(sb, "!!");

  if (string_buf_size(sb) != strlen("hello world!!")) {
    printf("sappend unexpected size\n");
    return 1;
  }

  const char *cast = string_buf_cast(sb);
  if (strcmp(cast, "hello world!!") != 0) {
    printf("sappend wrong cast\n");
    return 1;
  }

  return 0;
}

static int test_cappend() {
  struct StringBuf *sb = string_buf_new(0);
  string_buf_sappend(sb, "hello world");
  string_buf_cappend(sb, '!');
  string_buf_cappend(sb, '!');

  if (string_buf_size(sb) != strlen("hello world!!")) {
    printf("cappend wrong size\n");
    return 1;
  }

  const char *cast = string_buf_cast(sb);
  if (strcmp(cast, "hello world!!") != 0) {
    printf("cappend wrong cast\n");
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
    {"sappend", test_sappend},
    {"cappend", test_cappend},
  };

  for (int i = 0; i < sizeof(cases) / sizeof(struct TestCase); ++i) {
    struct TestCase test_case = cases[i];
    if (strcmp(argv[1], test_case.name) == 0) {
      return test_case.func();
    }
  }

  return 1;
}
