#ifndef _BOOTSTRAP_ERROR_H
#define _BOOTSTRAP_ERROR_H

#include <stdlib.h>

#include "string_buf.h"

enum ErrorCode {
  ERROR_CONFIG_ENV_CWD_INVALID = 1,
  ERROR_CONFIG_ENV_ROOT_DIR_INVALID,
  ERROR_CONFIG_TARGET_NOT_FOUND,
  ERROR_CONFIG_TARGET_INVALID,
  ERROR_CONFIG_TARGET_NOT_DIR,

  ERROR_PARSER_SPEC_JSON_INVALID,
  ERROR_PARSER_SPEC_JSON_INVALID_SYNTAX,

  ERROR_VALIDATOR_TOP_LEVEL_NOT_OBJECT,
  ERROR_VALIDATOR_FIELD_NOT_OBJECT,
  ERROR_VALIDATOR_FIELD_TYPE_INVALID,
  ERROR_VALIDATOR_FIELD_TYPE_UNKNOWN,
  ERROR_VALIDATOR_FIELD_PROMPT_INVALID,

  ERROR_EVALUATOR_RUN_SH_NOT_FOUND,
  ERROR_EVALUATOR_RUN_SH_NOT_EXEC,
  ERROR_EVALUATOR_RESPONSE_INVALID,
};

struct Error {
  enum ErrorCode code;
  const char *message;
};

static inline struct Error *priv_error_new(
  enum ErrorCode code, size_t n, const char *messages[static n]
) {
  struct Error *e = malloc(sizeof(struct Error));
  e->code = code;
  struct StringBuf *sb = string_buf_new(1024);
  for (int i = 0; i < n; ++i) {
    string_buf_sappend(sb, messages[i]);
  }
  e->message = string_buf_convert(sb);
  return e;
}

// clang-format off

/**
Return the number of elements of `__VA_ARGS__`.

Take the `__VA_ARGS__` list and append a list of decreasing numbers
31, 30, ..., 0. Then, by using ALEN0, return the 31st element of that list.
*/
#define ALEN(...) \
  ALEN0(          \
    __VA_ARGS__,  \
    0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, \
    0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, \
    0x0E, 0x0F, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, \
    0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)

#define ALEN0( \
  _00, _01, _02, _03, _04, _05, _06, _07, \
  _08, _09, _0A, _0B, _0C, _0D, _0F, _0E, \
  _10, _11, _12, _13, _14, _15, _16, _17, \
  _18, _19, _1A, _1B, _1C, _1D, _1E, _1F, \
  ...) _1F

/**
Create a new `struct Error` instance.
*/
#define ERROR_NEW(code, ...) \
  ERROR_NEW0(code, ALEN(__VA_ARGS__), __VA_ARGS__)

#define ERROR_NEW0(code, nargs, ...)                                   \
  priv_error_new(                                                      \
    (code), (nargs + 1), (const char * [nargs + 1]){__VA_ARGS__, "\n"} \
  )

// clang-format on

void error_free(struct Error *error);

#endif /* _BOOTSTRAP_ERROR_H */
