/**
@file
@brief A `spec` configuration.
*/
#ifndef _BOOTSTRAP_CONFIG_H
#define _BOOTSTRAP_CONFIG_H

#include "error.h"

/**
@brief A collection of parameters that identify a `spec`.

Each member of the @ref Config is expected to outlive the @ref Config instance
itself.
*/
struct Config {
  /// The directory the `bootstrap` command was run from. Does not take
  /// ownership of this pointer.
  const char *cwd;
  /// The root directory housing our specs. Does not take ownership of this
  /// pointer.
  const char *root_dir;
  /// The name of the spec we want to bootstrap. Does not take ownership of
  /// this pointer.
  const char *target;
};

/**
@brief Create a new @ref Config instance.

@param cwd
 The current working directory the `bootstrap` command was invoked from.
@param root_dir
 An absolute path to the collection of specs `bootstrap` will search through.
@param target
 The `spec` that should be bootstrapped.
@return
 A new @ref Config instance. The caller takes ownership of this value.

@see config_free
*/
struct Error *config_new(
  const char *cwd,
  const char *root_dir,
  const char *target,
  struct Config **config
);

/**
@brief Deallocates a previously allocated @ref Config instance.

@param config
 A pointer to a @ref Config instance. If null, this function is a no-op.

@see config_new
*/
void config_free(struct Config *config);

#endif /* BOOTSTRAP_CONFIG_H */
