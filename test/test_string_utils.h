#ifndef _BOOTSTRAP_TEST_STRING_UTILS
#define _BOOTSTRAP_TEST_STRING_UTILS

#include "sput.h"
#include "string_utils.h"

static void test_join_single() {
  const char *segments[] = {"abc"};
  char *joined = join(sizeof(segments) / sizeof(char *), segments, '/');
  sput_fail_unless(strcmp(joined, "abc") == 0, "abc");
  free(joined);
}

static void test_join_multiple() {
  const char *segments[] = {"abc", "def", "ghi"};
  char *joined = join(sizeof(segments) / sizeof(char *), segments, '/');
  sput_fail_unless(strcmp(joined, "abc/def/ghi") == 0, "abc/def/ghi");
  free(joined);
}

#endif /* _BOOTSTRAP_TEST_STRING_UTILS */
