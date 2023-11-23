#ifndef _SPEC_TEST_CONFIG
#define _SPEC_TEST_CONFIG

#include "config.h"
#include "sput.h"

static void test_load_config_root_dir_missing() {
  struct Config config;
  enum ConfigError retval = load_config(0, "target", &config);

  sput_fail_unless(retval == ENV_SPEC_ROOT_DIR_MISSING, "root_dir == 0");
}

static void test_load_config_root_dir_empty() {
  struct Config config;
  enum ConfigError retval = load_config("", "target", &config);

  sput_fail_unless(retval == ENV_SPEC_ROOT_DIR_EMPTY, "root_dir == \"\"");
}

static void test_config_run() {
  sput_enter_suite("load_config()");
  sput_run_test(test_load_config_root_dir_missing);
  sput_run_test(test_load_config_root_dir_empty);
}

#endif /* _SPEC_TEST_CONFIG */
