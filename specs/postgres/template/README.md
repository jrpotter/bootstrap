# Postgres Flake Template

This is a template for constructing a working environment for local
[Postgres](https://www.postgresql.org/) (version 15.5) usage. [direnv](https://direnv.net/)
can be used to a launch a dev shell upon entering this directory (refer to
`.envrc`). Otherwise run via:
```bash
$> nix develop
```

## Quickstart

To begin, create a new database:
```bash
$> pg_ctl initdb
```
If the flake's default `devShell` is loaded, this will create a database cluster
at `$PWD/data`. To start the database, run the following:
```bash
$> pg_ctl start -o --unix_socket_directories="$PGDATA"
```
To shut the database down, run:
```bash
$> pg_ctl stop
```
You can also specify a different location for the database cluster using the
`-D` option in each of the above commands.
