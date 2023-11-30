#include "validator.h"

#include <ctype.h>

#include "console.h"
#include "string_utils.h"

static struct Error *read_field(
  const struct Config *const config,
  const cJSON *const field,
  struct Field **out
) {
  if (!cJSON_IsObject(field)) {
    return ERROR_NEW(
      ERROR_VALIDATOR_FIELD_NOT_OBJECT,
      ANSI_RED_F("ERROR"),
      ": Field ",
      ANSI_PURPLE_F(field->string),
      " in ",
      ANSI_BLUE_F(config->target, "/spec.json"),
      " is not a JSON object."
    );
  }

  if (isdigit(field->string[0])) {
    return ERROR_NEW(
      ERROR_VALIDATOR_FIELD_NAME_INVALID,
      ANSI_RED_F("ERROR"),
      ": Field ",
      ANSI_PURPLE_F(field->string),
      " in ",
      ANSI_BLUE_F(config->target, "/spec.json"),
      " may not begin with a digit."
    );
  } else {
    for (const char *c = field->string; *c; ++c) {
      if (*c != '_' && !isalnum(*c)) {
        return ERROR_NEW(
          ERROR_VALIDATOR_FIELD_NAME_INVALID,
          ANSI_RED_F("ERROR"),
          ": Field ",
          ANSI_PURPLE_F(field->string),
          " in ",
          ANSI_BLUE_F(config->target, "/spec.json"),
          " must consist of only alphanumeric characters and underscores."
        );
      }
    }
  }

  struct Error *error = 0;
  *out = malloc(sizeof(struct Field));
  (*out)->key = field->string;

  const cJSON *type = cJSON_GetObjectItemCaseSensitive(field, "type");
  if (!cJSON_IsString(type)) {
    error = ERROR_NEW(
      ERROR_VALIDATOR_FIELD_TYPE_INVALID,
      ANSI_RED_F("ERROR"),
      ": Field ",
      ANSI_PURPLE_F(field->string),
      " in ",
      ANSI_BLUE_F(config->target, "/spec.json"),
      " has non-string ",
      ANSI_PURPLE_F("type"),
      "."
    );
    goto cleanup;
  }

  if (strcmp_ci(type->valuestring, "line") == 0) {
    (*out)->type = FT_LINE;
  } else {
    error = ERROR_NEW(
      ERROR_VALIDATOR_FIELD_TYPE_UNKNOWN,
      ANSI_RED_F("ERROR"),
      ": Field ",
      ANSI_PURPLE_F(field->string),
      " in ",
      ANSI_BLUE_F(config->target, "/spec.json"),
      " has unknown ",
      ANSI_PURPLE_F("type"),
      "."
    );
    goto cleanup;
  }

  const cJSON *required = cJSON_GetObjectItemCaseSensitive(field, "required");
  if (!required) {
    (*out)->required = true;
  } else if (cJSON_IsBool(required)) {
    (*out)->required = required->valueint;
  } else {
    error = ERROR_NEW(
      ERROR_VALIDATOR_FIELD_REQUIRED_INVALID,
      ANSI_RED_F("ERROR"),
      ": Field ",
      ANSI_PURPLE_F(field->string),
      " in ",
      ANSI_BLUE_F(config->target, "/spec.json"),
      " has non-boolean ",
      ANSI_PURPLE_F("required"),
      "."
    );
    goto cleanup;
  }

  const cJSON *prompt = cJSON_GetObjectItemCaseSensitive(field, "prompt");
  if (cJSON_IsString(prompt)) {
    (*out)->prompt = prompt->valuestring;
  } else {
    error = ERROR_NEW(
      ERROR_VALIDATOR_FIELD_PROMPT_INVALID,
      ANSI_RED_F("ERROR"),
      ": Field ",
      ANSI_PURPLE_F(field->string),
      " in ",
      ANSI_BLUE_F(config->target, "/spec.json"),
      " has non-string ",
      ANSI_PURPLE_F("prompt"),
      "."
    );
    goto cleanup;
  }

  return error;

cleanup:
  free(*out);
  return error;
}

struct Error *validate_spec_json(
  const struct Config *const config,
  const cJSON *const parsed,
  struct DynArray **fields
) {
  *fields = 0;

  if (!parsed) {
    // Indicates no `spec.json` file was found.
    return 0;
  }

  if (!cJSON_IsObject(parsed)) {
    return ERROR_NEW(
      ERROR_VALIDATOR_TOP_LEVEL_NOT_OBJECT,
      ANSI_RED_F("ERROR"),
      ": Top-level JSON value in ",
      ANSI_BLUE_F(config->target, "/spec.json"),
      " is not an object."
    );
  }

  struct Error *error = 0;
  // `cJSON_GetArraySize` works because internally JSON objects are stored as
  // arrays.
  *fields = dyn_array_new(cJSON_GetArraySize(parsed));

  cJSON *child = parsed->child;
  while (child) {
    struct Field *field = 0;
    error = read_field(config, child, &field);
    if (error) {
      goto cleanup;
    }
    dyn_array_push(*fields, field);
    child = child->next;
  }

  return 0;

cleanup:
  dyn_array_free(*fields);
  *fields = 0;
  return error;
}
