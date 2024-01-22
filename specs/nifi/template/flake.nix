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
            imageName = "apache/nifi";
            imageDigest = "sha256:8819e20e3e02484fe765790b28b4a5e8c043f50c341a1778956073149774ebd1";
            finalImageName = "nifi";
            finalImageTag = "1.19.0";
            # docker save image -o <output>
            # nix hash --base64 --type sha256 <output>
            sha256 = "sha256-BmRUHDhRpRief5oxjlxYgnBn9oPmiXKs2ojyV3sm4M0=";
            # docker manifest inspect apache/nifi:1.19.0
            os = "linux";
            arch = "amd64";
          };

          default = self.packages.${system}.image;
        };

        devShells.default = pkgs.mkShell {
          # docker run -p 8443:8443 nifi:1.19.0
          packages = [ pkgs.docker ];
        };
      });
}
