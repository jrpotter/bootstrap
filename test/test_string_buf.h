#ifndef _BOOTSTRAP_TEST_STRING_BUF
#define _BOOTSTRAP_TEST_STRING_BUF

#include "sput.h"
#include "string_buf.h"

static void test_string_buf_sappend() {
  struct StringBuf *sb = string_buf_new(0);
  string_buf_sappend(sb, "hello world");
  string_buf_sappend(sb, "!!");

  sput_fail_unless(
    string_buf_size(sb) == strlen("hello world!!"), "sappend size"
  );
  const char *converted = string_buf_convert(sb);
  sput_fail_unless(
    strcmp(converted, "hello world!!") == 0, "sappend converted"
  );
}

static void test_string_buf_cappend() {
  struct StringBuf *sb = string_buf_new(0);
  string_buf_sappend(sb, "hello world");
  string_buf_cappend(sb, '!');
  string_buf_cappend(sb, '!');

  sput_fail_unless(
    string_buf_size(sb) == strlen("hello world!!"), "cappend size"
  );
  const char *converted = string_buf_convert(sb);
  sput_fail_unless(
    strcmp(converted, "hello world!!") == 0, "cappend converted"
  );
}

static void test_string_buf_nonzero_capacity() {
  struct StringBuf *sb = string_buf_new(100);
  string_buf_sappend(sb, "hello world");
  string_buf_cappend(sb, '!');
  string_buf_free(sb);
  sput_fail_unless(sb == 0, "free");
}

#endif /* _BOOTSTRAP_TEST_STRING_BUF */
