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

  // TODO: Need to parse the spec.json file.
  fclose(handle);
  assert(false);
}
