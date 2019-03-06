CC=clang++
CFLAGS=-Wall -Wextra -std=c++17
OUTDIR=.
SRCDIR=./src
OBJDIR=./obj
SOURCE_FILES = ${shell find -type f -path './src/*.cc' -printf "%f\n"}
OBJECT_FILES = ${OBJDIR}/${SOURCE_FILES:.cc=.o}
TARGET = bignum

all: ${TARGET}

clean:
	rm -rf ${OBJECT_FILES}

${OBJDIR}/%.o : ${SRCDIR}/%.cc ${SRCDIR}/%.hpp
	${CC} ${CFLAGS} -c $< -o $@

${OUTDIR}/${TARGET} : ${OBJECT_FILES}
	${CC} ${CFLAGS} $< -o ${TARGET}

.PHONY: clean
