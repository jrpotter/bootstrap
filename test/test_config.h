#ifndef _BOOTSTRAP_TEST_CONFIG
#define _BOOTSTRAP_TEST_CONFIG

#include <unistd.h>

#include "config.h"
#include "path.h"
#include "sput.h"

struct TestConfigFixture {
  char *cwd;
  char *root_dir;
  char *target;
};

static struct TestConfigFixture *test_config_setup() {
  char *cwd = getcwd(0, 0);
  const char *segments[] = {cwd, "test", "specs"};
  char *root_dir =
    join_path_segments(sizeof(segments) / sizeof(char *), segments);

  struct TestConfigFixture *fixture = malloc(sizeof(struct TestConfigFixture));
  fixture->cwd = getcwd(0, 0);
  fixture->root_dir = root_dir;
  fixture->target = "minimal_spec_json";

  return fixture;
}

static void test_config_teardown(struct TestConfigFixture *fixture) {
  free(fixture->cwd);
  free(fixture->root_dir);
  free(fixture);
}

static void test_config_load_invalid_args() {
  struct TestConfigFixture *fixture = test_config_setup();

  struct Config *config = 0;
  enum ConfigError retval = 0;

  retval = config_load(0, fixture->root_dir, fixture->target, &config);
  sput_fail_unless(retval == CE_ENV_CWD_INVALID, "target == 0");
  retval = config_load(fixture->cwd, 0, fixture->target, &config);
  sput_fail_unless(retval == CE_ENV_ROOT_DIR_INVALID, "root_dir == 0");
  retval = config_load(fixture->cwd, fixture->root_dir, 0, &config);
  sput_fail_unless(retval == CE_TARGET_INVALID, "target == 0");

  test_config_teardown(fixture);
}

static void test_config_load_spec_not_found() {
  struct TestConfigFixture *fixture = test_config_setup();

  struct Config *config = 0;
  enum ConfigError retval =
    config_load(fixture->cwd, fixture->root_dir, "not_found", &config);
  sput_fail_unless(retval == CE_TARGET_NOT_FOUND, "target not found");

  test_config_teardown(fixture);
}

static void test_config_load_spec_not_dir() {
  struct TestConfigFixture *fixture = test_config_setup();

  struct Config *config = 0;
  enum ConfigError retval =
    config_load(fixture->cwd, fixture->root_dir, "not_dir", &config);
  sput_fail_unless(retval == CE_TARGET_NOT_DIR, "target not dir");

  test_config_teardown(fixture);
}

static void test_config_load_success() {
  struct TestConfigFixture *fixture = test_config_setup();

  struct Config *config = 0;
  enum ConfigError retval =
    config_load(fixture->cwd, fixture->root_dir, fixture->target, &config);

  sput_fail_unless(retval == 0, "config_load() success");
  sput_fail_unless(strcmp(config->cwd, fixture->cwd) == 0, "config_load() cwd");
  sput_fail_unless(
    strcmp(config->root_dir, fixture->root_dir) == 0, "config_load() root_dir"
  );
  sput_fail_unless(
    strcmp(config->target, fixture->target) == 0, "config_load() target"
  );

  config_free(config);
  test_config_teardown(fixture);
}

#endif /* _BOOTSTRAP_TEST_CONFIG */
