{
  description = ''
  An opinionated mix flake.

  To generate a copy of this template elsewhere, run:
  > nix flake init -t github:jrpotter/flake-templates#mix
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
          tmpl-lib = buildMix {
            name = "tmpl";
            src = ./.;
            version = "0.0.0";
            beamDeps = builtins.attrValues deps;
          };

          tmpl-app = mixRelease {
            pname = "tmpl";
            src = ./.;
            version = "0.0.0";
            mixNixDeps = deps;
          };

          default = self.packages.${system}.tmpl-lib;
        };

        devShells.default = pkgs.mkShell {
          inputsFrom = [
            self.packages.${system}.tmpl-app
            self.packages.${system}.tmpl-lib
          ];
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
