{
  description = ''
    An opinionated pheonix flake.

    To generate a copy of this template elsewhere, install
    [bootstrap](https://github.com/jrpotter/bootstrap) and run:
    ```bash
    $ bootstrap phoenix
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

        inherit (pkgs.beam.packages.erlang_25)
          beamPackages
          elixir
          elixir-ls
          hex
          mixRelease;

        nodeDependencies = (
          pkgs.callPackage ./assets/default.nix {}
        ).nodeDependencies;

        tailwindcss = pkgs.nodePackages.tailwindcss.overrideAttrs (oa: {
          plugins = [
            pkgs.nodePackages."@tailwindcss/forms"
          ];
        });
      in
      {
        packages = {
          app = mixRelease {
            pname = "<APP>";
            src = ./.;
            version = "0.1.0";
            mixNixDeps = import ./deps.nix {
              lib = pkgs.lib;
              inherit beamPackages;
            };

            # Enable if using distributed Erlang.
            # https://github.com/NixOS/nixpkgs/issues/166229
            removeCookie = false;

            # https://hexdocs.pm/esbuild/Esbuild.html
            # https://hexdocs.pm/tailwind/Tailwind.html
            postBuild = ''
              ln -s ${nodeDependencies}/lib/node_modules assets/node_modules
              export MIX_ESBUILD_PATH=${pkgs.esbuild}/bin/esbuild
              export MIX_TAILWIND_PATH=${tailwindcss}/bin/tailwind
              mix do deps.loadpaths --no-deps-check, assets.deploy
              mix phx.gen.release
            '';
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
            node2nix
            nodePackages.prettier
            nodePackages.typescript-language-server
            nodejs
            postgresql_15
            typescript
          ]);
          shellHook = ''
            # The server will try to use the data directory named by this
            # environment variable whenever `-D` is not specified (for e.g.
            # `postgres` and `pg_ctl`).
            # https://www.postgresql.org/docs/15/server-start.html
            export PGDATA="$PWD/db"
          '';
        };
      });
}
