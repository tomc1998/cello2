OUTDIR=build

CXX=g++
LDFLAGS=`llvm-config --ldflags` \
        `llvm-config --libs all` \
        `llvm-config --system-libs`

CFLAGS=`llvm-config --cxxflags | sed 's/-Wcovered-switch-default //;s/-Wstring-conversion //;s/-Werror=unguarded-availability-new //;s/-DNDEBUG //;s/-Wno-class-memaccess//'` \
    -fexceptions \
    -Wall -Werror -g -O0 -std=c++14 -c \
    -Ideps/variant/include \
    -Ideps/string-view-lite/include \
    -Ideps/optional-lite/include

SRC=$(shell find src -type f -iname "*.cpp")
OBJ=$(patsubst src/%.cpp,${OUTDIR}/%.o,${SRC})

DEP = $(OBJ:%.o=%.d)

$(info Obj files: ${OBJ})

ALL_CFLAGS=${CFLAGS} 

# Get a list of source directories to mirror in ${OUTDIR}
SRC_DIRS=$(shell find src -type d)
BIN_DIRS=$(patsubst src/%,${OUTDIR}/%,$(patsubst src,${OUTDIR}/,${SRC_DIRS}))
$(info Bin dirs: ${BIN_DIRS})

ALL: make_out_dirs ${OUTDIR}/out

clean:
	rm -rf ${OUTDIR}

# Include all .d files
-include $(DEP)

${OUTDIR}/out: ${OBJ}
	${CXX} ${LDFLAGS} -o $@ $^ ${ALL_LIBS}

${OUTDIR}/%.o: src/%.cpp
	${CXX} ${ALL_CFLAGS} -MMD -c -o $@ $<

# Makes all dirs
make_out_dirs:
	$(foreach dir,${BIN_DIRS},$(shell mkdir -p "${dir}"))
