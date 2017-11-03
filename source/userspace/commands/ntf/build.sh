#!/bin/sh
. $MEETIX_TOOLCHAIN_DIR/MXbase.sh

# Define build setup
SRC="src"
INC="src"
OBJ="obj"
ARTIFACT_NAME="ntf"
ARTIFACT_TYPE="command"
COMPILER="i686-mx-c++"
CFLAGS="-std=c++11 -I$INC"
LDFLAGS=""

# Include build tasks
. ../../builder.sh
