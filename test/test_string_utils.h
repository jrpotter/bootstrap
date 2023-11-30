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

static void test_trim_leading() {
  char a1[] = {0};
  char a2[] = {' ', ' ', ' ', 0};
  trim_leading(a1);
  trim_leading(a2);
  sput_fail_unless(a1[0] == 0, "trim leading empty string");
  sput_fail_unless(strcmp(a1, a2) == 0, "trim leading whitespace string");

  char b1[] = {'a', 'b', 'c', 'd', 'e', 'f', 0};
  char b2[] = {' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 0};
  trim_leading(b1);
  trim_leading(b2);
  sput_fail_unless(strcmp(b1, b2) == 0, "trim leading string");

  char c1[] = {'a', 'b', 'c', 'd', 'e', 'f', ' ', ' ', ' ', 0};
  char c2[] = {'a', 'b', 'c', 'd', 'e', 'f', ' ', ' ', ' ', 0};
  trim_leading(c1);
  sput_fail_unless(strcmp(c1, c2) == 0, "trim leading ignore trailing");
}

static void test_trim_trailing() {
  char a1[] = {0};
  char a2[] = {' ', ' ', ' ', 0};
  trim_trailing(a1);
  trim_trailing(a2);
  sput_fail_unless(a1[0] == 0, "trim trailing empty string");
  sput_fail_unless(strcmp(a1, a2) == 0, "trim trailing whitespace string");

  char b1[] = {'a', 'b', 'c', 'd', 'e', 'f', 0};
  char b2[] = {'a', 'b', 'c', 'd', 'e', 'f', ' ', ' ', ' ', 0};
  trim_trailing(b1);
  trim_trailing(b2);
  sput_fail_unless(strcmp(b1, b2) == 0, "trim trailing string");

  char c1[] = {' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 0};
  char c2[] = {' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 0};
  trim_trailing(c1);
  sput_fail_unless(strcmp(c1, c2) == 0, "trim trailing ignore leading");
}

#endif /* _BOOTSTRAP_TEST_STRING_UTILS */
