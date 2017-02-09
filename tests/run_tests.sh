#!/bin/bash
# Pragmatic compilation and running of tests

cd ..
make
cd tests


CC="gcc"
LINKER="gcc -o"

SRCDIR=../src
OBJDIR=../obj
BINDIR=../bin
INCDIR=../include


CFLAGS="-Wall -I. -g -O3 -ffast-math -fomit-frame-pointer -I$INCDIR -I$SRCDIR"
LFLAGS='-lm'

# OBJS=`ls $OBJDIR | grep -v main.c`
# echo $OBJS

# $CC $CFLAGS -c tests.c -o tests.o
# $LINKER tests.x $LFLAGS

