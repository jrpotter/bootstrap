{
  description = ''
    An opinionated pheonix flake.

    To generate a copy of this template elsewhere, run:
    $> bootstrap phoenix
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

        inherit (pkgs.beam.packages.erlang_25)
          beamPackages
          elixir
          elixir-ls
          hex
          mixRelease;

        deps = import ./deps.nix {
          lib = pkgs.lib;
          inherit beamPackages;
        };
      in
      {
        packages = {
          app = mixRelease {
            pname = "<APP_NAME>";
            src = ./.;
            version = "0.1.0";
            mixNixDeps = deps;
          };

          default = self.packages.${system}.app;
        };

        devShells.default = pkgs.mkShell {
          packages = [
            elixir
            elixir-ls
            hex
          ] ++ (with pkgs; [
            inotify-tools  # For file watching in development.
            mix2nix
            postgresql_15
          ]);
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
