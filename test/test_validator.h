#ifndef _BOOTSTRAP_TEST_VALIDATOR
#define _BOOTSTRAP_TEST_VALIDATOR

#include "dyn_array.h"
#include "sput.h"
#include "validator.h"

static void test_validate_spec_json_not_toplevel_object() {
  struct DynArray *prompts = 0;
  cJSON *parsed = cJSON_Parse("[]");
  enum SpecValidationError retval = validate_spec_json(parsed, &prompts);
  sput_fail_unless(retval == SVE_NOT_TOPLEVEL_OBJECT, "not top-level object");

  cJSON_Delete(parsed);
}

#endif /* _BOOTSTRAP_TEST_VALIDATOR */
