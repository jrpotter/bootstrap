{
  description = "A playground for NiFi.";

  inputs = {
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
          image = pkgs.dockerTools.pullImage {
            finalImageName = "apache/nifi";
            finalImageTag = "1.24.0";
            imageName = "apache/nifi";
            imageDigest = "sha256:dd6fcf708a63d7a43b1e0b1224d3cf754235a7f4e66945119fb2aa6be8d6ee2d";
            sha256 = "sha256-l4beU3yEDGFDXCauIPN7nwhTrKf0rZRGcpKvH+a9lo0=";
            os = "linux";
            arch = "amd64";
          };

          default = self.packages.${system}.image;
        };

        devShells.default = pkgs.mkShell {
          packages = [ pkgs.docker ];
        };
      });
}
