#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "dyn_array.h"

int main(int argc, char **argv) {
  int num = 0;

  if (argc != 2) {
    fprintf(stderr, "Usage: gen-flake <spec>\n");
    exit(EXIT_FAILURE);
  }

  struct Config config;
  switch (load_config(argv[1], &config)) {
  case ENV_SPEC_ROOT_DIR_MISSING:
    fprintf(stderr, "Must specify $SPEC_ROOT_DIR environment variable.");
    exit(EXIT_FAILURE);
  case ENV_SPEC_ROOT_DIR_EMPTY:
    fprintf(stderr, "$SPEC_ROOT_DIR environment variable should not be empty.");
    exit(EXIT_FAILURE);
  case INVALID_TARGET:
    fprintf(stderr, "Target spec `%s` is invalid.", argv[1]);
    exit(EXIT_FAILURE);
  default:
    // A return value of `0` indicates no issue.
    break;
  }

  free_config(&config);
  return EXIT_SUCCESS;
}
