#include <string.h>

#include "validator.h"

static struct Prompt *create_prompt(const cJSON *const kv) {
  if (!kv->string) {
    return 0;
  }

  if (cJSON_IsString(kv) && strcmp(kv->valuestring, "$STRING") == 0) {
    struct Prompt *prompt = malloc(sizeof(struct Prompt));
    prompt->key = kv->string;
    prompt->type = PT_STRING;
    return prompt;
  }

  return 0;
}

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
  // `cJSON_GetArraySize` works because internally JSON objects are stored as
  // arrays.
  *prompts = dyn_array_new(cJSON_GetArraySize(parsed));

  cJSON *child = parsed->child;
  while (child) {
    struct Prompt *prompt = create_prompt(child);
    if (!prompt) {
      retval = SVE_INVALID_VALUE;
      goto cleanup;
    }
    dyn_array_push(*prompts, prompt);
    child = child->next;
  }

  return 0;

cleanup:
  if (*prompts) {
    dyn_array_free(*prompts);
    *prompts = 0;
  }
  return retval;
}
