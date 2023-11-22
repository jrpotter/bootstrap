#include <stdlib.h>
#include <string.h>

#include "config.h"

const char *ENV_SPEC_PATH = "GEN_FLAKE_SPEC_PATH";

enum ConfigError load_config(const char *target, struct Config *config) {
  const char *spec_path = getenv(ENV_SPEC_PATH);
  if (spec_path == 0) {
    return ENV_SPEC_PATH_MISSING;
  }
  if (spec_path[0] == 0) {
    return ENV_SPEC_PATH_EMPTY;
  }
  if (target == 0) {
    return INVALID_TARGET;
  }

  size_t target_len = strlen(target);
  if (target_len == 0) {
    return INVALID_TARGET;
  }

  config = malloc(sizeof(struct Config));
  config->spec_path = spec_path;

  char *copy_target = calloc(1, target_len + 1);
  strcpy(copy_target, target);
  config->target=copy_target;

  return 0;
}

void free_config(struct Config *config) {
  if (!config) {
    return;
  }
  free((void *)config->target);
  free(config);
}
