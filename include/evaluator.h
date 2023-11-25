#ifndef _BOOTSTRAP_EVALUATOR_H
#define _BOOTSTRAP_EVALUATOR_H

#include "config.h"
#include "error.h"
#include "validator.h"

struct Error *evaluate_spec_json(
  const struct Config *const config, const struct DynArray *const prompts
);

#endif /* _BOOTSTRAP_EVALUATOR_H */
