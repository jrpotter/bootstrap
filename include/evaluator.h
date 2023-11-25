/**
@file
@brief `spec.json` and `run.sh` evaluator.
*/
#ifndef _BOOTSTRAP_EVALUATOR_H
#define _BOOTSTRAP_EVALUATOR_H

#include "config.h"
#include "error.h"
#include "validator.h"

/**
@brief Run the `run.sh` script found in the configured spec.

@param config
 A reference to the parameters describing the desired spec.
@param fields
 The list of prompts to have answered by the user prior to executing `run.sh`.
 Responses are included as environment variables in the invoked subshell.
@param error
 The out parameter containing a possible error. Set to a null pointer if no
 error occurs. Otherwise set to an @ref Error instance.
@return
 If @p error is set, returns `EXIT_FAILURE`. Otherwise returns the exit code
 returned by `run.sh`.
*/
int evaluate_run_sh(
  const struct Config *const config,
  const struct DynArray *const fields,
  struct Error **error
);

#endif /* _BOOTSTRAP_EVALUATOR_H */
