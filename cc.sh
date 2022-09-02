#!/bin/bash
[ -z $CC ] && CC=clang

hash $CC || exit 1

[ -z $CFLAGS] && CFLAGS="-O1 -g1 -Isource "
LFLAGS+=" -lcurses "

PLATFORM=$1
[ -z $PLATFORM] && PLATFORM="unix"
FILE_LIST=`ls -f1 $PLATFORM/*c source/*.c` || exit 1

BUILD_FILE=build.c
printf "#include \"%s\"\n" $FILE_LIST > $BUILD_FILE
time $CC $CFLAGS $LFLAGS $BUILD_FILE -o moria
