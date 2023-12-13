#ifndef _BOOTSTRAP_TEST_VALIDATOR
#define _BOOTSTRAP_TEST_VALIDATOR

#include <unistd.h>

#include "dyn_array.h"
#include "sput.h"
#include "string_utils.h"
#include "validator.h"

struct TestValidatorFixture {
  const char *json;
  struct DynArray *prompts;
  cJSON *parsed;
  struct Config config;
};

static struct TestValidatorFixture *test_validator_setup(const char *json) {
  struct TestValidatorFixture *fixture =
    malloc(sizeof(struct TestValidatorFixture));
  fixture->json = json;
  fixture->prompts = 0;
  fixture->parsed = cJSON_Parse(json);

  char *cwd = getcwd(0, 0);
  const char *segments[] = {cwd, "test", "specs"};
  char *root_dir = join(sizeof(segments) / sizeof(char *), segments, '/');

  fixture->config.cwd = cwd;
  fixture->config.root_dir = root_dir;
  fixture->config.target = "minimal_spec_json";

  return fixture;
}

static void test_validator_teardown(struct TestValidatorFixture *fixture) {
  if (fixture->parsed) {
    cJSON_Delete(fixture->parsed);
  }
  free((void *)fixture->config.cwd);
  free((void *)fixture->config.root_dir);
  free(fixture);
}

static void test_validator_toplevel_not_object() {
  struct TestValidatorFixture *fixture = test_validator_setup("[]");

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  sput_fail_unless(
    error->code == ERROR_VALIDATOR_TOP_LEVEL_NOT_OBJECT, "top-level not object"
  );
  error_free(error);

  test_validator_teardown(fixture);
}

static void test_validator_field_not_object() {
  struct TestValidatorFixture *fixture =
    test_validator_setup("{\"key\": \"$UNKNOWN\"}");

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  sput_fail_unless(
    error->code == ERROR_VALIDATOR_FIELD_NOT_OBJECT, "field not object"
  );
  error_free(error);

  test_validator_teardown(fixture);
}

static void test_validator_field_name_leading_digit() {
  struct TestValidatorFixture *fixture = test_validator_setup(
    "{"
    "  \"1abc\": {"
    "    \"type\": \"line\""
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  sput_fail_unless(
    error->code == ERROR_VALIDATOR_FIELD_NAME_INVALID,
    "field name leading digit"
  );
  error_free(error);

  test_validator_teardown(fixture);
}

static void test_validator_field_name_non_alnum() {
  struct TestValidatorFixture *fixture = test_validator_setup(
    "{"
    "  \"a~bc\": {"
    "    \"type\": \"line\""
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  sput_fail_unless(
    error->code == ERROR_VALIDATOR_FIELD_NAME_INVALID, "field name non alnum"
  );
  error_free(error);

  test_validator_teardown(fixture);
}

static void test_validator_field_type_invalid() {
  struct TestValidatorFixture *fixture = test_validator_setup(
    "{"
    "  \"key\": {"
    "    \"type\": 2"
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  sput_fail_unless(
    error->code == ERROR_VALIDATOR_FIELD_TYPE_INVALID, "field type invalid"
  );
  error_free(error);

  test_validator_teardown(fixture);
}

static void test_validator_field_type_unknown() {
  struct TestValidatorFixture *fixture = test_validator_setup(
    "{"
    "  \"key\": {"
    "    \"type\": \"UNKNOWN\""
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  sput_fail_unless(
    error->code == ERROR_VALIDATOR_FIELD_TYPE_UNKNOWN, "field type unknown"
  );
  error_free(error);

  test_validator_teardown(fixture);
}

static void test_validator_valid_type_ci() {
  struct TestValidatorFixture *fixture = test_validator_setup(
    "{"
    "  \"key\": {"
    "    \"type\": \"LiNe\","
    "    \"prompt\": \"What value for key?\""
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  sput_fail_unless(error == 0, "valid");

  test_validator_teardown(fixture);
}

static void test_validator_field_required_invalid() {
  struct TestValidatorFixture *fixture = test_validator_setup(
    "{"
    "  \"key\": {"
    "    \"type\": \"line\","
    "    \"required\": 5"
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  sput_fail_unless(
    error->code == ERROR_VALIDATOR_FIELD_REQUIRED_INVALID,
    "field required invalid"
  );
  error_free(error);

  test_validator_teardown(fixture);
}

static void test_validator_field_required_valid() {
  struct TestValidatorFixture *fixture = test_validator_setup(
    "{"
    "  \"key\": {"
    "    \"type\": \"line\","
    "    \"required\": true,"
    "    \"prompt\": \"What value for key? \""
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  sput_fail_unless(error == 0, "required valid");
  error_free(error);

  test_validator_teardown(fixture);
}

static void test_validator_field_prompt_invalid() {
  struct TestValidatorFixture *fixture = test_validator_setup(
    "{"
    "  \"key\": {"
    "    \"type\": \"line\","
    "    \"prompt\": 2"
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  sput_fail_unless(
    error->code == ERROR_VALIDATOR_FIELD_PROMPT_INVALID, "field prompt invalid"
  );
  error_free(error);

  test_validator_teardown(fixture);
}

static void test_validator_valid_no_required() {
  struct TestValidatorFixture *fixture = test_validator_setup(
    "{"
    "  \"key\": {"
    "    \"type\": \"line\","
    "    \"prompt\": \"What value for key?\""
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  sput_fail_unless(error == 0, "valid");

  test_validator_teardown(fixture);
}

static void test_validator_field_type_yes() {
  struct TestValidatorFixture *fixture = test_validator_setup(
    "{"
    "  \"abc\": {"
    "    \"type\": \"yes\""
    "    \"prompt\": \"What value for key?\""
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  sput_fail_unless(error == 0, "yes valid");

  test_validator_teardown(fixture);
}

#endif /* _BOOTSTRAP_TEST_VALIDATOR */
