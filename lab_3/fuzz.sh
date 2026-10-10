afl-fuzz                                 \
    -i ./lab_3/go-fuzz-corpus/zip/corpus \
    -o ./lab_3/out                       \
    -s 123                               \
    -m none                              \
    -x ./lab_3/dictionary                \
    -V 28800                             \
    -- ./result/lab_3_miniz_harness/miniz_harness @@
