#ifndef _SPEC_TEST_CONFIG
#define _SPEC_TEST_CONFIG

#include "config.h"
#include "sput.h"

static void test_config_load_root_dir_invalid() {
  struct Config *config = 0;

  enum ConfigError retval = 0;

  retval = config_load(0, "target", &config);
  sput_fail_unless(retval == ENV_SPEC_ROOT_DIR_MISSING, "root_dir == 0");

  retval = config_load("", "target", &config);
  sput_fail_unless(retval == ENV_SPEC_ROOT_DIR_EMPTY, "root_dir == \"\"");
}

static void test_config_load_target_invalid() {
  struct Config *config = 0;
  enum ConfigError retval = 0;

  retval = config_load("/usr/local/share/specs", 0, &config);
  sput_fail_unless(retval == INVALID_TARGET, "target == 0");

  retval = config_load("/usr/local/share/specs", "", &config);
  sput_fail_unless(retval == INVALID_TARGET, "target == \"\"");
}

static void test_config_load_success() {
  struct Config *config = 0;
  enum ConfigError retval =
      config_load("/usr/local/share/specs", "target", &config);

  sput_fail_unless(retval == 0, "config_load() success");
  sput_fail_unless(strcmp(config->root_dir, "/usr/local/share/specs") == 0,
                   "config_load() root_dir");
  sput_fail_unless(strcmp(config->target, "target") == 0,
                   "config_load(0 target");

  config_free(config);
}

#endif /* _SPEC_TEST_CONFIG */
