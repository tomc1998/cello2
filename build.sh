#!/bin/bash
mkdir -p build
g++ -Wall -Werror -g -O0 -std=c++14 -o build/out src/main.cpp ${CFLAGS} \
    -Ideps/variant/include \
    -Ideps/string-view-lite/include \
    -Ideps/optional-lite/include \
