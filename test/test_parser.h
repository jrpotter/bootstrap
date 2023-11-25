#ifndef _BOOTSTRAP_TEST_PARSER
#define _BOOTSTRAP_TEST_PARSER

#include <unistd.h>

#include "cJSON.h"
#include "config.h"
#include "parser.h"
#include "path.h"
#include "sput.h"

struct TestParserFixture {
  char *cwd;
  char *root_dir;
  const char *target;
  struct Config config;
};

static struct TestParserFixture *test_parser_setup(const char *target) {
  char *cwd = getcwd(0, 0);
  const char *segments[] = {cwd, "test", "specs"};
  char *root_dir =
    join_path_segments(sizeof(segments) / sizeof(char *), segments);

  struct TestParserFixture *fixture = malloc(sizeof(struct TestParserFixture));
  fixture->cwd = getcwd(0, 0);
  fixture->root_dir = root_dir;
  fixture->target = target;

  // Reproduce in `Config` instance for convenience.
  fixture->config.cwd = fixture->cwd;
  fixture->config.root_dir = fixture->root_dir;
  fixture->config.target = fixture->target;

  return fixture;
}

static void test_parser_teardown(struct TestParserFixture *fixture) {
  free(fixture->cwd);
  free(fixture->root_dir);
  free(fixture);
}

/*
A missing `spec.json` file is not an error. Our parsed @cJSON instance should
be set to NULL in this case.
*/
static void test_parser_missing() {
  struct TestParserFixture *fixture = test_parser_setup("no_spec_json");

  cJSON *parsed = 0;
  struct Error *error = parse_spec_json(&fixture->config, &parsed);
  sput_fail_unless(error == 0, "no spec.json, success");
  sput_fail_unless(parsed == 0, "no spec.json, no parsed");

  test_parser_teardown(fixture);
}

static void test_parser_minimal() {
  struct TestParserFixture *fixture = test_parser_setup("minimal_spec_json");

  cJSON *parsed = 0;
  struct Error *error = parse_spec_json(&fixture->config, &parsed);
  sput_fail_unless(error == 0, "minimal spec.json, success");
  sput_fail_unless(parsed != 0, "minimal spec.json, parsed");

  test_parser_teardown(fixture);
}

static void test_parser_invalid() {
  struct TestParserFixture *fixture = test_parser_setup("invalid_spec_json");

  cJSON *parsed = 0;
  struct Error *error = parse_spec_json(&fixture->config, &parsed);
  sput_fail_unless(
    error->code == ERROR_PARSER_SPEC_JSON_INVALID_SYNTAX,
    "invalid spec.json, INVALID_SYNTAX"
  );
  error_free(error);
  sput_fail_unless(parsed == 0, "invalid spec.json, not parsed");

  test_parser_teardown(fixture);
}

#endif /* _BOOTSTRAP_TEST_PARSER */
