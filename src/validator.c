#include <assert.h>

#include "validator.h"

enum SpecValidationError
validate_spec_json(const cJSON *const parsed, struct DynArray **prompts) {
  *prompts = 0;

  if (!parsed) {
    // Indicates no `spec.json` file was found.
    return 0;
  }

  if (!cJSON_IsObject(parsed)) {
    return SVE_NOT_TOPLEVEL_OBJECT;
  }

  enum SpecValidationError retval = 0;
  // `cJSON_GetArraySize` works because internally objects are stored as arrays.
  *prompts = dyn_array_new(cJSON_GetArraySize(parsed));

  assert(0);

cleanup:
  dyn_array_free(*prompts);
  *prompts = 0;
  return retval;
}
