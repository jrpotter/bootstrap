# spec

CLI utility for initializing projects in reproducible ways.

## Problem

Nix supports the concept of [templates](https://nixos.org/manual/nix/stable/command-ref/new-cli/nix3-flake-init)
out of the box, but the usefulness of the implementation is dubious. In
particular,
```bash
> nix flake init
```
simply copies the files specified in the template directory to the current
local directory, with no means of configuring the copied files. This is
especially useless when working with projects that typically generate files
on initialization (i.e. most projects).

## How This Works

Within the `specs` directory exists so-called *specs*. A spec is a directory
containing a `spec.json` file and a `run.sh` file. The former is configured like
so:

```spec.json
{
  versions: [...],
}
```

The keys of this top-level JSON object correspond to the parameters that are
prompted by the `gen-flake` curses interface. The value is used to determine
what kind of prompt `gen-flake` provides for the given question. Possible value
types are:

* `[...]` (list)
  * This indicates a select option prompt. The user chooses amongst the values
    specified in the JSON list.

Once all prompts are evaluated, the keys of the object are converted into
uppercase environment variables and passed to the `run.sh` file relative to the
current directory.
