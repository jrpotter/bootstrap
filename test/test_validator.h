#ifndef _BOOTSTRAP_TEST_VALIDATOR
#define _BOOTSTRAP_TEST_VALIDATOR

#include "dyn_array.h"
#include "sput.h"
#include "validator.h"

struct TestValidatorFixture {
  const char *json;
  struct DynArray *prompts;
  cJSON *parsed;
};

static struct TestValidatorFixture *test_validator_setup(const char *json) {
  struct TestValidatorFixture *fixture =
    malloc(sizeof(struct TestValidatorFixture));
  fixture->json = json;
  fixture->prompts = 0;
  fixture->parsed = cJSON_Parse(json);
  return fixture;
}

static void test_validator_teardown(struct TestValidatorFixture *fixture) {
  if (fixture->parsed) {
    cJSON_Delete(fixture->parsed);
  }
  free(fixture);
}

static void test_validator_toplevel_not_object() {
  struct TestValidatorFixture *fixture = test_validator_setup("[]");

  enum SpecValidationError retval =
    validate_spec_json(fixture->parsed, &fixture->prompts);
  sput_fail_unless(retval == SVE_TOPLEVEL_NOT_OBJECT, "top-level not object");

  test_validator_teardown(fixture);
}

static void test_validator_field_not_object() {
  struct TestValidatorFixture *fixture =
    test_validator_setup("{\"key\": \"$UNKNOWN\"}");

  enum SpecValidationError retval =
    validate_spec_json(fixture->parsed, &fixture->prompts);
  sput_fail_unless(retval == SVE_FIELD_NOT_OBJECT, "field not object");

  test_validator_teardown(fixture);
}

static void test_validator_field_type_invalid() {
  struct TestValidatorFixture *fixture = test_validator_setup("{"
                                                              "  \"key\": {"
                                                              "    \"type\": 2"
                                                              "  }"
                                                              "}");

  enum SpecValidationError retval =
    validate_spec_json(fixture->parsed, &fixture->prompts);
  sput_fail_unless(retval == SVE_FIELD_TYPE_INVALID, "field type invalid");

  test_validator_teardown(fixture);
}

static void test_validator_field_type_unknown() {
  struct TestValidatorFixture *fixture =
    test_validator_setup("{"
                         "  \"key\": {"
                         "    \"type\": \"UNKNOWN\""
                         "  }"
                         "}");

  enum SpecValidationError retval =
    validate_spec_json(fixture->parsed, &fixture->prompts);
  sput_fail_unless(retval == SVE_FIELD_TYPE_UNKNOWN, "field type unknown");

  test_validator_teardown(fixture);
}

static void test_validator_field_prompt_invalid() {
  struct TestValidatorFixture *fixture =
    test_validator_setup("{"
                         "  \"key\": {"
                         "    \"type\": \"STRING\","
                         "    \"prompt\": 2"
                         "  }"
                         "}");

  enum SpecValidationError retval =
    validate_spec_json(fixture->parsed, &fixture->prompts);
  sput_fail_unless(retval == SVE_FIELD_PROMPT_INVALID, "field prompt invalid");

  test_validator_teardown(fixture);
}

static void test_validator_valid() {
  struct TestValidatorFixture *fixture =
    test_validator_setup("{"
                         "  \"key\": {"
                         "    \"type\": \"STRING\","
                         "    \"prompt\": \"What value for key?\""
                         "  }"
                         "}");

  enum SpecValidationError retval =
    validate_spec_json(fixture->parsed, &fixture->prompts);
  sput_fail_unless(retval == 0, "valid");

  test_validator_teardown(fixture);
}

#endif /* _BOOTSTRAP_TEST_VALIDATOR */
