# Phoenix Flake Template

This is a template for constructing a environment for Elixir development
(version 1.15.7, Erlang/OTP 25) with the [Phoenix](https://www.phoenixframework.org/)
(version 1.7.10) framework. [direnv](https://direnv.net/) can be used to launch
a dev shell upon entering this directory (refer to `.envrc`). Otherwise run via:
```bash
$> nix develop
```

## Quickstart

An empty Postgres cluster is initialized at `/data`. To start the database, run
the following:
```bash
$> pg_ctl start -o --unix_socket_directories="$PGDATA"
```
To shut the database down, run:
```bash
$> pg_ctl stop
```
You can connect to this database from the project root directory by running:
```bash
$> psql -h "$PWD"/data -d postgres
```

Afterward, you can run the Phoenix setup commands:
```bash
$> mix ecto.setup
$> mix assets.setup
```
and then start the local server:
```bash
$> mix phx.server
```

## Language Server

The [elixir-ls](https://github.com/elixir-lsp/elixir-ls) LSP (version 0.17.10)
is included in this flake.

## Dependencies

This project pins Mix dependencies using [mix2nix](https://github.com/ydlr/mix2nix).
After updating your `mix.lock` file, make sure to re-run the following:
```bash
mix2nix > deps.nix
```
As of now, `mix2nix` cannot handle git dependencies found inside the `mix.lock`
file. If you have git dependencies, add them manually or use
[FODs](https://nixos.org/manual/nixpkgs/stable/#packaging-beam-applications).
