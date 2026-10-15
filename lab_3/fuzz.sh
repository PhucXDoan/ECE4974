#! /usr/bin/env bash

mkdir -p ./lab_3/out

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <MODE_NUMBER>" >&2
    exit 1
fi

MODE_NUMBER="$1"

export ASAN_OPTIONS=abort_on_error=1:symbolize=0:allocator_may_return_null=1

case "$MODE_NUMBER" in

    0)
        afl-fuzz                                 \
            -i "./lab_3/corpus_minimized"        \
            -o "./lab_3/out/$MODE_NUMBER"        \
            -s 123                               \
            -m none                              \
            -V 86400                             \
            -x ./lab_3/dictionary                \
            -- ./result/lab_3_miniz_harnesses/miniz_harness_0 @@
        ;;

    1)
        afl-fuzz                                 \
            -i "./lab_3/corpus_minimized"        \
            -o "./lab_3/out/$MODE_NUMBER"        \
            -s 123                               \
            -m none                              \
            -V 86400                             \
            -x ./lab_3/dictionary                \
            -- ./result/lab_3_miniz_harnesses/miniz_harness_1 @@
        ;;

    2)
        afl-fuzz                                 \
            -i "./lab_3/corpus_minimized"        \
            -o "./lab_3/out/$MODE_NUMBER"        \
            -s 123                               \
            -m none                              \
            -V 86400                             \
            -x ./lab_3/dictionary                \
            -- ./result/lab_3_miniz_harnesses/miniz_harness_2 @@
        ;;

    3)
        afl-fuzz                                 \
            -i "./lab_3/corpus_minimized"        \
            -o "./lab_3/out/$MODE_NUMBER"        \
            -s 123                               \
            -m none                              \
            -V 86400                             \
            -x ./lab_3/dictionary                \
            -- ./result/lab_3_miniz_harnesses/miniz_harness_3 @@
        ;;

    4)
        afl-fuzz                                                     \
            -i "./lab_3/corpus_minimized"                            \
            -o "./lab_3/out/$MODE_NUMBER"                            \
            -s 123                                                   \
            -m none                                                  \
            -V 86400                                                 \
            -c ./result/lab_3_miniz_harnesses/miniz_harness_4.cmplog \
            -l 2AT                                                   \
            -x ./lab_3/dictionary                                    \
            -- ./result/lab_3_miniz_harnesses/miniz_harness_4 @@
        ;;

esac
