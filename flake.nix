{
  description = ''
  CLI for initializing new projects deterministically with flakes.
  '';

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        codelldb = pkgs.writeShellScriptBin "codelldb" ''
         exec ${pkgs.vscode-extensions.vadimcn.vscode-lldb}/share/vscode/extensions/vadimcn.vscode-lldb/adapter/codelldb "$@"
        '';
      in
      {
        packages = {
          bootstrap = pkgs.stdenv.mkDerivation {
            pname = "bootstrap";
            src = ./.;
            version = "0.1.0";
            nativeBuildInputs = with pkgs; [
              clang
            ];

            buildPhase = ''
              make release
            '';
          };

          default = self.packages.${system}.bootstrap;
        };
        devShells.default = pkgs.mkShell.override {
          # https://nixos.wiki/wiki/Using_Clang_instead_of_GCC
          stdenv = pkgs.clangStdenv;
        } {
          packages = with pkgs; [
            bear
            clang-tools
            codelldb
            doxygen
          ];
          shellHook = ''
            export BOOTSTRAP_ROOT_DIR="${./specs}"
          '';
        };
      }
    );
}
