afl-fuzz                  \
    -i ./lab_2/examples   \
    -o ./lab_2/out        \
    -s 123                \
    -m none               \
    -x ./lab_2/dictionary \
    -- ./result/miniz_harness/miniz_harness @@
