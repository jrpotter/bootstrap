{
  description = ''
    An opinionated nodejs flake.

    To generate a copy of this template elsewhere, install
    [bootstrap](https://github.com/jrpotter/bootstrap) and run:
    ```bash
    $ bootstrap nodejs
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
        packages = {
          lib = pkgs.buildNpmPackage {
            pname = "<NAME>";
            version = "0.1.0";
            src = ./.;
            npmDepsHash = "<SHA_256>";
            # npmDepsHash = pkgs.lib.fakeHash;

            # Needed to properly invoke npm run build.
            nativeBuildInputs = [ pkgs.typescript ];

            installPhase = ''
              mkdir $out
              cp dist/index.js $out
            '';
          };

          default = self.packages.${system}.lib;
        };

        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            nodePackages.prettier
            nodePackages.typescript-language-server
            nodejs
            prefetch-npm-deps
            typescript
          ];
        };
      });
}
