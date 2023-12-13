# bootstrap

CLI utility for defining custom project initialization scripts.

## Overview

`bootstrap` is a tool for quickly defining your own init-like scripts. If you
are familiar with tools like

* `npm init`
* `nix flake init`
* `django-admin startproject`
* `mix phx.new`
* etc.

this project will feel at home. Ultimately the goal is to create (optionally)
interactive scripts like those mentioned in the above list to quickly scaffold
your new projects in a consistent way.

---

We start with an example. Consider the following *spec*, which we'll name
`touch` (this example exists as a [pre-packaged spec](./specs/touch)):
```json
{
  "filename": {
    "type": "line",
    "prompt": "What file should I create for you? "
  }
}
```
and its associated *runner*:
```bash
#!/usr/bin/env bash

echo "Creating $FILENAME"
touch "$OUT/$FILENAME"
```

Running `bootstrap` with these two files configured will invoke the following
interactive script:
```bash
$ bootstrap touch
What file should I create for you? hello-world.txt
Creating hello-world.txt
$ ls
... hello-world.txt ...
```
You should now see a new `hello-world.txt` file in your current working
directory.

## Installation

### Nix

It is recommended you use Nix to install `bootstrap`. If using a flake, specify
`bootstrap` as an `inputs` attribute the normal way. Otherwise, if you have a
new enough version of nix, import the executable like so:
```nix
(builtins.getFlake "github:jrpotter/bootstrap/${version}").packages.${system}.default;
```
If flakes is not enabled or your nix version does not support
`builtins.getFlake`, you can instead use:
```nix
(import (pkgs.fetchFromGitHub {
  owner = "jrpotter";
  repo = "bootstrap";
  rev = "${version}";
  sha256 = "${sha256}";
})).packages.${system}.default;
```

### Source

If you do not have Nix or prefer building from source, clone this repository and
run
```bash
$ make BUILD=release
```
The `bootstrap` binary will be made available in `dist/release` by default.

## Usage

### Runners

A spec refers to any directory containing a file named `runner`. The only
requirement enforced by `bootstrap` is for this file to be an executable (e.g.
`chmod +x`), but typically the `runner` is a shell script:
```bash
#!/usr/bin/env bash
...
```

The `runner` is invoked with its current working directory set to that of the
directory containing it. For instance, if we have a `runner` script living in
directory `~/Documents/specs/example` with contents:
```bash
#!/usr/bin/env bash
echo "$PWD"
```
the output of `bootstrap example` will *always* be e.g.
```bash
> bootstrap example
/home/jrpotter/Documents/specs/example
```
regardless of where we call the `bootstrap` command.

#### Exit Code

`bootstrap` always invokes the `runner` using the system `sh` command:
```bash
> /bin/sh sh -c ./runner
```
The exit code emitted by `bootstrap` will mirror that returned by the `runner`
executable.

### Specs

If interested in making the `runner` more flexible, you can provide different
environment variables in the form of a `spec.json` file. This file must live
in the same spec as the `runner`. When invoking `bootstrap`, the file is used to
determine what prompts should be displayed to the user before executing the
`runner` file. The user's responses are then included as environment variables
to the `runner` process.

The file contents should consist of a top-level JSON object and any number of
child objects called *fields*. A typical `spec.json` file looks like:
```json
{
  "fieldname": {
    "type": "line",
    "prompt": "Prompt for field> "
  },
  ...
}
```
In this example, the first field is called `"fieldname"`. `bootstrap` sees this
field and writes the prompt `"Prompt for field> "` to `stdout`. Since
`"fieldname"` has type `"line"`, `bootstrap` will wait for the user to input
a string (submitted with a newline).

If the user were to enter `fieldvalue` in response to the prompt, the `runner`
script would then have access to an environment variable `FIELDNAME` set to
`fieldvalue`. Field names should respect the [POSIX standard](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap08.html)
on environment variable naming. That is, field names must consist solely of
alphanumeric characters or underscores and are not permitted to start with a
digit.

#### Types

The value of `type` determines how a field is prompted for. Note the value of
`type` is case insenstive. The currently supported list of types are:

* `line`
  * Takes in a free-form response submitted after encountering a newline (`\n`).
    The resulting environment variable has leading and trailing whitespace
    trimmed.
  * If `required`, whitespace-only strings are re-prompted.
* `yes`
  * Takes in any of `"yes"`, `"y"`, `"no"`, and `"n"`. Answers are case
    insensitive.
  * Even if not `required`, any answer that does not match one of these patterns
    is re-prompted.
  * A value of `"yes"` has an environment variable with value `1` passed to the
    runner. A value of `"no"` has an environment variable with a null value
    (i.e. an empty string) passed to the runner.

#### Options

Additional options can be included in a field definition.

* `required`
  * A value must be specified. How this option is interpreted depends on `type`.
  * Defaults to `true`.

#### Root Directory

All specs should exist in the same *root directory*. As an example of what this
directory might look like, refer to `specs` at the top-level of this project.
When invoking `bootstrap <name>`, `<name>` is expected to correspond to some
spec found within the root directory.

To tell `bootstrap` where your specs are located, you can provide the path to
the root directory using the `-d` option like so:
```bash
> bootstrap -d ~/Documents/specs example
...
```

If no option is set, `bootstrap` will fallback to using the value of the
`BOOTSTRAP_ROOT_DIR` environment variable. If this also isn't set, `bootstrap`
will abort with an appropriate error message.

### Other Environment Variables

By default, the `runner` command will have the following environment variables
defined. Defining these fields in a `spec.json` file will override the default
values:

* `OUT`
  * The directory `bootstrap` was invoked from. Named since this is usually
    where you want to initialize new files of your project in.

### Supplied Specs

A number of specs are provided out of the box. If you installed `bootstrap`
using `nix`, the `BOOTSTRAP_ROOT_DIR` will automatically be set to the location
of these [specs](./specs). Keep in mind this list is very opinionated - they
reflect my personal needs for projects. Feel free to specify a different root
directory if these do not fit your needs.

As a suggestion, use `nix` from within your `runner` scripts for maximum
reproducibility. Refer to the provided specs for inspiration on how you can do
this.

## Development

This tool was originally written for personal usage and, as such, any
functionality (or lack thereof) reflects my own needs as I have come across
them. If interested in adding more capabilities, please send a PR or just fork
the project for your own purposes.

### Testing

We use [Sput](https://www.use-strict.de/sput-unit-testing/) for unit tests. To
run tests, type:
```bash
$ make test
```
Tests are located in the `test` directory. `test/suites.c` serves as the
entrypoint for the test runner.

### Documentation

We use [doxygen](https://www.doxygen.nl/index.html) for documentation
generation. Run either of the following two commands to generate documentation
locally:
```bash
$ make docs
$ doxygen
```

### Formatting

We use `clang-format` to ensure consistent formatting. A `pre-commit` file is
included in `.githooks` to enforce usage. Run the following to configure `git`
to use it:

```bash
git config --local core.hooksPath .githooks/
```

If running [direnv](https://direnv.net/), this is done automatically upon
entering the project directory.
