# Phoenix Flake Template

This is a template for constructing a environment for Elixir development
(version 1.15.7, Erlang/OTP 25) with the [Phoenix](https://www.phoenixframework.org/)
(version 1.7.10) framework. [direnv](https://direnv.net/) can be used to launch
a dev shell upon entering this directory (refer to `.envrc`). Otherwise run via:
```bash
$ nix develop
```

## Quickstart

An empty Postgres cluster is initialized at `db`. To start the database, run the
following:
```bash
$ pg_ctl -D db -l db/logfile -o --unix_socket_directories=@phoenix start
```
In the above command, `@phoenix` refers to an [abstract socket name](https://www.postgresql.org/docs/15/runtime-config-connection.html#GUC-UNIX-SOCKET-DIRECTORIES).
Rename to whatever is appropriate for your use case. To then connect to this
database instance, run:
```bash
$ psql -h @phoenix
```
To later shut the database down, run:
```bash
$ pg_ctl -D db stop
```

Afterward, you can run the Phoenix setup commands:
```bash
$ mix ecto.setup
$ mix assets.setup
```
and then start the local server:
```bash
$ mix phx.server
```

## Dependencies

This project pins Mix dependencies using [mix2nix](https://github.com/ydlr/mix2nix).
After updating your `mix.lock` file, make sure to re-run the following:
```bash
$ mix2nix > deps.nix
```
As of now, `mix2nix` cannot handle git dependencies found inside the `mix.lock`
file. If you have git dependencies, add them manually or use
[FODs](https://nixos.org/manual/nixpkgs/stable/#packaging-beam-applications).

## Language Server

The [elixir-ls](https://github.com/elixir-lsp/elixir-ls) LSP (version 0.17.10)
is included in this flake.

## Formatting

Formatting depends on the `mix format` task. A `pre-commit` hook is included in
`.githooks` that can be used to format all `*.exs?` files prior to commit.
Install via:
```bash
$ git config --local core.hooksPath .githooks/
```
If running [direnv](https://direnv.net/), this hook is installed automatically
when entering the directory.
