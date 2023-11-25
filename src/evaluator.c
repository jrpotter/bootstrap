#include "evaluator.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "path.h"
#include "string_buf.h"
#include "validator.h"

static struct Error *find_run_sh(const struct Config *const config) {
  assert(config);

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

  if (!(sb.st_mode & S_IXUSR)) {
    return ERROR_NEW(
      ERROR_EVALUATOR_RUN_SH_NOT_EXEC,
      config->target,
      "/run.sh is not executable."
    );
  }

  return 0;
}

static const char *prompt_field(struct Field *field) {
  assert(field);

  switch (field->type) {
  case FT_STRING:
    printf("%s", field->prompt);
    // TODO: Probably want this buffer size to be a bit more dynamic.
    char *input = calloc(1, 1024);
    if (fgets(input, 1024, stdin)) {
      size_t len = strlen(input);
      if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
      }
      return input;
    } else {
      free(input);
      return 0;
    }
  }
}

static void push_env(
  struct StringBuf *env, const char *key, const char *value
) {
  assert(env);
  for (const char *c = key; *c; ++c) {
    string_buf_cappend(env, toupper(*c));
  }
  string_buf_sappend(env, "='");
  string_buf_sappend(env, value);
  string_buf_sappend(env, "' ");
}

int evaluate_run_sh(
  const struct Config *const config,
  const struct DynArray *const fields,
  struct Error **error
) {
  *error = find_run_sh(config);
  if (*error) {
    return EXIT_FAILURE;
  }

  struct StringBuf *env_buf = string_buf_new(512);
  push_env(env_buf, "OUT", config->cwd);

  if (fields) {
    for (int i = 0; i < fields->size; ++i) {
      struct Field *field = fields->buf[i];
      const char *response = prompt_field(field);
      if (!response) {
        *error = ERROR_NEW(
          ERROR_EVALUATOR_RESPONSE_INVALID, "Could not read in response."
        );
        string_buf_free(env_buf);
        return EXIT_FAILURE;
      }
      push_env(env_buf, field->key, response);
    }
  }

  const char *segments[] = {config->root_dir, config->target, "run.sh"};
  const char *filepath =
    join_path_segments(sizeof(segments) / sizeof(char *), segments);
  const char *env = string_buf_convert(env_buf);

  struct StringBuf *command_buf = string_buf_new(1024);
  string_buf_sappend(command_buf, "cd ");
  string_buf_sappend(command_buf, config->root_dir);
  string_buf_cappend(command_buf, '/');
  string_buf_sappend(command_buf, config->target);
  string_buf_sappend(command_buf, " && ");
  string_buf_sappend(command_buf, env);
  string_buf_sappend(command_buf, filepath);
  const char *command = string_buf_convert(command_buf);

  free((void *)env);
  free((void *)filepath);

  int exit_code = system(command);
  free((void *)command);
  return WEXITSTATUS(exit_code);
}
