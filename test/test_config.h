#ifndef _SPEC_TEST_CONFIG
#define _SPEC_TEST_CONFIG

#include "config.h"
#include "sput.h"

static const char *SAMPLE_CWD = "/home/jrpotter/Documents/spec";
static const char *SAMPLE_ROOT_DIR = "/usr/local/share/specs";
static const char *SAMPLE_TARGET = "example-target";

static void test_config_load_cwd_invalid() {
  struct Config *config = 0;
  enum ConfigError retval =
      config_load(0, SAMPLE_ROOT_DIR, SAMPLE_TARGET, &config);
  sput_fail_unless(retval == CE_ENV_CWD_INVALID, "target == 0");
}

static void test_config_load_root_dir_invalid() {
  struct Config *config = 0;
  enum ConfigError retval = config_load(SAMPLE_CWD, 0, SAMPLE_TARGET, &config);
  sput_fail_unless(retval == CE_ENV_SPEC_ROOT_DIR_INVALID, "root_dir == 0");
}

static void test_config_load_target_invalid() {
  struct Config *config = 0;
  enum ConfigError retval =
      config_load(SAMPLE_CWD, SAMPLE_ROOT_DIR, 0, &config);
  sput_fail_unless(retval == CE_TARGET_INVALID, "target == 0");
}

static void test_config_load_success() {
  struct Config *config = 0;
  enum ConfigError retval =
      config_load(SAMPLE_CWD, SAMPLE_ROOT_DIR, SAMPLE_TARGET, &config);

  sput_fail_unless(retval == 0, "config_load() success");
  sput_fail_unless(strcmp(config->root_dir, SAMPLE_ROOT_DIR) == 0,
                   "config_load() root_dir");
  sput_fail_unless(strcmp(config->target, SAMPLE_TARGET) == 0,
                   "config_load() target");

  config_free(config);
}

#endif /* _SPEC_TEST_CONFIG */
