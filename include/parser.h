/**
@file
@brief `spec.json` parser.
*/
#ifndef _BOOTSTRAP_PARSER_H
#define _BOOTSTRAP_PARSER_H

#include "cJSON.h"
#include "config.h"
#include "error.h"

/**
@brief Reads in the `spec.json` file relative to the paths of the provided @ref
Config.

A spec directory is not required to contain a `spec.json` file. If this file
cannot be found, the @p parsed pointer is set to NULL and no error is returned.

@param config
 The @ref Config instance describing the `spec` location.
@param parsed
 The out variable a valid `cJSON` instance will be placed into, if parsing is
 successful.
@return
 A null pointer if no error occurs. Otherwise an @ref Error pointer.
*/
struct Error *parse_spec_json(
  const struct Config *const config, cJSON **parsed
);

#endif /* _BOOTSTRAP_PARSER_H */
