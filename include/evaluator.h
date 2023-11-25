#ifndef _BOOTSTRAP_EVALUATOR_H
#define _BOOTSTRAP_EVALUATOR_H

#include "config.h"
#include "validator.h"

enum SpecEvaluationError {
  // Then `run.sh` file could not be found.
  SEE_RUN_SH_NOT_FOUND = 1,
  // The provided input does not match the expected prompt response type.
  SEE_INVALID_PROMPT_RESPONSE,
};

enum SpecEvaluationError evaluate_spec_json(
  const struct Config *const config, const struct DynArray *const prompts
);

#endif /* _BOOTSTRAP_EVALUATOR_H */
