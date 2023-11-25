#include <string.h>

#include "validator.h"

static enum SpecValidationError
read_field(const cJSON *const field, struct Field **out) {
  if (!cJSON_IsObject(field)) {
    return SVE_FIELD_NOT_OBJECT;
  }

  enum SpecValidationError retval = 0;

  *out = malloc(sizeof(struct Field));

  const cJSON *type = cJSON_GetObjectItemCaseSensitive(field, "type");
  if (!cJSON_IsString(type)) {
    retval = SVE_FIELD_TYPE_INVALID;
    goto cleanup;
  }

  if (strcmp(type->valuestring, "STRING") == 0) {
    (*out)->type = FT_STRING;
  } else {
    retval = SVE_FIELD_TYPE_UNKNOWN;
    goto cleanup;
  }

  const cJSON *prompt = cJSON_GetObjectItemCaseSensitive(field, "prompt");
  if (cJSON_IsString(prompt)) {
    (*out)->prompt = prompt->valuestring;
  } else {
    retval = SVE_FIELD_PROMPT_INVALID;
    goto cleanup;
  }

  return retval;

cleanup:
  free(*out);
  return retval;
}

enum SpecValidationError
validate_spec_json(const cJSON *const parsed, struct DynArray **fields) {
  *fields = 0;

  if (!parsed) {
    // Indicates no `spec.json` file was found.
    return 0;
  }

  if (!cJSON_IsObject(parsed)) {
    return SVE_TOPLEVEL_NOT_OBJECT;
  }

  enum SpecValidationError retval = 0;
  // `cJSON_GetArraySize` works because internally JSON objects are stored as
  // arrays.
  *fields = dyn_array_new(cJSON_GetArraySize(parsed));

  cJSON *child = parsed->child;
  while (child) {
    struct Field *field = 0;
    retval = read_field(child, &field);
    if (retval) {
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
  return retval;
}
