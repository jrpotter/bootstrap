#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cJSON.h"
#include "config.h"
#include "loader.h"

const char *ENV_BOOTSTRAP_ROOT_DIR = "BOOTSTRAP_ROOT_DIR";

int main(int argc, char **argv) {
  int num = 0;

  if (argc != 2) {
    fprintf(stderr, "Usage: bootstrap <spec>\n");
    exit(EXIT_FAILURE);
  }

  int retval = EXIT_SUCCESS;
  char *cwd = getcwd(0, 0);
  const char *root_dir = getenv(ENV_BOOTSTRAP_ROOT_DIR);
  const char *target = argv[1];

  // `cwd` must be free'd.

  struct Config *config = 0;
  switch (config_load(cwd, root_dir, target, &config)) {
  case CE_ENV_CWD_INVALID:
    fprintf(stderr, "Could not retrieve the $CWD value.");
    retval = EXIT_FAILURE;
    goto cwd_cleanup;
  case CE_ENV_ROOT_DIR_INVALID:
    fprintf(stderr, "Must specify $BOOTSTRAP_ROOT_DIR environment variable.");
    retval = EXIT_FAILURE;
    goto cwd_cleanup;
  case CE_TARGET_INVALID:
    fprintf(stderr, "Target spec `%s` is invalid.", argv[1]);
    retval = EXIT_FAILURE;
    goto cwd_cleanup;
  }

  // `config` must be free'd.

  cJSON *parsed = 0;
  switch (read_spec_json(config, &parsed)) {
  case SJE_JSON_CANNOT_OPEN:
    fprintf(stderr, "Found `spec.json` but could not open.");
    retval = EXIT_FAILURE;
    goto config_cleanup;
  case SJE_JSON_INVALID:
    fprintf(stderr, "`spec.json` does not conform to bootstrap format.");
    retval = EXIT_FAILURE;
    goto config_cleanup;
  }

  // TODO: Extract the prompts out of the `spec.json` file.
  // TODO: Load in the curses interface.
  // TODO: Run `run.sh`.

config_cleanup:
  config_free(config);

cwd_cleanup:
  free(cwd);
  return retval;
}
