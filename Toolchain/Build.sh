#!/bin/bash

# Colors
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
RESET=$(tput sgr0)

# Target triplet
TARGET=i686-pc-meetix

# Construct directory names
TOOLCHAIN_PREFIX="$(pwd)/Local"
TARBALLS_DIR=Tarballs
BUILD_DIR=Build

# Define GCC source stuffs
GCC_VERSION=11.2.0
GCC_UNPACKED=gcc-$GCC_VERSION
GCC_REMOTE=https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/$GCC_UNPACKED.tar.gz
GCC_PATCH=Patches/GccPatch.diff

# Define Binutils source stuffs
BINUTILS_VERSION=2.37
BINUTILS_UNPACKED=binutils-$BINUTILS_VERSION
BINUTILS_REMOTE=https://ftp.gnu.org/gnu/binutils/$BINUTILS_UNPACKED.tar.gz
BINUTILS_PATCH=Patches/BinutilsPatch.diff

# Checks whether the given tool is installed
require_tool() {
    echo "Checking ${GREEN}$1${RESET}"
    if ! type "$1" >/dev/null 2>&1; then
        echo >&2 "${RED} Error: missing required tool '$1'${RESET}"
        exit 1
    fi
}

# Overrides the builtin pushd command
dir_push() {
    pushd "$@" >/dev/null || exit 1
}

# Overrides the builtin popd command
dir_pop() {
    popd >/dev/null || exit 1
}

# Parse parameters
for CMD_ARG in "$@"; do
    if [[ "$CMD_ARG" == "--rebuild" ]]; then
        echo "${RED}Cleaning previous installation${RESET}"
        rm -rf "$TOOLCHAIN_PREFIX" "$BUILD_DIR"
    else
        echo "${RED}Unknown parameter: $CMD_ARG${RESET}"
        exit 1
    fi
done

# Use the amount of CPUs as JOB_COUNT
if [ -f "/proc/cpuinfo" ]; then
    BUILD_JOBS=$(grep -c '^processor[[:space:]]*:' </proc/cpuinfo)
else
    BUILD_JOBS=1
fi

# Show how many jobs will be used
echo "${GREEN}Building Toolchains with $BUILD_JOBS jobs${RESET}"
MAKE_JOBS=-j$BUILD_JOBS

# Check tools
require_tool g++
require_tool patch
require_tool wget
require_tool tar
require_tool cmake
require_tool ninja

# Safe creation of build dir
echo "Building toolchain into ${GREEN}$(realpath $BUILD_DIR)${RESET}"
mkdir -p $BUILD_DIR || exit 1
mkdir -p $TARBALLS_DIR || exit 1

# Download the tarballs
dir_push $TARBALLS_DIR
    wget -N $BINUTILS_REMOTE || exit 1
    wget -N $GCC_REMOTE || exit 1
dir_pop

# Print some information and create devel directory
echo "Toolchain will be installed into: ${GREEN}$TOOLCHAIN_PREFIX${RESET}"
mkdir -p "$TOOLCHAIN_PREFIX/bin" || exit 1

# Unpack archives
echo "Unpacking ${GREEN}$TARBALLS_DIR/$GCC_UNPACKED${RESET}"
tar -xf $TARBALLS_DIR/$GCC_UNPACKED.tar.gz -C $BUILD_DIR || exit 1

echo "Unpacking ${GREEN}$TARBALLS_DIR/$BINUTILS_UNPACKED${RESET}"
tar -xf $TARBALLS_DIR/$BINUTILS_UNPACKED.tar.gz -C $BUILD_DIR || exit 1

# Apply patches to the Binutils
echo "Patching ${GREEN}$BINUTILS_UNPACKED${RESET}"
patch -d $BUILD_DIR/$BINUTILS_UNPACKED -p 1 <$BINUTILS_PATCH || exit 1

# Apply patches to the GCC
echo "Patching ${GREEN}$GCC_UNPACKED${RESET}"
patch -d $BUILD_DIR/$GCC_UNPACKED -p 1 <$GCC_PATCH || exit 1

# Build tools
echo "Building ${GREEN}PackageConfig tool${RESET}"
dir_push ../Meta/Tools/PackageConfig
    bash Build.sh || exit 1
dir_pop

echo "Building ${GREEN}RamdiskWriter tool${RESET}"
dir_push ..
    mkdir -p Build/Release/Meta/Tools || exit 1
    dir_push Build/Release/Meta/Tools
        cmake -DCMAKE_BUILD_TYPE=Release -GNinja ../../../../Meta/Tools/RamdiskWriter || exit 1

        ninja || exit 1
    dir_pop
dir_pop

# Build binutils
BUILD_BINUTILS=$BUILD_DIR/Binutils
mkdir -p $BUILD_BINUTILS || exit 1
dir_push $BUILD_BINUTILS
    echo "Configuring ${GREEN}$BINUTILS_UNPACKED${RESET}"
    CFLAGS="-g0 -O2 -mtune=native"                                   \
    CXXFLAGS="-g0 -O2 -mtune=native"                                 \
        ../$BINUTILS_UNPACKED/configure --target=$TARGET             \
                                        --prefix="$TOOLCHAIN_PREFIX" \
                                        --disable-nls                \
                                        --disable-werror             \
                                        --with-sysroot="$TOOLCHAIN_PREFIX" || exit 1

    echo "Building ${GREEN}$BINUTILS_UNPACKED${RESET}"
    make $MAKE_JOBS all || exit 1

    echo "Installing ${GREEN}$BINUTILS_UNPACKED${RESET}"
    make $MAKE_JOBS install || exit 1
dir_pop

# Build GCC
BUILD_GCC=$BUILD_DIR/Gcc
mkdir -p $BUILD_GCC || exit 1
dir_push $BUILD_GCC
    echo "Configuring ${GREEN}$GCC_UNPACKED${RESET}"
    CFLAGS="-g0 -O2 -mtune=native"                              \
    CXXFLAGS="-g0 -O2 -mtune=native"                            \
        ../$GCC_UNPACKED/configure --target=$TARGET             \
                                   --prefix="$TOOLCHAIN_PREFIX" \
                                   --enable-languages=c,c++     \
                                   --with-sysroot="$TOOLCHAIN_PREFIX" || exit 1

    echo "Building ${GREEN}$GCC_UNPACKED${RESET}"
    make $MAKE_JOBS all-gcc || exit 1

    echo "Installing ${GREEN}$GCC_UNPACKED${RESET}"
    make $MAKE_JOBS install-gcc || exit 1

    echo "Building ${GREEN}libgcc${RESET}"
    make $MAKE_JOBS all-target-libgcc || exit 1

    echo "Installing ${GREEN}libgcc${RESET}"
    make $MAKE_JOBS install-target-libgcc || exit 1
dir_pop

# Configure CMake then build C and API library
dir_push ../Build/Release
    cmake -DCMAKE_BUILD_TYPE=Release -GNinja ../.. || exit 1

    echo "Building ${GREEN}CRTs${RESET}"
    ninja crt0 crti crtn || exit 1

    echo "Building ${GREEN}libApi${RESET}"
    ninja Api || exit 1

    echo "Building ${GREEN}libC${RESET}"
    ninja C || exit 1
dir_pop

# Build GCC libstdc++-v3
dir_push $BUILD_GCC
    echo "Building ${GREEN}libstdc++ v3${RESET}"
    make $MAKE_JOBS all-target-libstdc++-v3 || exit 1

    echo "Installing ${GREEN}libstdc++ v3${RESET}"
    make $MAKE_JOBS install-target-libstdc++-v3 || exit 1
dir_pop

# Build cross port libs
dir_push ../Ports
    echo "Building ${GREEN}zlib${RESET}"
    bash BuildPort.sh zlib || exit 1

    echo "Building ${GREEN}libpng${RESET}"
    bash BuildPort.sh libpng || exit 1

    echo "Building ${GREEN}pixman${RESET}"
    bash BuildPort.sh pixman || exit 1

    echo "Building ${GREEN}freetype${RESET}"
    bash BuildPort.sh freetype || exit 1

    echo "Building ${GREEN}cairo${RESET}"
    bash BuildPort.sh cairo || exit 1
dir_pop

# Clean build stuffs
echo "Last Cleaning"
rm -rf $BUILD_DIR || exit 1

# Finished
echo "${GREEN}Toolchain successfully built${RESET}"
