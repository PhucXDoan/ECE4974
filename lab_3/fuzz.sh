mkdir -p ./lab_3/out

case "$1" in

    0)
        afl-fuzz                                 \
            -i ./lab_3/go-fuzz-corpus/zip/corpus \
            -o ./lab_3/out/0                     \
            -s 123                               \
            -m none                              \
            -V 43200                             \
            -x ./lab_3/dictionary                \
            -- ./result/lab_3_miniz_harnesses/miniz_harness_0 @@
        ;;

    1)
        afl-fuzz                                 \
            -i ./lab_3/go-fuzz-corpus/zip/corpus \
            -o ./lab_3/out/1                     \
            -s 123                               \
            -m none                              \
            -V 43200                             \
            -x ./lab_3/dictionary                \
            -- ./result/lab_3_miniz_harnesses/miniz_harness_1 @@
        ;;

    2)
        afl-fuzz                                 \
            -i ./lab_3/go-fuzz-corpus/zip/corpus \
            -o ./lab_3/out/2                     \
            -s 123                               \
            -m none                              \
            -V 43200                             \
            -x ./lab_3/dictionary                \
            -- ./result/lab_3_miniz_harnesses/miniz_harness_2 @@
        ;;

    3)
        afl-fuzz                                 \
            -i ./lab_3/go-fuzz-corpus/zip/corpus \
            -o ./lab_3/out/3                     \
            -s 123                               \
            -m none                              \
            -V 43200                             \
            -x ./lab_3/dictionary                \
            -- ./result/lab_3_miniz_harnesses/miniz_harness_3 @@
        ;;

esac
