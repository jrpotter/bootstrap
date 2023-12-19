#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "string_utils.h"
#include "test_cases.h"

struct TestConfigFixture {
  char *cwd;
  char *root_dir;
  char *target;
};

static struct TestConfigFixture *test_setup() {
  char *cwd = getcwd(0, 0);
  const char *segments[] = {cwd, "specs"};
  char *root_dir = join(sizeof(segments) / sizeof(char *), segments, '/');

  struct TestConfigFixture *fixture = malloc(sizeof(struct TestConfigFixture));
  fixture->cwd = getcwd(0, 0);
  fixture->root_dir = root_dir;
  fixture->target = "minimal_spec_json";

  return fixture;
}

static int test_new_invalid_args() {
  struct TestConfigFixture *fixture = test_setup();
  struct Error *error = 0;
  struct Config *config = 0;

  error = config_new(0, fixture->root_dir, fixture->target, &config);
  if (error->code != ERROR_CONFIG_ENV_CWD_INVALID) {
    printf("cwd == 0\n");
    return 1;
  }

  error_free(error);
  error = config_new(fixture->cwd, 0, fixture->target, &config);
  if (error->code != ERROR_CONFIG_ENV_ROOT_DIR_INVALID) {
    printf("root_dir == 0\n");
    return 1;
  }

  return 0;
}

static int test_new_spec_not_found() {
  struct TestConfigFixture *fixture = test_setup();
  struct Error *error = 0;
  struct Config *config = 0;

  error = config_new(fixture->cwd, fixture->root_dir, "not_found", &config);
  if (error->code != ERROR_CONFIG_TARGET_NOT_FOUND) {
    printf("target not found\n");
    return 1;
  }

  return 0;
}

static int test_new_spec_not_dir() {
  struct TestConfigFixture *fixture = test_setup();
  struct Error *error = 0;
  struct Config *config = 0;

  error = config_new(fixture->cwd, fixture->root_dir, "not_dir", &config);
  if (error->code != ERROR_CONFIG_TARGET_NOT_DIR) {
    printf("target not dir\n");
    return 1;
  }

  return 0;
}

static int test_new_success() {
  struct TestConfigFixture *fixture = test_setup();
  struct Error *error = 0;
  struct Config *config = 0;

  error = config_new(fixture->cwd, fixture->root_dir, fixture->target, &config);
  if (error != 0) {
    error_free(error);
    printf("config_new() success\n");
    return 1;
  } else if (strcmp(config->cwd, fixture->cwd) != 0) {
    printf("config_new() cwd\n");
    return 1;
  } else if (strcmp(config->root_dir, fixture->root_dir) != 0) {
    printf("config_new() root_dir\n");
    return 1;
  } else if (strcmp(config->target, fixture->target) != 0) {
    printf("config_new() target\n");
    return 1;
  }

  return 0;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Expected exactly one argument.");
    return 1;
  }

  struct TestCase cases[] = {
    {"new-invalid-args", test_new_invalid_args},
    {"new-spec-not-found", test_new_spec_not_found},
    {"new-spec-not-dir", test_new_spec_not_dir},
    {"new-success", test_new_success},
  };

  for (int i = 0; i < sizeof(cases) / sizeof(struct TestCase); ++i) {
    struct TestCase test_case = cases[i];
    if (strcmp(argv[1], test_case.name) == 0) {
      return test_case.func();
    }
  }

  return 1;
}
