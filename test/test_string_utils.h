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

static void test_strcmp_ci() {
  const char *a1 = "aBcD";
  const char *a2 = "AbCd";
  sput_fail_unless(strcmp_ci(a1, a2) == 0, "strcmp_ci == 0");
  const char *b1 = "aBcDe";
  const char *b2 = "AbCd";
  sput_fail_unless(strcmp_ci(b1, b2) > 0, "strcmp_ci > 0");
  sput_fail_unless(strcmp_ci(b2, b1) < 0, "strcmp_ci < 0");
}

#endif /* _BOOTSTRAP_TEST_STRING_UTILS */
