rm    -rf ./result
mkdir -p  ./result/miniz_harness
afl-clang-fast                            \
  -Og                                     \
  -g                                      \
  -fsanitize=address,undefined            \
  -o ./result/miniz_harness/miniz_harness \
  -I ./lab_2/source                       \
  -I ./lab_2/source/miniz                 \
     ./lab_2/source/miniz_harness.c
