#include "validator.h"

#include <string.h>

static struct Error *read_field(const cJSON *const field, struct Field **out) {
  if (!cJSON_IsObject(field)) {
    return ERROR_NEW(
      ERROR_VALIDATOR_FIELD_NOT_OBJECT,
      "Field \"",
      field->string,
      "\" is not a JSON object."
    );
  }

  struct Error *error = 0;
  *out = malloc(sizeof(struct Field));

  const cJSON *type = cJSON_GetObjectItemCaseSensitive(field, "type");
  if (!cJSON_IsString(type)) {
    error = ERROR_NEW(
      ERROR_VALIDATOR_FIELD_TYPE_INVALID,
      "Field \"",
      field->string,
      "\" has non-string \"type\"."
    );
    goto cleanup;
  }

  if (strcmp(type->valuestring, "STRING") == 0) {
    (*out)->type = FT_STRING;
  } else {
    error = ERROR_NEW(
      ERROR_VALIDATOR_FIELD_TYPE_UNKNOWN,
      "Field \"",
      field->string,
      "\" has unknown \"type\"."
    );
    goto cleanup;
  }

  const cJSON *prompt = cJSON_GetObjectItemCaseSensitive(field, "prompt");
  if (cJSON_IsString(prompt)) {
    (*out)->prompt = prompt->valuestring;
  } else {
    error = ERROR_NEW(
      ERROR_VALIDATOR_FIELD_PROMPT_INVALID,
      "Field \"",
      field->string,
      "\" has non-string \"prompt\"."
    );
    goto cleanup;
  }

  return error;

cleanup:
  free(*out);
  return error;
}

struct Error *validate_spec_json(
  const cJSON *const parsed, struct DynArray **fields
) {
  *fields = 0;

  if (!parsed) {
    // Indicates no `spec.json` file was found.
    return 0;
  }

  if (!cJSON_IsObject(parsed)) {
    return ERROR_NEW(
      ERROR_VALIDATOR_TOP_LEVEL_NOT_OBJECT,
      "Top-level JSON value in spec.json is not an object."
    );
  }

  struct Error *error = 0;
  // `cJSON_GetArraySize` works because internally JSON objects are stored as
  // arrays.
  *fields = dyn_array_new(cJSON_GetArraySize(parsed));

  cJSON *child = parsed->child;
  while (child) {
    struct Field *field = 0;
    error = read_field(child, &field);
    if (error) {
      goto cleanup;
    }
    dyn_array_push(*fields, field);
    child = child->next;
  }

  return 0;

cleanup:
  if (*fields) {
    dyn_array_free(*fields);
    *fields = 0;
  }
  return error;
}
