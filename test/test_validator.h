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

static void test_validate_spec_json_not_toplevel_object() {
  struct TestValidatorFixture *fixture = test_validator_setup("[]");

  enum SpecValidationError retval =
    validate_spec_json(fixture->parsed, &fixture->prompts);
  sput_fail_unless(retval == SVE_NOT_TOPLEVEL_OBJECT, "not top-level object");

  test_validator_teardown(fixture);
}

static void test_validate_spec_json_invalid_value_type() {
  struct TestValidatorFixture *fixture =
    test_validator_setup("{\"key\": \"$UNKNOWN\"}");

  enum SpecValidationError retval =
    validate_spec_json(fixture->parsed, &fixture->prompts);
  sput_fail_unless(retval == SVE_INVALID_VALUE, "invalid value");

  test_validator_teardown(fixture);
}

static void test_validate_spec_json_valid() {
  struct TestValidatorFixture *fixture =
    test_validator_setup("{\"key\": \"$STRING\"}");

  enum SpecValidationError retval =
    validate_spec_json(fixture->parsed, &fixture->prompts);
  sput_fail_unless(retval == 0, "valid");

  test_validator_teardown(fixture);
}

#endif /* _BOOTSTRAP_TEST_VALIDATOR */
