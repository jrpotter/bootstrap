#include <stdlib.h>
#include <string.h>

#include "config.h"

enum ConfigError config_load(
  const char *cwd,
  const char *root_dir,
  const char *target,
  struct Config **config
) {
  if (cwd == 0) {
    return CE_ENV_CWD_INVALID;
  }
  if (root_dir == 0) {
    return CE_ENV_SPEC_ROOT_DIR_INVALID;
  }
  if (target == 0) {
    return CE_TARGET_INVALID;
  }

  *config = malloc(sizeof(struct Config));
  (*config)->cwd = cwd;
  (*config)->root_dir = root_dir;
  (*config)->target = target;

  return 0;
}

void config_free(struct Config *config) {
  if (!config) {
    return;
  }
  free(config);
}
