#!/bin/bash
[ -z $CC ] && CC=clang

hash $CC || exit 1

[ -z $CFLAGS] && CFLAGS="-O1 -g1 -Isource "
LFLAGS+=" -lcurses "

TARGET_FILE=${1:-moria.c}
PLATFORM=${2:-unix}

[ -z $PLATFORM ] && PLATFORM="unix"
FILE_LIST=`ls -f1 $PLATFORM/*c source/*.c` || exit 1

[ ! -f moria.c ] && printf "#include \"%s\"\n" $FILE_LIST > moria.c

OUT_FILE=`basename $TARGET_FILE .c`
time $CC $TARGET_FILE $CFLAGS $LFLAGS -o $OUT_FILE
