#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "dyn_array.h"

const char *ENV_BOOTSTRAP_ROOT_DIR = "BOOTSTRAP_ROOT_DIR";

int main(int argc, char **argv) {
  int num = 0;

  if (argc != 2) {
    fprintf(stderr, "Usage: bootstrap <spec>\n");
    exit(EXIT_FAILURE);
  }

  const char *cwd = getcwd(0, 0);
  const char *root_dir = getenv(ENV_BOOTSTRAP_ROOT_DIR);
  const char *target = argv[1];

  struct Config *config = 0;
  switch (config_load(cwd, root_dir, target, &config)) {
  case CE_ENV_CWD_INVALID:
    fprintf(stderr, "Could not retrieve the $CWD value.");
    exit(EXIT_FAILURE);
  case CE_ENV_ROOT_DIR_INVALID:
    fprintf(stderr, "Must specify $BOOTSTRAP_ROOT_DIR environment variable.");
    exit(EXIT_FAILURE);
  case CE_TARGET_INVALID:
    fprintf(stderr, "Target spec `%s` is invalid.", argv[1]);
    exit(EXIT_FAILURE);
  }

  config_free(config);
  free((void *)cwd);
  return EXIT_SUCCESS;
}
