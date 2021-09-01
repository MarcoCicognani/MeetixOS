#!/bin/sh
. $MEETIX_TOOLCHAIN_DIR/MXbase.sh

# Define build setup
SRC="src"
INC="src"
OBJ="obj"
ARTIFACT_NAME="js"
ARTIFACT_TYPE="command"
COMPILER="i686-pc-meetix-g++"
CFLAGS="-Os -pedantic -std=c++11 -Wall -fstrict-aliasing -Wunused-variable -fomit-frame-pointer -I./src -DDUK_USE_DATE_TZO_GMTIME -lm"
LDFLAGS=""

# Include build tasks
. ../../builder.sh
