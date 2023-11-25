#ifndef _BOOTSTRAP_TEST_PATH
#define _BOOTSTRAP_TEST_PATH

#include "path.h"
#include "sput.h"

static void test_join_path_single_segments() {
  const char *segments[] = {"abc"};
  char *joined =
    join_path_segments(sizeof(segments) / sizeof(char *), segments);
  sput_fail_unless(strcmp(joined, "abc") == 0, "abc");
  free(joined);
}

static void test_join_path_multiple_segments() {
  const char *segments[] = {"abc", "def", "ghi"};
  char *joined =
    join_path_segments(sizeof(segments) / sizeof(char *), segments);
  sput_fail_unless(strcmp(joined, "abc/def/ghi") == 0, "abc/def/ghi");
  free(joined);
}

#endif /* _BOOTSTRAP_TEST_PATH */
