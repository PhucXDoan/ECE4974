afl-fuzz                  \
    -i ./lab_3/examples   \
    -o ./lab_3/out        \
    -s 123                \
    -m none               \
    -x ./lab_3/dictionary \
    -- ./result/lab_3_miniz_harness/miniz_harness @@
