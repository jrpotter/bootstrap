{
  description = ''
    CLI for initializing new projects deterministically with flakes.
  '';

  inputs = {
    flake-compat.url = "https://flakehub.com/f/edolstra/flake-compat/1.tar.gz";
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
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
          app = pkgs.stdenv.mkDerivation {
            pname = "bootstrap";
            src = ./.;
            version = "0.1.3";
            nativeBuildInputs = with pkgs; [ cmake doxygen ];
            installPhase = ''
              mkdir -p $out/bin
              cp ./bootstrap $out/bin
            '';
          };

          default = pkgs.writeShellScriptBin "bootstrap" ''
            if [ -z "$BOOTSTRAP_ROOT_DIR" ]; then
              export BOOTSTRAP_ROOT_DIR="${./specs}"
            fi
            exec ${self.packages.${system}.app}/bin/bootstrap "$@"
          '';
        };
        devShells.default = pkgs.mkShell.override {
          # https://nixos.wiki/wiki/Using_Clang_instead_of_GCC
          stdenv = pkgs.clangStdenv;
        } {
          packages = with pkgs; [
            clang-tools
            cmake
            cmakeCurses
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
