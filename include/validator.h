#ifndef _BOOTSTRAP_VALIDATOR_H
#define _BOOTSTRAP_VALIDATOR_H

#include "cJSON.h"
#include "dyn_array.h"

enum FieldType {
  FT_STRING = 1,
};

struct Field {
  enum FieldType type;
  const char *key;
  const char *prompt;
};

enum SpecValidationError {
  // The top-level JSON value of a `spec.json` file must be a JSON object.
  SVE_TOPLEVEL_NOT_OBJECT = 1,
  // The field is not a JSON object.
  SVE_FIELD_NOT_OBJECT,
  SVE_FIELD_TYPE_INVALID,
  SVE_FIELD_TYPE_UNKNOWN,
  SVE_FIELD_PROMPT_INVALID,
};

enum SpecValidationError validate_spec_json(
  const cJSON *const parsed, struct DynArray **fields
);

#endif /* _BOOTSTRAP_VALIDATOR_H */
