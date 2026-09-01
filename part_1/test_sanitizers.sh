clang -g -O0 \
    -fsanitize=address,undefined \
    -fno-omit-frame-pointer \
    test_sanitizers.c -o test_sanitizers
./test_sanitizers
