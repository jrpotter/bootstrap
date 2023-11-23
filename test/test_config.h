#ifndef _BOOTSTRAP_TEST_CONFIG
#define _BOOTSTRAP_TEST_CONFIG

#include "config.h"
#include "sput.h"

static const char *SAMPLE_CWD = "/home/jrpotter/Documents/bootstrap";
static const char *SAMPLE_ROOT_DIR = "/usr/local/share/specs";
static const char *SAMPLE_TARGET = "example-target";

/*
A valid $CWD environment variable value must be supplied.
*/
static void test_config_load_cwd_invalid() {
  struct Config *config = 0;
  enum ConfigError retval =
      config_load(0, SAMPLE_ROOT_DIR, SAMPLE_TARGET, &config);
  sput_fail_unless(retval == CE_ENV_CWD_INVALID, "target == 0");
}

/*
A valid $BOOTSTRAP_ROOT_DIR environment variable value must be supplied.
*/
static void test_config_load_root_dir_invalid() {
  struct Config *config = 0;
  enum ConfigError retval = config_load(SAMPLE_CWD, 0, SAMPLE_TARGET, &config);
  sput_fail_unless(retval == CE_ENV_ROOT_DIR_INVALID, "root_dir == 0");
}

/*
A valid target value must be supplied.
*/
static void test_config_load_target_invalid() {
  struct Config *config = 0;
  enum ConfigError retval =
      config_load(SAMPLE_CWD, SAMPLE_ROOT_DIR, 0, &config);
  sput_fail_unless(retval == CE_TARGET_INVALID, "target == 0");
}

/*
If the supplied arguments to `config_load` are non-NULL, we should be able to
successfully construct a new @Config instance.
*/
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

#endif /* _BOOTSTRAP_TEST_CONFIG */
