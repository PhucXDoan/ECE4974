# AFL++ instrumentation + ASan
AFL_USE_ASAN=1 afl-clang-fast \
    -g -O1 crash_me.c -o crash_me

# Seed corpus
mkdir -p in out
printf "test\n" > in/seed

# Launch the fuzzer
afl-fuzz -i in -o out -- ./crash_me
