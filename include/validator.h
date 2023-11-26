/**
@file
@brief `spec.json` validator.
*/
#ifndef _BOOTSTRAP_VALIDATOR_H
#define _BOOTSTRAP_VALIDATOR_H

#include "cJSON.h"
#include "config.h"
#include "dyn_array.h"
#include "error.h"

/**
@brief The types of fields `bootstrap` can handle.
*/
enum FieldType {
  FT_TEXT = 1,
};

/**
@brief A container for relevant fields of a `cJSON`-parsed field.

A @ref Field refers to the top-level key-value pairs found in the `spec.json`
file. For instance, the fields of:

```json
{
  "abc": {
    "type": "text",
    "prompt": "ABC> "
  },
  "def": {
    "type": "text",
    "prompt": "DEF> "
  },
}
```

are `abc` and `def`.
*/
struct Field {
  /// @brief The type of field. Denotes what prompt should be displayed prior to
  /// evaluation.
  enum FieldType type;
  /// @brief A reference to the name of the field. Does not take ownership of
  /// this value.
  const char *key;
  /// @brief A reference to the supplied prompt. Does not take ownership of this
  /// value.
  const char *prompt;
};

/**
@brief Verify the `spec.json` file is formatted correctly.

@param config
 A reference to the parameters describing the desired spec.
@param parsed
 A possible null pointer to the parsed `spec.json` file. If null, this method
 simply sets *fields to a null pointer.
@param fields
 The array of `Field`s defined in the top-level `spec.json` file, provided
 validation was successful.
@return
 A null pointer if no error occurs. Otherwise an @ref Error pointer.
*/
struct Error *validate_spec_json(
  const struct Config *const config,
  const cJSON *const parsed,
  struct DynArray **fields
);

#endif /* _BOOTSTRAP_VALIDATOR_H */
