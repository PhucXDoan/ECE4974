let
  pkgs = import ./nixpkgs-pinned.nix;
in
pkgs.stdenv.mkDerivation {
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

  installPhase = ''
    mkdir -p $out
    cp -r . $out
  '';
}
