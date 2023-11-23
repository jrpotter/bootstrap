#ifndef _SPEC_CONFIG_H
#define _SPEC_CONFIG_H

struct Config {
  // The root directory housing our specs. This string is nonempty.
  const char *root_dir;
  // The name of the spec we want to load. This string is nonempty.
  const char *target;
};

enum ConfigError {
  // Indicates the `$SPEC_ROOT_DIR` environment variable is empty.
  ENV_SPEC_ROOT_DIR_MISSING = 1,
  // Indicates the `$SPEC_ROOT_DIR` environment variable is not set.
  ENV_SPEC_ROOT_DIR_EMPTY,
  // Indicates the target argument is invalid.
  INVALID_TARGET,
};

enum ConfigError load_config(
  const char *root_dir,
  const char *target,
  struct Config *config
);

void free_config(struct Config *config);

#endif /* _SPEC_CONFIG_H */
