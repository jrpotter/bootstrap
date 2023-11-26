/**
@file
@brief Error handling.
*/
#ifndef _BOOTSTRAP_ERROR_H
#define _BOOTSTRAP_ERROR_H

#include <stdlib.h>

#include "string_buf.h"

/**
@brief The various error codes produced by `bootstrap`.
*/
enum ErrorCode {
  /// Could not retrieve the value of `$CWD`.
  ERROR_CONFIG_ENV_CWD_INVALID = 1,
  /// No root directory was specified. This must either be set via the command
  /// line flag `-d` or environment variable `$BOOTSTRAP_ROOT_DIR`. If both are
  /// provided, the command line flag takes priority.
  ERROR_CONFIG_ENV_ROOT_DIR_INVALID,
  /// There is no file at the location corresponding to the specified spec.
  ERROR_CONFIG_TARGET_NOT_FOUND,
  /// For generic reasons, `bootstrap` could not access the file at the provided
  /// spec.
  ERROR_CONFIG_TARGET_INVALID,
  /// The file at the location corresponding to the provided spec is not a
  /// directory.
  ERROR_CONFIG_TARGET_NOT_DIR,

  /// For generic reasons, `bootstrap` could not access the `spec.json` file.
  ERROR_PARSER_SPEC_JSON_INVALID,
  /// The `spec.json` file is not valid JSON.
  ERROR_PARSER_SPEC_JSON_INVALID_SYNTAX,

  /// The top-level JSON object of the `spec.json` file is not an object.
  ERROR_VALIDATOR_TOP_LEVEL_NOT_OBJECT,
  /// A field in `spec.json` is not an object.
  ERROR_VALIDATOR_FIELD_NOT_OBJECT,
  /// A field name in `spec.json` is not alphanumeric and beginning with a
  /// non-digit.
  ERROR_VALIDATOR_FIELD_NAME_INVALID,
  /// The `type` of a `spec.json` field is not a string.
  ERROR_VALIDATOR_FIELD_TYPE_INVALID,
  /// The `type` of a `spec.json` field does not correspond to a known prompt
  /// type.
  ERROR_VALIDATOR_FIELD_TYPE_UNKNOWN,
  /// The `prompt` of a `spec.json` field is not a string.
  ERROR_VALIDATOR_FIELD_PROMPT_INVALID,

  /// The `runner` file could not be found.
  ERROR_EVALUATOR_RUNNER_NOT_FOUND,
  /// The `runner` file is not executable.
  ERROR_EVALUATOR_RUNNER_NOT_EXEC,
  /// A user response to a prompt is not valid.
  ERROR_EVALUATOR_RESPONSE_INVALID,
};

/**
@brief A `bootstrap` error.

@see ERROR_NEW
*/
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
@brief Return the number of elements of `__VA_ARGS__`.

Take the `__VA_ARGS__` list and append a list of decreasing numbers
31, 30, ..., 0.

@return
 The number of elements of `__VA_ARGS__`.
*/
#define ALEN(...) \
  ALEN0(          \
    __VA_ARGS__,  \
    0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, \
    0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, \
    0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, \
    0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)

#define ALEN0( \
  _00, _01, _02, _03, _04, _05, _06, _07, \
  _08, _09, _0A, _0B, _0C, _0D, _0F, _0E, \
  _10, _11, _12, _13, _14, _15, _16, _17, \
  _18, _19, _1A, _1B, _1C, _1D, _1E, _1F, \
  ...) _1F

/**
@brief Creates a new @ref Error instance.

It is the responsibility of the caller to free the @ref Error instance.

@param __VA_ARGS__
 Allows supplying up to 31 `const char*` instances. These arguments will be
 concatenated together and supplied to the new @ref Error instance.
@return
 A new @ref Error instance. The caller takes ownership of this value.

@see error_free
*/
#define ERROR_NEW(code, ...) \
  ERROR_NEW0(code, ALEN(__VA_ARGS__), __VA_ARGS__)

#define ERROR_NEW0(code, nargs, ...) \
  priv_error_new((code), (nargs), (const char *[nargs]){__VA_ARGS__})

// clang-format on

#define ANSI_BLACK(...) "\e[0;30m", __VA_ARGS__, "\e[0m"
#define ANSI_RED(...) "\e[0;31m", __VA_ARGS__, "\e[0m"
#define ANSI_GREEN(...) "\e[0;32m", __VA_ARGS__, "\e[0m"
#define ANSI_YELLOW(...) "\e[0;33m", __VA_ARGS__, "\e[0m"
#define ANSI_BLUE(...) "\e[0;34m", __VA_ARGS__, "\e[0m"
#define ANSI_PURPLE(...) "\e[0;35m", __VA_ARGS__, "\e[0m"
#define ANSI_CYAN(...) "\e[0;36m", __VA_ARGS__, "\e[0m"
#define ANSI_WHITE(...) "\e[0;37m", __VA_ARGS__, "\e[0m"

/**
@brief Deallocates a previously allocated @ref Error isntance.

@param error
 A pointer to a @ref Error instance. If null, this function is a no-op.

@see ERROR_NEW
*/
void error_free(struct Error *error);

#endif /* _BOOTSTRAP_ERROR_H */
