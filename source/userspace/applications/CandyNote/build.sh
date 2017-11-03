#!/bin/sh
. $MEETIX_TOOLCHAIN_DIR/MXbase.sh

# Define build setup
SRC="src"
INC="src"
OBJ="obj"
ARTIFACT_NAME="CandyNote"
ARTIFACT_TYPE="application"
COMPILER="i686-mx-g++"
CFLAGS="-std=c++11 -I$SRC -I$SYSROOT/include/freetype2"
LDFLAGS="-lcairo -lfreetype -lpixman-1 -lpng -lz"

# Include build tasks
. ../../builder.sh
