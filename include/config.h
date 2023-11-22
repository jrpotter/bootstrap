#ifndef GEN_FLAKE_CONFIG_H
#define GEN_FLAKE_CONFIG_H

struct Config {
  // The root directory housing our specs. This string is nonempty.
  const char *spec_path;
  // The name of the spec we want to load. This string is nonempty.
  const char *target;
};

enum ConfigError {
  // Indicates the `$GEN_FLAKE_SPEC_PATH` environment variable is not set.
  ENV_SPEC_PATH_EMPTY = 1,
  // Indicates the `$GEN_FLAKE_SPEC_PATH` environment variable is empty.
  ENV_SPEC_PATH_MISSING,
  // Indicates the target argument is invalid.
  INVALID_TARGET,
};

enum ConfigError load_config(const char *target, struct Config *config);

void free_config(struct Config *config);

#endif /* GEN_FLAKE_CONFIG_H */
