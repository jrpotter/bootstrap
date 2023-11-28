# Postgres Flake Template

This is a template for constructing a working environment for local
[Postgres](https://www.postgresql.org/) (version 15.5) usage. [direnv](https://direnv.net/)
can be used to a launch a dev shell upon entering this directory (refer to
`.envrc`). Otherwise run via:
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
