CC=gcc
CXX=g++
AS=as
CFLAGS=-Wall -Wextra -std=c11 -O3 -g3 -fomit-frame-pointer
CXXFLAGS=-Wall -Wextra -std=c++17 -O3 -g3 -fomit-frame-pointer
LDFLAGS=-flto -O3
ASFLAGS=
OUTDIR=.
SRCDIR=./src
OBJDIR=./obj
C_SOURCES = ${shell find -type f -path './src/*.c' -printf "%f\n"}
CXX_SOURCES = ${shell find -type f -path './src/*.cc' -printf "%f\n"}
ASM_SOURCES = ${shell find -type f -path './src/*.s' -printf "%f\n"}
C_OBJECTS = ${C_SOURCES:%.c=${OBJDIR}/%.o}
CXX_OBJECTS = ${CXX_SOURCES:%.cc=${OBJDIR}/%.o}
ASM_OBJECTS = ${ASM_SOURCES:%.s=${OBJDIR}/%.o}
OBJECTS = ${C_OBJECTS}
OBJECTS += ${CXX_OBJECTS}
OBJECTS += ${ASM_OBJECTS}
TARGET = crypt

all: ${TARGET}

clean:
	rm -f ${OBJECTS}
	rm -f crypt

${OBJDIR}/%.o : ${SRCDIR}/%.s
	${AS} ${ASFLAGS} -c $< -o $@

${OBJDIR}/%.o : ${SRCDIR}/%.c ${SRCDIR}/%.h
	${CC} ${CFLAGS} -c $< -o $@

${OBJDIR}/%.o : ${SRCDIR}/%.c
	${CC} ${CFLAGS} -c $< -o $@

${OBJDIR}/%.o : ${SRCDIR}/%.cc
	${CXX} ${CXXFLAGS} -c $< -o $@


${OUTDIR}/${TARGET} : ${OBJECTS}
	${CXX} ${LDFLAGS} ${OBJECTS} -o ${TARGET}

print-% : ; @echo $* = $($*)

.PHONY: clean
