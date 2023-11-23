#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

#include "loader.h"
#include "path.h"

/*
Returns a file pointer to the `spec.json` file if it exists. Returns 0
otherwise.
*/
static int find_spec_json(const struct Config *const config, FILE **handle) {
  const char *segments[] = {config->root_dir, config->target, "spec.json"};
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

enum SpecJsonError
read_spec_json(const struct Config *const config, cJSON **parsed) {
  FILE *handle = 0;
  int retval = find_spec_json(config, &handle);

  if (retval != 0) {
    return SJE_JSON_CANNOT_OPEN;
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
    return SJE_JSON_INVALID;
  }

  return 0;
}
