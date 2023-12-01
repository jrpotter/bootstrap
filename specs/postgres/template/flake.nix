{
  description = ''
    An opinionated postgres flake.

    To generate a copy of this template elsewhere, install
    [bootstrap](https://github.com/jrpotter/bootstrap) and run:
    ```bash
    $ bootstrap postgres
    ```
  '';

  inputs = {
    flake-compat.url = "https://flakehub.com/f/edolstra/flake-compat/1.tar.gz";
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            postgresql_15
          ];
          shellHook = ''
            # The server will try to use the data directory named by this
            # environment variable whenever `-D` is not specified (for e.g.
            # `postgres` and `pg_ctl`).
            # https://www.postgresql.org/docs/15/server-start.html
            export PGDATA="$PWD/data"
          '';
        };
      });
}
