#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cJSON.h"
#include "config.h"
#include "error.h"
#include "evaluator.h"
#include "parser.h"
#include "validator.h"

static int run(const char *root_dir, const char *target) {
  int retval = EXIT_FAILURE;

  char *cwd = getcwd(0, 0);
  if (!root_dir) {
    root_dir = getenv("BOOTSTRAP_ROOT_DIR");
  }

  struct Error *error = 0;
  struct Config *config = 0;

  if ((error = config_load(cwd, root_dir, target, &config))) {
    fprintf(stderr, "%s", error->message);
    goto cleanup_cwd;
  }

  cJSON *parsed = 0;
  if ((error = parse_spec_json(config, &parsed))) {
    fprintf(stderr, "%s", error->message);
    goto cleanup_config;
  }

  struct DynArray *prompts = 0;
  if ((error = validate_spec_json(parsed, &prompts))) {
    fprintf(stderr, "%s", error->message);
    goto cleanup_parsed;
  }

  if ((error = evaluate_spec_json(config, prompts))) {
    fprintf(stderr, "%s", error->message);
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
  error_free(error);
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
