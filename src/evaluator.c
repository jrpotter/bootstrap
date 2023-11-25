#include "evaluator.h"

#include <errno.h>
#include <stdio.h>

#include "path.h"

static int find_run_sh(const struct Config *const config, FILE **handle) {
  const char *segments[] = {config->root_dir, config->target, "run.sh"};
  char *filepath =
    join_path_segments(sizeof(segments) / sizeof(char *), segments);

  int retval = 0;
  // It is ok if the file does not exist. It is not ok if we couldn't open the
  // file for any other reason.
  *handle = fopen(filepath, "r");
  if (!*handle && errno != ENOENT) {
    retval = errno;
  }

  free(filepath);
  return retval;
}

enum SpecEvaluationError evaluate_spec_json(
  const struct Config *const config, const struct DynArray *const prompts
) {
  if (prompts) {
    for (int i = 0; i < prompts->size; ++i) {
    }
    // TODO: Display prompts and collect answers.
  }

  // TODO: Run `run.sh`.

  return 0;
}
