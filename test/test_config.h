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

static void test_config_new_invalid_args() {
  struct TestConfigFixture *fixture = test_config_setup();

  struct Error *error = 0;
  struct Config *config = 0;

  error = config_new(0, fixture->root_dir, fixture->target, &config);
  sput_fail_unless(error->code == ERROR_CONFIG_ENV_CWD_INVALID, "cwd == 0");
  error_free(error);
  error = config_new(fixture->cwd, 0, fixture->target, &config);
  sput_fail_unless(
    error->code == ERROR_CONFIG_ENV_ROOT_DIR_INVALID, "root_dir == 0"
  );
  error_free(error);

  test_config_teardown(fixture);
}

static void test_config_new_spec_not_found() {
  struct TestConfigFixture *fixture = test_config_setup();

  struct Error *error = 0;
  struct Config *config = 0;

  error = config_new(fixture->cwd, fixture->root_dir, "not_found", &config);
  sput_fail_unless(
    error->code == ERROR_CONFIG_TARGET_NOT_FOUND, "target not found"
  );
  error_free(error);

  test_config_teardown(fixture);
}

static void test_config_new_spec_not_dir() {
  struct TestConfigFixture *fixture = test_config_setup();

  struct Error *error = 0;
  struct Config *config = 0;

  error = config_new(fixture->cwd, fixture->root_dir, "not_dir", &config);
  sput_fail_unless(
    error->code == ERROR_CONFIG_TARGET_NOT_DIR, "target not dir"
  );
  error_free(error);

  test_config_teardown(fixture);
}

static void test_config_new_success() {
  struct TestConfigFixture *fixture = test_config_setup();

  struct Error *error = 0;
  struct Config *config = 0;

  error = config_new(fixture->cwd, fixture->root_dir, fixture->target, &config);
  sput_fail_unless(error == 0, "config_new() success");
  sput_fail_unless(strcmp(config->cwd, fixture->cwd) == 0, "config_new() cwd");
  sput_fail_unless(
    strcmp(config->root_dir, fixture->root_dir) == 0, "config_new() root_dir"
  );
  sput_fail_unless(
    strcmp(config->target, fixture->target) == 0, "config_new() target"
  );

  config_free(config);
  test_config_teardown(fixture);
}

#endif /* _BOOTSTRAP_TEST_CONFIG */
