#ifndef _BOOTSTRAP_EVALUATOR_H
#define _BOOTSTRAP_EVALUATOR_H

#include "config.h"
#include "validator.h"

enum SpecEvaluationError {
  // The provided input does not match the expected prompt response type.
  SEE_INVALID_PROMPT_RESPONSE = 1
};

enum SpecEvaluationError evaluate_spec_json(
  const struct Config *const config, const struct DynArray *const prompts
);

#endif /* _BOOTSTRAP_EVALUATOR_H */
