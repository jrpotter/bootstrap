# Postgres Flake Template

This is a template for constructing a working environment for local
[Postgres](https://www.postgresql.org/) (version 15.5) usage. [direnv](https://direnv.net/)
can be used to a launch a dev shell upon entering this directory (refer to
`.envrc`). Otherwise run via:
```bash
$> nix develop
```

## Quickstart

An empty Postgres cluster is initialized at `/db`. To start the database, run
the following:
```bash
$> pg_ctl -D db -l db/logfile -o --unix_socket_directories=@postgres start
```
In the above command, `@postgres` refers to an [abstract socket name](https://www.postgresql.org/docs/15/runtime-config-connection.html#GUC-UNIX-SOCKET-DIRECTORIES).
Rename to whatever is appropriate for your use case. To then connect to this
database instance, run:
```bash
$> psql -h @postgres
```
To later shut the database down, run:
```bash
$> pg_ctl -D db stop
```
