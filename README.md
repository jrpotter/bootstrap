# bootstrap

CLI utility for defining custom project initialization scripts.

TODO:
- [ ] Add evaluator tests.
- [ ] Color output to console.
- [ ] Ensure keys are alphanumeric, underscore

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
    "type": "text",
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
$> bootstrap touch
What file should I create for you? hello-world.txt
Creating hello-world.txt
$> ls
... hello-world.txt ...
```
You should now see a new `hello-world.txt` file in your current working
directory.

## Usage

### Installation

TODO

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
    "type": "text",
    "prompt": "Prompt for field> "
  },
  ...
}
```
In this example, the first field is called `"fieldname"`. `bootstrap` sees this
field and writes the prompt `"Prompt for field> "` to `stdout`. Since
`"fieldname"` has type `"text"`, `bootstrap` will wait for the user to input
a string (submitted with a newline).

If the user were to enter, say `fieldvalue`, in response to the prompt,
the `runner` script would then have access to an environment variable
`FIELDNAME` set to `fieldvalue` on launch.

#### Types

The value of `type` determines how a field is prompted for. Note the value of
`type` is case insenstive. The currently supported list of types are:

* `text`
  * The simplest prompt type. Takes in a free-form response submitted after a
    newline (`\n`) is encountered.

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
reflect my personal needs for projects. Feel free to specify a different specs
root directory if these do not fit your needs.

As a suggestion, use `nix` from within your `runner` scripts for maximum
reproducibility. Refer to the provided specs for inspiration on how you can do
this.

## Development

TODO

### Documentation

TODO

### Formatting

A `pre-commit` file is included in `.githooks` to ensure consistent formatting.
Run the following to configure `git` to use it:

```bash
git config --local core.hooksPath .githooks/
```

If running [direnv](https://direnv.net/), this is done automatically if `git` is
installed.
