#!/bin/bash

# Colors
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
RESET=$(tput sgr0)

PACKAGE=$1
SOURCE_DIR=$(realpath ..)
BUILD_ROOT="$SOURCE_DIR/Build/Ports"
TOOLCHAIN_ROOT="$SOURCE_DIR/Toolchain/Local"
MEETIX_LIBC_DIR="$SOURCE_DIR/Build/Release/Libs/LibC"
MEETIX_EXTRA_LIBS="-lLibC -lsupc++ -lstdc++"
BUILD_IN_SOURCE_DIR=0

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
BUILD_DIR="$BUILD_ROOT/$PACKAGE"
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
pushd "$BUILD_DIR" || exit 1
    wget -c "$REMOTE_ARCHIVE" || exit 1
    port_unpack
popd || exit 1

# Check for unpacked directory
if [ ! -d "$BUILD_DIR/$UNPACKED_DIR" ]; then
    fail "Selected port did not correctly unpack the package"
fi

# Patch it if necessary
if [ -f "$PACKAGE/Patch.diff" ]; then
    pushd "$BUILD_DIR/$UNPACKED_DIR" || exit 1
        patch -p1 <"../../../../Ports/$PACKAGE/Patch.diff" || exit 1
    popd || exit 1
fi

# Build now the port package
if [ "$BUILD_IN_SOURCE_DIR" -eq 1 ]; then
    pushd "$BUILD_DIR/$UNPACKED_DIR" || exit 1
else
    mkdir -p "$BUILD_DIR/Build" || exit 1
    pushd "$BUILD_DIR/Build" || exit 1
fi

# Build the port and install it
export PATH="$PATH:$TOOLCHAIN_ROOT/bin"
port_build || exit 1

# Cleanup
popd || exit 1
rm -rf "$BUILD_DIR" || exit 1

exit 0