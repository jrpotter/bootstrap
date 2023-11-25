#include "parser.h"

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

#include "path.h"

static struct Error *find_spec_json(
  const struct Config *const config, FILE **handle
) {
  const char *segments[] = {config->root_dir, config->target, "spec.json"};
  char *filepath =
    join_path_segments(sizeof(segments) / sizeof(char *), segments);

  struct Error *error = 0;
  // It is ok if the file does not exist. It is not ok if we couldn't open the
  // file for any other reason.
  *handle = fopen(filepath, "r");
  if (!*handle && errno != ENOENT) {
    error = ERROR_NEW(
      ERROR_PARSER_SPEC_JSON_INVALID, config->target, "/spec.json is invalid."
    );
  }

  free(filepath);
  return error;
}

struct Error *parse_spec_json(
  const struct Config *const config, cJSON **parsed
) {
  FILE *handle = 0;
  struct Error *error = find_spec_json(config, &handle);

  if (error) {
    return error;
  }

  // The `spec.json` file does not exist.
  if (!handle) {
    *parsed = 0;
    return 0;
  }

  // For simplicity, load the entire file into memory.
  fseek(handle, 0, SEEK_END);
  size_t file_size = ftell(handle);
  fseek(handle, 0, SEEK_SET);
  char *fcontent = malloc(file_size);
  size_t read_count = fread(fcontent, 1, file_size, handle);
  assert(read_count == file_size);

  *parsed = cJSON_Parse(fcontent);

  free(fcontent);
  fclose(handle);

  // Can use `cJSON_GetErrorPtr()` to get the actual error message.
  if (!*parsed) {
    return ERROR_NEW(
      ERROR_PARSER_SPEC_JSON_INVALID_SYNTAX,
      "The spec.json file contains invalid JSON."
    );
  }

  return 0;
}
