# bootstrap

CLI utility for defining custom project initialization scripts.

TODO:
- [ ] Add evaluator tests.
- [ ] Color output to console.

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

We start with an example. Consider the following *spec*, which we'll name
`example`:
```json
{
  "filename": {
    "type": "STRING",
    "prompt": "What file should I create for you? "
  }
}
```
and its associated *builder*:
```bash
#!/usr/bin/env bash

echo "Creating $FILENAME"
touch "$OUT/$FILENAME"
```

Running `bootstrap` with these two files configured will invoke the following
interactive script:
```bash
> bootstrap example
I: What file should I create for you? hello-world.txt
O: Creating hello-world.txt

>
```
Here the line prefixed with `I:` indicates a prompt that must be answered by
the user. The line prefixed with `O:` indicates the output of the builder
script. You should now see a new `hello-world.txt` file in your current working
directory.

## Usage

TODO

### Installation

TODO

### Specs and Builders

TODO

### Other Environment Variables

TODO

### Supplied Specs

TODO

### Using With Nix

TODO

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
