#ifndef _BOOTSTRAP_PARSER_H
#define _BOOTSTRAP_PARSER_H

#include "cJSON.h"
#include "config.h"
#include "error.h"

/**
Reads in the `spec.json` file relative to the paths of the provided @Config.

A spec directory does not necessarily contain a `spec.json` file. If this file
cannot be found, the @parsed pointer is set to NULL with a success return code.
*/
struct Error *parse_spec_json(
  const struct Config *const config, cJSON **parsed
);

#endif /* _BOOTSTRAP_PARSER_H */
