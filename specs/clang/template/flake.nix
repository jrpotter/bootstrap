{
  description = ''
    An opinionated clang flake.

    To generate a copy of this template elsewhere, run:
    $> bootstrap clang
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
        };
      }
    );
}
