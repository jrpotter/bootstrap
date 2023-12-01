{
  description = ''
    An opinionated mix flake.

    To generate a copy of this template elsewhere, install
    [bootstrap](https://github.com/jrpotter/bootstrap) and run:
    ```bash
    $ bootstrap mix
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
          buildMix
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
          lib = buildMix {
            name = "<APP_NAME>";
            src = ./.;
            version = "0.1.0";
            beamDeps = builtins.attrValues deps;
          };

          app = mixRelease {
            pname = "<APP_NAME>";
            src = ./.;
            version = "0.1.0";
            mixNixDeps = deps;
          };

          default = self.packages.${system}.lib;
        };

        devShells.default = pkgs.mkShell {
          packages = [
            elixir
            elixir-ls
            hex
          ] ++ (with pkgs; [
            mix2nix
          ]);
        };
      });
}
