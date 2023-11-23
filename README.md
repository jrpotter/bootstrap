# bootstrap

CLI utility for initializing projects in reproducible ways.

## Overview

Within the `specs` directory exists so-called *specs*. A spec is a directory
containing an optional `spec.json` file and a `run.sh` file. The former is
configured like so:

```spec.json
{
  versions: [...],
}
```

The keys of this top-level JSON object correspond to the parameters that are
prompted by the `bootstrap init` curses interface. The value is used to
determine what kind of prompt `bootstrap` provides for the given question.
Possible value types include:

* `[...]` (list)
  * This indicates a select option prompt. The user chooses amongst the values
    specified in the JSON list.

Once all prompts are evaluated, the keys of the object are converted into
uppercase environment variables and passed to the `run.sh` file relative to the
current directory.

## Development

This template includes an `.envrc` file for use with [direnv](https://direnv.net/).

### Formatting

A `pre-commit` file is included in `.githooks` to ensure consistent formatting.
Run the following to configure `git` to use it:

```bash
git config --local core.hooksPath .githooks/
```

If running [direnv](https://direnv.net/), this is done automatically provided
`git` is installed and a repository is initialized.
