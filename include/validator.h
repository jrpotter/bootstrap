#ifndef _BOOTSTRAP_VALIDATOR_H
#define _BOOTSTRAP_VALIDATOR_H

#include "cJSON.h"
#include "dyn_array.h"

enum PromptType {
  PT_STRING = 1,
};

struct Prompt {
  const char *key;
  enum PromptType type;
};

enum SpecValidationError {
  SVE_NOT_TOPLEVEL_OBJECT = 1,
  // The value of a top-level key does not correspond to one of the following:
  // * "$STRING"
  SVE_INVALID_VALUE,
};

enum SpecValidationError
validate_spec_json(const cJSON *const parsed, struct DynArray **prompts);

#endif /* _BOOTSTRAP_VALIDATOR_H */
