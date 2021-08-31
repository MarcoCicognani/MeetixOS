#!/bin/sh
. $MEETIX_TOOLCHAIN_DIR/MXbase.sh

# Define build setup
SRC="src"
INC="src"
OBJ="obj"
ARTIFACT_NAME="gzip"
ARTIFACT_TYPE="command"
COMPILER="i686-mx-gcc"
CFLAGS="-std=c11 -I$SRC"
LDFLAGS="-lz"

# Include build tasks
. ../../builder.sh
