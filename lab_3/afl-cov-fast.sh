#! /usr/bin/env bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <MODE>" >&2
    exit 1
fi

MODE_NUMBER="$1"

cd "./lab_3/out/$MODE_NUMBER"

gcc                              \
  -O2                            \
  -coverage                      \
  -fprofile-arcs                 \
  -ftest-coverage                \
  -o miniz_harness_coverage      \
  -D MODE=0                      \
  -I .                           \
  -I ../../source                \
  -I ../../source/miniz          \
  ../../source/miniz_harness.c

../../../afl-cov-fast/afl-cov-fast.py            \
    -m gcc                                       \
    --code-dir "."                               \
    --afl-fuzzing-dir "."                        \
    --coverage-cmd "./miniz_harness_coverage @@" \
    -j8                                          \
    --overwrite
