#include "config.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "path.h"

struct Error *config_load(
  const char *cwd,
  const char *root_dir,
  const char *target,
  struct Config **config
) {
  assert(target);

  if (cwd == 0) {
    return ERROR_NEW(ERROR_CONFIG_ENV_CWD_INVALID, "Could not retrieve $CWD.");
  }
  if (root_dir == 0) {
    return ERROR_NEW(
      ERROR_CONFIG_ENV_ROOT_DIR_INVALID, "No specified root directory."
    );
  }

  const char *segments[] = {root_dir, target};
  const char *filepath =
    join_path_segments(sizeof(segments) / sizeof(char *), segments);

  struct stat sb;
  int stat_res = stat(filepath, &sb);
  struct Error *error = 0;

  if (stat_res == -1) {
    if (errno == ENOENT) {
      error = ERROR_NEW(
        ERROR_CONFIG_TARGET_NOT_FOUND, "Spec ", filepath, " not found."
      );
    } else {
      error = ERROR_NEW(
        ERROR_CONFIG_TARGET_INVALID, "Spec ", filepath, " is invalid."
      );
    }
    goto cleanup;
  }
  if (!S_ISDIR(sb.st_mode)) {
    error = ERROR_NEW(
      ERROR_CONFIG_TARGET_NOT_DIR, "Spec ", filepath, " is not a directory."
    );
    goto cleanup;
  }

  *config = malloc(sizeof(struct Config));
  (*config)->cwd = cwd;
  (*config)->root_dir = root_dir;
  (*config)->target = target;

cleanup:
  free((void *)filepath);
  return error;
}

void config_free(struct Config *config) {
  if (!config) {
    return;
  }
  free(config);
  config = 0;
}
