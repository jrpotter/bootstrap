#ifndef _BOOTSTRAP_VALIDATOR_H
#define _BOOTSTRAP_VALIDATOR_H

#include "cJSON.h"
#include "dyn_array.h"
#include "error.h"

enum FieldType {
  FT_STRING = 1,
};

struct Field {
  enum FieldType type;
  const char *key;
  const char *prompt;
};

struct Error *validate_spec_json(
  const cJSON *const parsed, struct DynArray **fields
);

#endif /* _BOOTSTRAP_VALIDATOR_H */
