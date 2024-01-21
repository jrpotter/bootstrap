# Airflow Dev Shell

This is a small flake template for experimenting with [Apache Airflow](https://airflow.apache.org/)
(version 2.7.3). [direnv](https://direnv.net/) can be used to a launch a dev
shell upon entering this directory (refer to `.envrc`). Otherwise run via:
```bash
$ nix develop
```
Once your environment has booted, you can get started with the UI by running:
```bash
$ airflow standalone
```
The webserver lives at `localhost:8080` by default. The username is `admin` and
the generated password can be found by running:
```bash
$ cat airflow/standalone_admin_password.txt
```
or looking through the console output.

A basic DAG is included in the top-level `dags/` directory. Airflow discovers
this DAG with the help of the `AIRFLOW__CORE__DAGS_FOLDER` environment variable,
set within the nix dev shell.
