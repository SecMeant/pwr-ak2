CC=gcc
AS=as
CFLAGS=-Wall -Wextra
ASFLAGS=
OUTDIR=.
SRCDIR=./src
OBJDIR=./obj
C_SOURCES = ${shell find -type f -path './src/*.c' -printf "%f\n"}
ASM_SOURCES = ${shell find -type f -path './src/*.s' -printf "%f\n"}
C_OBJECTS = ${OBJDIR}/${C_SOURCES:.c=.o}
ASM_OBJECTS = ${OBJDIR}/${ASM_SOURCES:.s=.o}
OBJECTS = ${C_OBJECTS}
OBJECTS += ${ASM_OBJECTS}
TARGET = crypt

all: ${TARGET}

clean:
	rm -rf ${OBJECT_FILES}

${OBJDIR}/%.o : ${SRCDIR}/%.s
	${AS} ${ASFLAGS} -c $< -o $@

${OBJDIR}/%.o : ${SRCDIR}/%.c
	${CC} ${CFLAGS} -c $< -o $@

${OBJDIR}/%.o : ${SRCDIR}/%.c ${SRCDIR}/%.h
	${CC} ${CFLAGS} -c $< -o $@

${OUTDIR}/${TARGET} : ${OBJECTS}
	${CC} ${CFLAGS} ${OBJECTS} -o ${TARGET}

.PHONY: clean
