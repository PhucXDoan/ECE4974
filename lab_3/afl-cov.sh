#! /usr/bin/env bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <FUZZ_OUT_PATH>" >&2
    exit 1
fi

MODE_NUMBER="$1"

cd "./lab_3/out/$MODE_NUMBER"

gcc                              \
  -O0                            \
  -coverage                      \
  -fprofile-arcs                 \
  -ftest-coverage                \
  -o miniz_harness_coverage      \
  -D MODE=0                      \
  -I .                           \
  -I ../../source                \
  -I ../../source/miniz          \
  ../../source/miniz_harness.c

../../../afl-cov/afl-cov                               \
    -d .                                               \
    --coverage-cmd "./miniz_harness_coverage AFL_FILE" \
    --code-dir .                                       \
    --enable-branch-coverage                           \
    --overwrite                                        \
    --readelf-path $(which readelf)                    \
    --lcov-path    $(which lcov   )                    \
    --genhtml-path $(which genhtml)
