#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "string_utils.h"
#include "test_cases.h"
#include "validator.h"

struct TestValidatorFixture {
  const char *json;
  struct DynArray *prompts;
  cJSON *parsed;
  struct Config config;
};

static struct TestValidatorFixture *test_setup(const char *json) {
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

static int test_toplevel_not_object() {
  struct TestValidatorFixture *fixture = test_setup("[]");

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  if (error->code != ERROR_VALIDATOR_TOP_LEVEL_NOT_OBJECT) {
    printf("top-level not object\n");
    return 1;
  }

  return 0;
}

static int test_field_not_object() {
  struct TestValidatorFixture *fixture = test_setup("{\"key\": \"$UNKNOWN\"}");

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  if (error->code != ERROR_VALIDATOR_FIELD_NOT_OBJECT) {
    printf("field not object\n");
    return 1;
  }

  return 0;
}

static int test_field_name_leading_digit() {
  struct TestValidatorFixture *fixture = test_setup(
    "{"
    "  \"1abc\": {"
    "    \"type\": \"line\""
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  if (error->code != ERROR_VALIDATOR_FIELD_NAME_INVALID) {
    printf("field name leading digit\n");
    return 1;
  }

  return 0;
}

static int test_field_name_non_alnum() {
  struct TestValidatorFixture *fixture = test_setup(
    "{"
    "  \"a~bc\": {"
    "    \"type\": \"line\""
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  if (error->code != ERROR_VALIDATOR_FIELD_NAME_INVALID) {
    printf("field name non alnum\n");
    return 1;
  }

  return 0;
}

static int test_field_type_invalid() {
  struct TestValidatorFixture *fixture = test_setup(
    "{"
    "  \"key\": {"
    "    \"type\": 2"
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  if (error->code != ERROR_VALIDATOR_FIELD_TYPE_INVALID) {
    printf("field type invalid\n");
    return 1;
  }

  return 0;
}

static int test_field_type_unknown() {
  struct TestValidatorFixture *fixture = test_setup(
    "{"
    "  \"key\": {"
    "    \"type\": \"UNKNOWN\""
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  if (error->code != ERROR_VALIDATOR_FIELD_TYPE_UNKNOWN) {
    printf("field type unknown\n");
    return 1;
  }

  return 0;
}

static int test_valid_type_ci() {
  struct TestValidatorFixture *fixture = test_setup(
    "{"
    "  \"key\": {"
    "    \"type\": \"LiNe\","
    "    \"prompt\": \"What value for key?\""
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  if (error != 0) {
    printf("valid ci not working\n");
    return 1;
  }

  return 0;
}

static int test_field_required_invalid() {
  struct TestValidatorFixture *fixture = test_setup(
    "{"
    "  \"key\": {"
    "    \"type\": \"line\","
    "    \"required\": 5"
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  if (error->code != ERROR_VALIDATOR_FIELD_REQUIRED_INVALID) {
    printf("field required invalid\n");
    return 1;
  }

  return 0;
}

static int test_field_required_valid() {
  struct TestValidatorFixture *fixture = test_setup(
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
  if (error != 0) {
    printf("required valid\n");
    return 1;
  }

  return 0;
}

static int test_field_prompt_invalid() {
  struct TestValidatorFixture *fixture = test_setup(
    "{"
    "  \"key\": {"
    "    \"type\": \"line\","
    "    \"prompt\": 2"
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  if (error->code != ERROR_VALIDATOR_FIELD_PROMPT_INVALID) {
    printf("field prompt invalid\n");
    return 1;
  }

  return 0;
}

static int test_valid_no_required() {
  struct TestValidatorFixture *fixture = test_setup(
    "{"
    "  \"key\": {"
    "    \"type\": \"line\","
    "    \"prompt\": \"What value for key?\""
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  if (error != 0) {
    printf("valid\n");
    return 1;
  }

  return 0;
}

static int test_field_type_yes() {
  struct TestValidatorFixture *fixture = test_setup(
    "{"
    "  \"abc\": {"
    "    \"type\": \"yes\""
    "    \"prompt\": \"What value for key?\""
    "  }"
    "}"
  );

  struct Error *error =
    validate_spec_json(&fixture->config, fixture->parsed, &fixture->prompts);
  if (error != 0) {
    printf("yes valid\n");
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
    {"toplevel-not-object", test_toplevel_not_object},
    {"field-not-object", test_field_not_object},
    {"field-name-leading-digit", test_field_name_leading_digit},
    {"field-name-non-alnum", test_field_name_non_alnum},
    {"field-type-invalid", test_field_type_invalid},
    {"field-type-unknown", test_field_type_unknown},
    {"valid-type-ci", test_valid_type_ci},
    {"field-required-invalid", test_field_required_invalid},
    {"field-required-valid", test_field_required_valid},
    {"field-prompt-invalid", test_field_prompt_invalid},
    {"valid-no-required", test_valid_no_required},
    {"field-type-yes", test_field_type_yes},
  };

  for (int i = 0; i < sizeof(cases) / sizeof(struct TestCase); ++i) {
    struct TestCase test_case = cases[i];
    if (strcmp(argv[1], test_case.name) == 0) {
      return test_case.func();
    }
  }

  return 1;
}
