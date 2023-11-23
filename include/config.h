#ifndef _SPEC_CONFIG_H
#define _SPEC_CONFIG_H

struct Config {
  // The directory the `spec` command was run from.
  // OWNERSHIP: Does not own this pointer.
  const char *cwd;
  // The root directory housing our specs.
  // OWNERSHIP: Does not own this pointer.
  const char *root_dir;
  // The name of the spec we want to load.
  // OWNERSHIP: Does not own this pointer.
  const char *target;
};

enum ConfigError {
  // Indicates the $CWD could not be retrieved.
  CE_ENV_CWD_INVALID = 1,
  // Indicates the `$SPEC_ROOT_DIR` environment variable is empty.
  CE_ENV_SPEC_ROOT_DIR_INVALID,
  // Indicates the target argument is invalid.
  CE_TARGET_INVALID,
};

enum ConfigError config_load(
  const char *cwd,
  const char *root_dir,
  const char *target,
  struct Config **config
);

void config_free(struct Config *config);

#endif /* _SPEC_CONFIG_H */
