#!/bin/bash

set -e

# Colors
if [[ -n "$TERM" && "$TERM" != "dumb" ]]; then
    RED=$(tput setaf 1)
    RESET=$(tput sgr0)
fi

PACKAGE=$1
SOURCE_DIR=$(realpath ..)
SOURCE_LIBS_DIR="$SOURCE_DIR/Libs"
BUILD_PORT_ROOT="$SOURCE_DIR/Build/Ports"
TOOLCHAIN_ROOT="$SOURCE_DIR/Toolchain/Local"
BUILD_IN_SOURCE_DIR=0

EXTRA_INCLUDE_PATHS=(
    "$TOOLCHAIN_ROOT/i686-pc-meetix/include"
    "$TOOLCHAIN_ROOT/i686-pc-meetix/include/c++/11.2.0"
    "$TOOLCHAIN_ROOT/i686-pc-meetix/include/c++/11.2.0/i686-pc-meetix"
    "$TOOLCHAIN_ROOT/i686-pc-meetix/include/c++/11.2.0/backward"
    "$TOOLCHAIN_ROOT/lib/gcc/i686-pc-meetix/11.2.0/include"
    "$TOOLCHAIN_ROOT/lib/gcc/i686-pc-meetix/11.2.0/include-fixed"
    "$TOOLCHAIN_ROOT/include"
    "$SOURCE_LIBS_DIR"
    "$SOURCE_LIBS_DIR/LibMath"
    "$SOURCE_LIBS_DIR/LibApi"
    "$SOURCE_LIBS_DIR/LibC"
)
EXTRA_OPTIONS=(
    -m32
    -nostdinc
    -Wdouble-promotion
    -fdiagnostics-color=always
)
EXTRA_LINK_DIRS=(
    "$SOURCE_DIR/Build/Release/Libs/LibC"
    "$SOURCE_DIR/Build/Release/Libs/LibMath"
    "$SOURCE_DIR/Build/Release/Libs/LibTC"
)
EXTRA_LIBS=(
    LibC
    LibMath
    STC
    stdc++
)

# Fails with the given error message and exits
fail() {
    echo "${RED}Failure${RESET}: $1"
    exit 1
}

# Use the amount of CPUs as JOB_COUNT
BUILD_JOBS=$(nproc)

# Check for null parameter
if [ -z "$PACKAGE" ]; then
    fail "Invalid package name given"
fi

# Rebuild the Build directory for the package
BUILD_DIR="$BUILD_PORT_ROOT/$PACKAGE"
if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
fi
mkdir -p "$BUILD_DIR"

# Check for port existence
if [ ! -d "$PACKAGE" ]; then
    fail "Selected port NOT found"
fi

# Check for required Package.sh file
if [ ! -f "$PACKAGE/Package.sh" ]; then
    fail "Selected port did not provide 'Package.sh' file"
fi

# Load the Package.sh file
source "$PACKAGE/Package.sh"

# Sanity check for package
if [ -z "$REMOTE_ARCHIVE" ]; then
    fail "Selected port did not provide $$REMOTE_ARCHIVE"
fi
if [ -z "$ARCHIVE" ]; then
    fail "Selected port did not provide $$ARCHIVE"
fi
if [ -z "$UNPACKED_DIR" ]; then
    fail "Selected port did not provide $$UNPACKED_DIR"
fi

# Download then run the port_unpack function
pushd "$BUILD_DIR"
    wget -c "$REMOTE_ARCHIVE"
    port_unpack
popd

# Check for unpacked directory
if [ ! -d "$BUILD_DIR/$UNPACKED_DIR" ]; then
    fail "Selected port did not correctly unpack the package"
fi

# Patch it if necessary
if [ -f "$PACKAGE/Patch.diff" ]; then
    pushd "$BUILD_DIR/$UNPACKED_DIR"
        patch -p1 <"../../../../Ports/$PACKAGE/Patch.diff"
    popd
fi

# Build now the port package
if [ "$BUILD_IN_SOURCE_DIR" -eq 1 ]; then
    pushd "$BUILD_DIR/$UNPACKED_DIR"
else
    mkdir -p "$BUILD_DIR/Build"
    pushd "$BUILD_DIR/Build"
fi

# build the arguments
MEETIX_EXTRA_INCLUDE_PATHS=""
for path in "${EXTRA_INCLUDE_PATHS[@]}"; do
    MEETIX_EXTRA_INCLUDE_PATHS+="-isystem $path "
done

MEETIX_EXTRA_OPTIONS=""
for option in "${EXTRA_OPTIONS[@]}"; do
    MEETIX_EXTRA_OPTIONS+="$option "
done

MEETIX_EXTRA_LINK_DIRS=""
for path in "${EXTRA_LINK_DIRS[@]}"; do
    MEETIX_EXTRA_LINK_DIRS+="-L$path "
done

MEETIX_EXTRA_LIBS=""
for path in "${EXTRA_LIBS[@]}"; do
    MEETIX_EXTRA_LIBS+="-l$path "
done

# Build the port and install it
export PATH="$PATH:$TOOLCHAIN_ROOT/bin"
port_build

# Cleanup
popd
rm -rf "$BUILD_DIR"

exit 0
