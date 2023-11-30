#include "evaluator.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "console.h"
#include "string_buf.h"
#include "string_utils.h"
#include "validator.h"

#define BANNER_LENGTH 60

static struct Error *find_run_exec(const struct Config *const config) {
  assert(config);

  struct stat sb;
  const char *segments[] = {config->root_dir, config->target, "runner"};
  char *filepath = join(sizeof(segments) / sizeof(char *), segments, '/');
  int stat_res = stat(filepath, &sb);
  free(filepath);

  if (stat_res == -1 && errno == ENOENT) {
    return ERROR_NEW(
      ERROR_EVALUATOR_RUNNER_NOT_FOUND,
      ANSI_RED_F("NOT_FOUND"),
      ": Could not find ",
      ANSI_BLUE_F(config->target, "/runner"),
      "."
    );
  }

  if (!(sb.st_mode & S_IXUSR)) {
    return ERROR_NEW(
      ERROR_EVALUATOR_RUNNER_NOT_EXEC,
      ANSI_RED_F("ERROR"),
      ": ",
      ANSI_BLUE_F(config->target, "/runner"),
      " is not executable."
    );
  }

  return 0;
}

static void print_header(const struct Config *const config) {
  assert(config);

  struct StringBuf *banner = string_buf_new(BANNER_LENGTH * 2);
  for (int i = 0; i < BANNER_LENGTH; ++i) {
    string_buf_cappend(banner, '=');
  }

  struct StringBuf *header = string_buf_new(128);
  string_buf_sappend(header, config->target);
  string_buf_sappend(header, "/spec.json");
  int left_padding = (BANNER_LENGTH - header->size) / 2;

  printf("%s\n", banner->buf);
  printf("%*s", left_padding, "");
  printf("%s%s/spec.json%s\n\n", ANSI_BLUE_F(config->target));
  printf("(%s%s%s) indicates a required field.\n", ANSI_YELLOW_F("*"));
  printf("%s\n\n", banner->buf);

  string_buf_free(header);
  string_buf_free(banner);
}

static void print_prompt(const struct Field *const field) {
  assert(field);

  if (field->required) {
    printf("%s%s%s%s", ANSI_YELLOW_F("*"), field->prompt);
  } else {
    printf("%s", field->prompt);
  }
}

static const char *query_line(const struct Field *const field) {
  assert(field);

  // TODO: Dynamically size this value.
  char *response = calloc(1, 1024);

  do {
    print_prompt(field);
    if (fgets(response, 1024, stdin)) {
      trim_leading(response);
      trim_trailing(response);
      if (response[0] != 0) {
        return response;
      }
    } else {  // Likely EOF. Force-quit even if required.
      printf("\n");
      break;
    }
  } while (field->required);

  free(response);
  return 0;
}

static void push_env(
  struct StringBuf *env, const char *key, const char *value
) {
  assert(env);

  for (const char *c = key; *c; ++c) {
    string_buf_cappend(env, toupper(*c));
  }
  string_buf_sappend(env, "=");

  if (value) {
    string_buf_cappend(env, '\'');
    string_buf_sappend(env, value);
    string_buf_sappend(env, "' ");
  } else {
    string_buf_cappend(env, ' ');
  }
}

static struct Error *push_fields(
  const struct Config *const config,
  const struct DynArray *const fields,
  struct StringBuf **env_buf
) {
  for (int i = 0; i < fields->size; ++i) {
    struct Field *field = fields->buf[i];
    const char *response = 0;
    switch (field->type) {
    case FT_LINE:
      response = query_line(field);
      break;
    }
    if (field->required && !response) {
      return ERROR_NEW(
        ERROR_EVALUATOR_RESPONSE_INVALID,
        ANSI_RED_F("ERROR"),
        ": Could not read response."
      );
    }
    push_env(*env_buf, field->key, response);
  }
  return 0;
}

int evaluate_runner(
  const struct Config *const config,
  const struct DynArray *const fields,
  struct Error **error
) {
  if ((*error = find_run_exec(config))) {
    return EXIT_FAILURE;
  }

  struct StringBuf *env_buf = string_buf_new(512);
  push_env(env_buf, "OUT", config->cwd);
  if (fields) {
    print_header(config);
    if ((*error = push_fields(config, fields, &env_buf))) {
      string_buf_free(env_buf);
      return EXIT_FAILURE;
    }
  }

  const char *segments[] = {config->root_dir, config->target, "runner"};
  const char *filepath = join(sizeof(segments) / sizeof(char *), segments, '/');
  const char *env = string_buf_cast(env_buf);

  struct StringBuf *command_buf = string_buf_new(1024);
  string_buf_sappend(command_buf, "cd ");
  string_buf_sappend(command_buf, config->root_dir);
  string_buf_cappend(command_buf, '/');
  string_buf_sappend(command_buf, config->target);
  string_buf_sappend(command_buf, " && ");
  string_buf_sappend(command_buf, env);
  string_buf_sappend(command_buf, filepath);
  const char *command = string_buf_cast(command_buf);

  free((void *)env);
  free((void *)filepath);

  int exit_code = system(command);
  free((void *)command);
  return WEXITSTATUS(exit_code);
}
