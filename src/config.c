#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "config.h"
#include "path.h"

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
    return CE_ENV_ROOT_DIR_INVALID;
  }
  if (target == 0) {
    return CE_TARGET_INVALID;
  }

  { // Check if the specified directory exists.
    struct stat sb;

    const char *segments[] = {root_dir, target};
    char *filepath =
      join_path_segments(sizeof(segments) / sizeof(char *), segments);

    int stat_res = stat(filepath, &sb);
    free(filepath);

    if (stat_res == -1) {
      if (errno == ENOENT) {
        return CE_TARGET_NOT_FOUND;
      }
      return CE_TARGET_INVALID;
    }
    if (!S_ISDIR(sb.st_mode)) {
      return CE_TARGET_NOT_DIR;
    }
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
