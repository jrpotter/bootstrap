#include "evaluator.h"

#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

#include "path.h"
#include "string_buf.h"

static enum SpecEvaluationError find_run_sh(const struct Config *const config) {
  struct stat sb;
  const char *segments[] = {config->root_dir, config->target, "run.sh"};
  char *filepath =
    join_path_segments(sizeof(segments) / sizeof(char *), segments);
  int stat_res = stat(filepath, &sb);
  free(filepath);

  if (stat_res == -1 && errno == ENOENT) {
    return SEE_RUN_SH_NOT_FOUND;
  }

  return 0;
}

enum SpecEvaluationError evaluate_spec_json(
  const struct Config *const config, const struct DynArray *const prompts
) {
  enum SpecEvaluationError retval = find_run_sh(config);
  if (retval != 0) {
    return retval;
  }

  if (prompts) {
    for (int i = 0; i < prompts->size; ++i) {
    }
    // TODO: Display prompts and collect answers.
  }

  const char *segments[] = {config->root_dir, config->target, "run.sh"};
  char *filepath =
    join_path_segments(sizeof(segments) / sizeof(char *), segments);

  struct StringBuf *command_buf = string_buf_new(1024);
  string_buf_sappend(command_buf, "cd ");
  string_buf_sappend(command_buf, config->root_dir);
  string_buf_cappend(command_buf, '/');
  string_buf_sappend(command_buf, config->target);
  string_buf_sappend(command_buf, " && ");
  string_buf_sappend(command_buf, "OUT=");
  string_buf_sappend(command_buf, config->cwd);
  string_buf_cappend(command_buf, ' ');
  string_buf_sappend(command_buf, filepath);
  const char *command = string_buf_convert(command_buf);

  free(filepath);

  // TODO: Want to return this status out.
  int status = system(command);
  ;

  return 0;
}
