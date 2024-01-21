{
  description = ''
    An Airflow dev shell.

    To generate a copy of this template elsewhere, install
    [bootstrap](https://github.com/jrpotter/bootstrap) and run:
    ```bash
    $ bootstrap airflow
    ```
  '';

  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          AIRFLOW_HOME = "$PWD/airflow";
          AIRFLOW__CORE__DAGS_FOLDER = "$PWD/dags";
          LD_LIBRARY_PATH = "${pkgs.stdenv.cc.cc.lib}/lib";

          packages = (with pkgs; [
            apache-airflow
          ] ++ (with python311Packages; [
            black
            debugpy
            mccabe
            mypy
            pycodestyle
            pyflakes
            pyls-isort
            python-lsp-black
            python-lsp-server
          ]));
        };
      });
}
