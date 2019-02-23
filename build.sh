#!/bin/bash
set -x
mkdir -p build

g++ `llvm-config --cxxflags | sed 's/-Wcovered-switch-default //;s/-Wstring-conversion //;s/-Werror=unguarded-availability-new //;s/-DNDEBUG //'` \
    -fexceptions \
    -Wall -Werror -g -O0 -std=c++14 -c \
    -o build/out.o src/main.cpp ${CFLAGS} \
    -Ideps/variant/include \
    -Ideps/string-view-lite/include \
    -Ideps/optional-lite/include && \
g++ `llvm-config --ldflags` -o build/out build/out.o \
    `llvm-config --libs all`
