#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cJSON.h"
#include "config.h"
#include "parser.h"
#include "validator.h"

static int run(const char *root_dir, const char *target) {
  int retval = EXIT_FAILURE;
  char *cwd = getcwd(0, 0);
  if (!root_dir) {
    root_dir = getenv("BOOTSTRAP_ROOT_DIR");
  }

  struct Config *config = 0;
  switch (config_load(cwd, root_dir, target, &config)) {
  case CE_ENV_CWD_INVALID:
    fprintf(stderr, "Could not retrieve $CWD.\n");
    goto cleanup_cwd;
  case CE_ENV_ROOT_DIR_INVALID:
    fprintf(
      stderr,
      "Either supply a value to `-d` or specify the $BOOTSTRAP_ROOT_DIR "
      "environment variable.\n"
    );
    goto cleanup_cwd;
  case CE_TARGET_INVALID:
    fprintf(stderr, "Spec `%s` is invalid.\n", target);
    goto cleanup_cwd;
  case CE_TARGET_NOT_FOUND:
    fprintf(stderr, "Spec `%s` not found.\n", target);
    goto cleanup_cwd;
  case CE_TARGET_NOT_DIR:
    fprintf(stderr, "Spec `%s` is not a directory.\n", target);
    goto cleanup_cwd;
  }

  cJSON *parsed = 0;
  switch (parse_spec_json(config, &parsed)) {
  case SPE_CANNOT_OPEN:
    fprintf(stderr, "Cannot open `%s/spec.json`.\n", target);
    goto cleanup_config;
  case SPE_INVALID_SYNTAX:
    fprintf(stderr, "`%s/spec.json` is not valid JSON.\n", target);
    goto cleanup_config;
  }

  struct DynArray *prompts = 0;
  switch (validate_spec_json(parsed, &prompts)) {
  case SVE_NOT_TOPLEVEL_OBJECT:
    fprintf(stderr, "`%s/spec.json` is not a JSON object.\n", target);
    goto cleanup_parsed;
  case SVE_INVALID_VALUE:
    fprintf(stderr, "unknown value type found in `%s/spec.json`.\n", target);
    goto cleanup_parsed;
  }

  // TODO: Extract the prompts out of the `spec.json` file.
  // TODO: Load in the curses interface.
  // TODO: Run `run.sh`.

  retval = EXIT_SUCCESS;

cleanup_parsed:
  cJSON_Delete(parsed);

cleanup_config:
  config_free(config);

cleanup_cwd:
  free(cwd);
  return retval;
}

int main(int argc, char **argv) {
  char *root_dir = 0;
  char *target = 0;

  int opt;
  while ((opt = getopt(argc, argv, "d:")) != -1) {
    switch (opt) {
    case 'd':
      root_dir = optarg;
      break;
    }
  }

  for (int index = optind; index < argc; index++) {
    if (target == 0) {
      target = argv[index];
    } else {
      fprintf(stderr, "Usage: bootstrap [-d <ROOT_DIR>] <spec>\n");
      return EXIT_FAILURE;
    }
  }

  if (!target) {
    fprintf(stderr, "Usage: bootstrap [-d <ROOT_DIR>] <spec>\n");
    return EXIT_FAILURE;
  }

  return run(root_dir, target);
}
