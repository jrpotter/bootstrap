#include <stdlib.h>
#include <string.h>

#include "config.h"

const char *ENV_SPEC_ROOT_DIR = "SPEC_ROOT_DIR";

enum ConfigError load_config(const char *target, struct Config *config) {
  const char *root_dir = getenv(ENV_SPEC_ROOT_DIR);
  if (root_dir == 0) {
    return ENV_SPEC_ROOT_DIR_MISSING;
  }
  if (root_dir[0] == 0) {
    return ENV_SPEC_ROOT_DIR_EMPTY;
  }
  if (target == 0) {
    return INVALID_TARGET;
  }

  size_t target_len = strlen(target);
  if (target_len == 0) {
    return INVALID_TARGET;
  }

  config = malloc(sizeof(struct Config));
  config->root_dir = root_dir;

  char *copy_target = calloc(1, target_len + 1);
  strcpy(copy_target, target);
  config->target = copy_target;

  return 0;
}

void free_config(struct Config *config) {
  if (!config) {
    return;
  }
  free((void *)config->target);
  free(config);
}
