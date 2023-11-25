#ifndef _BOOTSTRAP_CONFIG_H
#define _BOOTSTRAP_CONFIG_H

#include "error.h"

struct Config {
  // The directory the `bootstrap` command was run from.
  // OWNERSHIP: Does not own this pointer.
  const char *cwd;
  // The root directory housing our specs.
  // OWNERSHIP: Does not own this pointer.
  const char *root_dir;
  // The name of the spec we want to bootstrap.
  // OWNERSHIP: Does not own this pointer.
  const char *target;
};

struct Error *config_load(
  const char *cwd,
  const char *root_dir,
  const char *target,
  struct Config **config
);

void config_free(struct Config *config);

#endif /* BOOTSTRAP_CONFIG_H */
