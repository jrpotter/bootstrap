#ifndef _BOOTSTRAP_LOADER_H
#define _BOOTSTRAP_LOADER_H

#include "cJSON.h"
#include "config.h"

enum SpecParseError {
  // The `spec.json` file exists but cannot be open.
  SPE_PARSE_CANNOT_OPEN = 1,
  // The JSON of the corresponding file is not syntactically valid JSON.
  SPE_PARSE_INVALID,
};

/*
Reads in the `spec.json` file relative to the paths of the provided @Config.

A spec directory does not necessarily contain a `spec.json` file. If this file
cannot be found, the @parsed pointer is set to NULL with a success return code.

@return: 0 on success and a @SpecParseError otherwise.
*/
enum SpecParseError
read_spec_json(const struct Config *const config, cJSON **parsed);

#endif /* _BOOTSTRAP_LOADER_H */
