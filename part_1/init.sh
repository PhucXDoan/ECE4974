#! /usr/bin/env bash

# Update package index
sudo apt update

# Core toolchain: Clang/LLVM, debugger, Python
sudo apt install -y build-essential clang lld llvm llvm-dev gdb \
    git curl ca-certificates python3 python3-pip python3-dev python3-setuptools

# AFL++ build dependencies (for make distrib)
sudo apt install -y automake cmake flex bison ninja-build cargo \
    libglib2.0-dev libpixman-1-dev libgtk-3-dev



(
    cd ~/Documents
    git clone https://github.com/AFLplusplus/AFLplusplus.git
    cd AFLplusplus

    # Build the core fuzzer + all instrumentations
    make distrib

    # Install to /usr/local (afl-fuzz, afl-clang-fast, afl-cc, ...)
    sudo make install

    # Verify
    echo
    afl-fuzz --help | head -5
    echo
    afl-cc --version
)



# Sanity check versions
echo
cat /etc/os-release   # Expect `Ubuntu 22.04`.
echo
clang --version       # "Expected: Clang 14 or newer (Ubuntu 22.04 ships Clang 14; Ubuntu 24.04 ships Clang 18). Either is fine."
echo
llvm-config --version # "
