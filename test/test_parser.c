#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "cJSON.h"
#include "config.h"
#include "parser.h"
#include "string_utils.h"
#include "test_cases.h"

struct TestParserFixture {
  char *cwd;
  char *root_dir;
  const char *target;
  struct Config config;
};

static struct TestParserFixture *test_setup(const char *target) {
  char *cwd = getcwd(0, 0);
  const char *segments[] = {cwd, "specs"};
  char *root_dir = join(sizeof(segments) / sizeof(char *), segments, '/');

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

/*
A missing `spec.json` file is not an error. Our parsed @cJSON instance should
be set to NULL in this case.
*/
static int test_missing() {
  struct TestParserFixture *fixture = test_setup("no_spec_json");
  struct Error *error = 0;

  cJSON *parsed = 0;
  error = parse_spec_json(&fixture->config, &parsed);
  if (error != 0) {
    printf("no spec.json, failure\n");
    return 1;
  }
  if (parsed != 0) {
    printf("no spec.json, parsed\n");
    return 1;
  }

  return 0;
}

static int test_minimal() {
  struct TestParserFixture *fixture = test_setup("minimal_spec_json");
  struct Error *error = 0;
  cJSON *parsed = 0;

  error = parse_spec_json(&fixture->config, &parsed);
  if (error != 0) {
    printf("minimal spec.json, failure\n");
    return 1;
  }
  if (parsed == 0) {
    printf("minimal spec.json, no parse\n");
    return 1;
  }

  return 0;
}

static int test_invalid() {
  struct TestParserFixture *fixture = test_setup("invalid_spec_json");
  struct Error *error = 0;
  cJSON *parsed = 0;

  error = parse_spec_json(&fixture->config, &parsed);
  if (error->code != ERROR_PARSER_SPEC_JSON_INVALID_SYNTAX) {
    printf("invalid spec, wrong code\n");
    return 1;
  }
  if (parsed != 0) {
    printf("invalid spec, parsed\n");
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
    {"missing", test_missing},
    {"minimal", test_minimal},
    {"invalid", test_invalid},
  };

  for (int i = 0; i < sizeof(cases) / sizeof(struct TestCase); ++i) {
    struct TestCase test_case = cases[i];
    if (strcmp(argv[1], test_case.name) == 0) {
      return test_case.func();
    }
  }

  return 1;
}
