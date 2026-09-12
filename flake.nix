{
  description = "ECE4974";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-26.05";

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in {
      packages.${system}.xpdf-asan = pkgs.stdenv.mkDerivation {
        pname = "xpdf-asan";
        version = "3.02";

        src = pkgs.fetchurl {
          url = "https://dl.xpdfreader.com/old/xpdf-3.02.tar.gz";
          sha256 = "szp9VvRUwzGuUJlvmJ6GyRZuV6+Xt03ijN3z1RrBHwA=";
        };

        nativeBuildInputs = [ pkgs.aflplusplus ];

        CC = "${pkgs.aflplusplus}/bin/afl-clang-fast";
        CXX = "${pkgs.aflplusplus}/bin/afl-clang-fast++";
        CFLAGS = "-w -Og -fsanitize=address";
        CXXFLAGS = "-w -Og -fsanitize=address";

        # xpdf-3.02's ./configure predates modern autoconf conventions;
        # mkDerivation's default configurePhase runs ./configure automatically.

        installPhase = ''
          mkdir -p $out
          cp -r . $out
        '';
      };

      devShells.${system}.default = pkgs.mkShell {
        buildInputs = [
          pkgs.aflplusplus
        ];
      };
    };
}
