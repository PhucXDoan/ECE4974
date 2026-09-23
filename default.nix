let

  pkgs = import (fetchTarball {
    url    = "https://github.com/NixOS/nixpkgs/archive/refs/tags/26.05.tar.gz";
    sha256 = "0am8xx09fx5yf2p0wb001v0jx1g5hrfb76h4r37xph378jgk7pcr";
  }) {
    system = "x86_64-linux";
  };

  aflplusplus = pkgs.aflplusplus.overrideAttrs (old: rec {
    version  = "5.03c";
    src      = pkgs.fetchFromGitHub {
      owner  = "AFLplusplus";
      repo   = "AFLplusplus";
      tag    = "v${version}";
      sha256 = "kQ4f/4F3nlPp0wxG7HE3QSJvQYbfRy0BfYJMVGg1o4w=";
    };
    doInstallCheck = false;
  });

in rec {



  devShell = pkgs.mkShell {
    buildInputs = [
      aflplusplus
    ];
  };



  default = pkgs.linkFarm "ECE4974" [
    { name = "pdftotext_asan";       path = pdftotext_asan;       }
    { name = "pdftotext_asan_ubsan"; path = pdftotext_asan_ubsan; }
    { name = "miniz_harness";        path = miniz_harness;        }
  ];



  pdftotext_asan = pkgs.stdenv.mkDerivation {

    name = "pdftotext_asan";
    src  = pkgs.fetchurl {
      url    = "https://dl.xpdfreader.com/old/xpdf-3.02.tar.gz";
      sha256 = "szp9VvRUwzGuUJlvmJ6GyRZuV6+Xt03ijN3z1RrBHwA=";
    };



    # Environmental variables.

    CC       = "${aflplusplus}/bin/afl-clang-fast";
    CXX      = "${aflplusplus}/bin/afl-clang-fast++";
    CFLAGS   = "-w -Og -fsanitize=address";
    CXXFLAGS = "-w -Og -fsanitize=address";



    # The only program we care about is `pdftotext`.

    installPhase = ''
      mkdir -p $out
      cp ./xpdf/pdftotext $out
    '';

  };



  pdftotext_asan_ubsan = pkgs.stdenv.mkDerivation {

    name = "pdftotext_asan_ubsan";
    src  = pkgs.fetchurl {
      url    = "https://dl.xpdfreader.com/old/xpdf-3.02.tar.gz";
      sha256 = "szp9VvRUwzGuUJlvmJ6GyRZuV6+Xt03ijN3z1RrBHwA=";
    };



    # Environmental variables.

    CC       = "${aflplusplus}/bin/afl-clang-fast";
    CXX      = "${aflplusplus}/bin/afl-clang-fast++";
    CFLAGS   = "-w -Og -fsanitize=address,undefined";
    CXXFLAGS = "-w -Og -fsanitize=address,undefined";



    # The only program we care about is `pdftotext`.

    installPhase = ''
      mkdir -p $out
      cp ./xpdf/pdftotext $out
    '';

  };



  miniz_harness = pkgs.stdenv.mkDerivation {

    name = "miniz_harness";
    src  = ./lab_2/source;

    buildPhase = ''
      ${aflplusplus}/bin/afl-clang-fast \
        -Og                             \
        -g                              \
        -fsanitize=address,undefined    \
        -o miniz_harness                \
        -I .                            \
        -I ./miniz                      \
        ./miniz_harness.c
    '';

    installPhase = ''
      mkdir -p $out
      cp -r ./miniz_harness $out
    '';

  };

}
