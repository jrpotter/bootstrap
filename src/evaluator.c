#include "evaluator.h"

#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

#include "path.h"
#include "string_buf.h"

static struct Error *find_run_sh(const struct Config *const config) {
  struct stat sb;
  const char *segments[] = {config->root_dir, config->target, "run.sh"};
  char *filepath =
    join_path_segments(sizeof(segments) / sizeof(char *), segments);
  int stat_res = stat(filepath, &sb);
  free(filepath);

  if (stat_res == -1 && errno == ENOENT) {
    return ERROR_NEW(
      ERROR_EVALUATOR_RUN_SH_NOT_FOUND,
      "Could not find ",
      config->target,
      "/run.sh"
    );
  }

  // TODO: Check run.sh is executable.

  return 0;
}

struct Error *evaluate_spec_json(
  const struct Config *const config, const struct DynArray *const prompts
) {
  struct Error *error = find_run_sh(config);
  if (error) {
    return error;
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

  return 0;
}
