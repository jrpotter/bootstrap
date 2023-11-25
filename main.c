#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cJSON.h"
#include "config.h"
#include "evaluator.h"
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
  case SVE_TOPLEVEL_NOT_OBJECT:
    fprintf(stderr, "`%s/spec.json` is not a JSON object.\n", target);
    goto cleanup_parsed;
  case SVE_FIELD_NOT_OBJECT:
    fprintf(
      stderr,
      "Encountered child in `%s/spec.json` that is not a JSON object.\n",
      target
    );
    goto cleanup_parsed;
  case SVE_FIELD_TYPE_INVALID:
    fprintf(stderr, "Types must be string values.\n");
    goto cleanup_parsed;
  case SVE_FIELD_TYPE_UNKNOWN:
    fprintf(
      stderr, "Encountered an unknown `type` in `%s/spec.json`.\n", target
    );
    goto cleanup_parsed;
  case SVE_FIELD_PROMPT_INVALID:
    fprintf(stderr, "Prompts must be string values.\n");
    goto cleanup_parsed;
  }

  switch (evaluate_spec_json(config, prompts)) {
  case SEE_RUN_SH_NOT_FOUND:
    fprintf(stderr, "Could not find `%s/run.sh`.\n", target);
    goto cleanup_parsed;
  case SEE_INVALID_PROMPT_RESPONSE:
    fprintf(stderr, "Could not interpret response.\n");
    goto cleanup_parsed;
  }

  retval = EXIT_SUCCESS;

cleanup_prompts:
  if (prompts) {
    dyn_array_free(prompts);
  }

cleanup_parsed:
  if (parsed) {
    cJSON_Delete(parsed);
  }

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
