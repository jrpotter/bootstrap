#include <assert.h>
#include <glob.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "config.h"
#include "dyn_array.h"

static const char *glob_spec_pattern(const struct Config *const config) {
  assert(config);

  int spec_path_length = strlen(config->spec_path);
  int target_length = strlen(config->target);
  // Support paths that have trailing forward slashes.
  int sep_length = 0;
  if (config->spec_path[spec_path_length - 1] == '/') {
    sep_length = 1;
  }

  char *pattern = calloc(1, spec_path_length + sep_length + target_length + 1);
  memcpy(pattern, config->spec_path, spec_path_length);
  if (sep_length) {
    memcpy(pattern + spec_path_length, "/", sep_length);
  }
  memcpy(pattern + spec_path_length + sep_length, config->target,
         target_length);

  return pattern;
}

static int glob_specs(char *name, const char *spec_path,
                      struct DynArray *entries) {
  assert(name);
  assert(entries);

  int name_length = strlen(name);
  char *pattern = malloc(name_length + 3);
  if (!pattern) {
    goto cleanup;
  }
  memcpy(pattern, name, name_length);
  memcpy(pattern + name_length + 1, "/*", 3);

  glob_t *pglob = 0;

  int retval = 0;
  if ((retval = glob(pattern, GLOB_ONLYDIR | GLOB_MARK, 0, pglob))) {
    goto cleanup;
  }

  // GLOB_ONLYDIR is just a hint. We still need to actually verify the only
  // results we are reading in are directories.
  // TODO:

  globfree(pglob);

cleanup:
  if (pattern) {
    free(pattern);
  }
  return retval;
}

int main(int argc, char **argv) {
  int num = 0;

  if (argc != 2) {
    fprintf(stderr, "Usage: gen-flake <spec>\n");
    exit(EXIT_FAILURE);
  }

  struct Config config;
  switch (load_config(argv[1], &config)) {
  case ENV_SPEC_PATH_EMPTY:
    fprintf(stderr, "Must specify GEN_FLAKE_SPEC_PATH environment variable.");
    exit(EXIT_FAILURE);
  case ENV_SPEC_PATH_MISSING:
    fprintf(stderr,
            "GEN_FLAKE_SPEC_PATH environment variable should not be empty.");
    exit(EXIT_FAILURE);
  case INVALID_TARGET:
    fprintf(stderr, "Spec `%s` is invalid.", argv[1]);
    exit(EXIT_FAILURE);
  default:
    // Return value of `0` indicates no issue.
    break;
  }

  free_config(&config);
  return EXIT_SUCCESS;
}
