#ifndef _BOOTSTRAP_EVALUATOR_H
#define _BOOTSTRAP_EVALUATOR_H

#include "config.h"
#include "error.h"
#include "validator.h"

int evaluate_run_sh(
  const struct Config *const config,
  const struct DynArray *const prompts,
  struct Error **error
);

#endif /* _BOOTSTRAP_EVALUATOR_H */
