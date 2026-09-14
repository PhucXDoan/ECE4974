let

  pkgs = import (fetchTarball {
    url    = "https://github.com/NixOS/nixpkgs/archive/8c50a710ddca43d7a530fb805ad55bde8d0141c5.tar.gz";
    sha256 = "0am8xx09fx5yf2p0wb001v0jx1g5hrfb76h4r37xph378jgk7pcr";
  }) {
    system = "x86_64-linux";
  };

in rec {



  default = pkgs.linkFarm "builds" [
    { name = "pdftotext_asan";       path = pdftotext_asan;       }
    { name = "pdftotext_asan_ubsan"; path = pdftotext_asan_ubsan; }
  ];



  pdftotext_asan = pkgs.stdenv.mkDerivation {

    name = "pdftotext_asan";
    src  = pkgs.fetchurl {
      url    = "https://dl.xpdfreader.com/old/xpdf-3.02.tar.gz";
      sha256 = "szp9VvRUwzGuUJlvmJ6GyRZuV6+Xt03ijN3z1RrBHwA=";
    };



    # Environmental variables.

    CC       = "${pkgs.aflplusplus}/bin/afl-clang-fast";
    CXX      = "${pkgs.aflplusplus}/bin/afl-clang-fast++";
    CFLAGS   = "-w -Og -fsanitize=address";
    CXXFLAGS = "-w -Og -fsanitize=address";



    # The only program we care about is `pdftotext`.

    installPhase = ''
      mkdir -p $out/
      cp ./xpdf/pdftotext $out/
    '';

  };



  pdftotext_asan_ubsan = pkgs.stdenv.mkDerivation {

    name = "pdftotext_asan_ubsan";
    src  = pkgs.fetchurl {
      url    = "https://dl.xpdfreader.com/old/xpdf-3.02.tar.gz";
      sha256 = "szp9VvRUwzGuUJlvmJ6GyRZuV6+Xt03ijN3z1RrBHwA=";
    };



    # Environmental variables.

    CC       = "${pkgs.aflplusplus}/bin/afl-clang-fast";
    CXX      = "${pkgs.aflplusplus}/bin/afl-clang-fast++";
    CFLAGS   = "-w -Og -fsanitize=address,undefined";
    CXXFLAGS = "-w -Og -fsanitize=address,undefined";



    # The only program we care about is `pdftotext`.

    installPhase = ''
      mkdir -p $out/
      cp ./xpdf/pdftotext $out/
    '';

  };



}
