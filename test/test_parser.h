#ifndef _BOOTSTRAP_TEST_PARSER
#define _BOOTSTRAP_TEST_PARSER

#include <unistd.h>

#include "cJSON.h"
#include "config.h"
#include "parser.h"
#include "path.h"
#include "sput.h"

/*
A missing `spec.json` file is not an error. Our parsed @cJSON instance should
be set to NULL in this case.
*/
static void test_parse_spec_json_missing() {
  char *cwd = getcwd(0, 0);

  const char *segments[] = {cwd, "test", "specs"};
  char *root_dir =
    join_path_segments(sizeof(segments) / sizeof(char *), segments);

  struct Config config = {
    cwd,
    root_dir,
    "no_spec_json",
  };

  cJSON *parsed = 0;
  enum SpecParseError retval = parse_spec_json(&config, &parsed);
  sput_fail_unless(retval == 0, "no spec.json, success");
  sput_fail_unless(parsed == 0, "no spec.json, no parsed");

  free(cwd);
  free(root_dir);
}

static void test_parse_spec_json_minimal() {
  char *cwd = getcwd(0, 0);

  const char *segments[] = {cwd, "test", "specs"};
  char *root_dir =
    join_path_segments(sizeof(segments) / sizeof(char *), segments);

  struct Config config = {
    cwd,
    root_dir,
    "minimal_spec_json",
  };

  cJSON *parsed = 0;
  enum SpecParseError retval = parse_spec_json(&config, &parsed);
  sput_fail_unless(retval == 0, "minimal spec.json, success");
  sput_fail_unless(parsed != 0, "minimal spec.json, parsed");

  free(cwd);
  free(root_dir);
}

static void test_parse_spec_json_invalid() {
  char *cwd = getcwd(0, 0);

  const char *segments[] = {cwd, "test", "specs"};
  char *root_dir =
    join_path_segments(sizeof(segments) / sizeof(char *), segments);

  struct Config config = {
    cwd,
    root_dir,
    "invalid_spec_json",
  };

  cJSON *parsed = 0;
  enum SpecParseError retval = parse_spec_json(&config, &parsed);
  sput_fail_unless(
    retval == SPE_INVALID_SYNTAX, "invalid spec.json, INVALID_SYNTAX"
  );
  sput_fail_unless(parsed == 0, "invalid spec.json, not parsed");

  free(cwd);
  free(root_dir);
}

#endif /* _BOOTSTRAP_TEST_PARSER */
