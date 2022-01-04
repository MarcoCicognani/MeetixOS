#!/bin/bash

# ----------------------------------------------- Variables Definitions ---------------------------------------------- #

# Colors
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
RESET=$(tput sgr0)

# Make jobs to use
BUILD_JOBS=$(nproc)
SKIP_LAST_CLEAN=0

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

# ----------------------------------------------- Functions Definitions ---------------------------------------------- #

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

# Prints the build-step before the command
build_step() {
    local STEP_NAME=$1
    shift
    "$@" 2>&1 | sed $'s|^|\x1b[34m['"${STEP_NAME}"$']\x1b[39m |'
}

# ---------------------------------- Script Code: Argument Parsing & Variables Init ---------------------------------- #

for CMD_ARG in "$@"; do
    case "$CMD_ARG" in
        --rebuild)
            build_step "Previous install cleaning..." rm -rf "$TOOLCHAIN_PREFIX" "$BUILD_DIR" ../Build
            ;;
        --skip-last-clean)
            SKIP_LAST_CLEAN=1
            ;;
        *)
            echo "${RED}Unknown parameter: $CMD_ARG${RESET}"
            echo "Valid parameters are:"
            echo "    --rebuild:         Clean previous installation"
            echo "    --skip-last-clean: Keep toolchain build files for debug"
            exit 1
            ;;
    esac
done

echo "${GREEN}Building Toolchains with $BUILD_JOBS jobs${RESET}"
MAKE_JOBS=-j$BUILD_JOBS

# -------------------------------------------- Script Code: Tools Checking ------------------------------------------- #

require_tool g++
require_tool patch
require_tool wget
require_tool tar
require_tool cmake
require_tool ninja
require_tool grub-mkrescue
require_tool xorriso

# -------------------------------------- Script Code: Dirs & Tarballs Downloads -------------------------------------- #

echo "Building toolchain into ${GREEN}$(realpath $BUILD_DIR)${RESET}"
mkdir -p $BUILD_DIR $TARBALLS_DIR || exit 1

dir_push $TARBALLS_DIR
    build_step "Download/$GCC_UNPACKED"      wget -N $BINUTILS_REMOTE || exit 1
    build_step "Download/$BINUTILS_UNPACKED" wget -N $GCC_REMOTE      || exit 1
dir_pop

# Print some information and create devel directory
echo "Toolchain will be installed into: ${GREEN}$TOOLCHAIN_PREFIX${RESET}"
mkdir -p "$TOOLCHAIN_PREFIX/bin" || exit 1

# ------------------------------------------- Script Code: Tarballs Unpack ------------------------------------------- #

build_step "Unpack/$GCC_UNPACKED"      tar -xf $TARBALLS_DIR/$GCC_UNPACKED.tar.gz -C $BUILD_DIR      || exit 1
build_step "Unpack/$BINUTILS_UNPACKED" tar -xf $TARBALLS_DIR/$BINUTILS_UNPACKED.tar.gz -C $BUILD_DIR || exit 1

# ----------------------------------------- Script Code: Toolchains Patching ----------------------------------------- #

build_step "Patch/$BINUTILS_UNPACKED" patch -d $BUILD_DIR/$BINUTILS_UNPACKED -p 1 <$BINUTILS_PATCH || exit 1
build_step "Patch/$GCC_UNPACKED"      patch -d $BUILD_DIR/$GCC_UNPACKED -p 1 <$GCC_PATCH           || exit 1

# ----------------------------------------- Script Code: Build Internal Tools ---------------------------------------- #

dir_push ../Meta/Tools/PackageConfig
    build_step "Tool/PackageConfig" bash Build.sh || exit 1
dir_pop

echo "Building ${GREEN}RamdiskWriter tool${RESET}"
dir_push ..
    mkdir -p Build/Release/Meta/Tools/RamdiskWriter || exit 1
    dir_push Build/Release/Meta/Tools/RamdiskWriter
        build_step "Tool/RamdiskWriter" \
            cmake -DCMAKE_BUILD_TYPE=Release -GNinja ../../../../../Meta/Tools/RamdiskWriter || exit 1
        build_step "Tool/RamdiskWriter" ninja || exit 1
    dir_pop
dir_pop

export CFLAGS="-g0 -O2 -mtune=native"
export CXXFLAGS="-g0 -O2 -mtune=native"

# -------------------------------------------- Script Code: Binutils Build ------------------------------------------- #

BUILD_BINUTILS=$BUILD_DIR/Binutils
mkdir -p $BUILD_BINUTILS || exit 1
dir_push $BUILD_BINUTILS
    build_step "Binutils/Configure"                                  \
        ../$BINUTILS_UNPACKED/configure --target=$TARGET             \
                                        --prefix="$TOOLCHAIN_PREFIX" \
                                        --disable-nls                \
                                        --disable-werror             \
                                        --with-sysroot="$TOOLCHAIN_PREFIX" || exit 1
    build_step "Binutils/Build"   make $MAKE_JOBS all     || exit 1
    build_step "Binutils/Install" make $MAKE_JOBS install || exit 1
dir_pop

# ---------------------------------------------- Script Code: GCC Build ---------------------------------------------- #

BUILD_GCC=$BUILD_DIR/Gcc
mkdir -p $BUILD_GCC || exit 1
dir_push $BUILD_GCC
    build_step "GCC/Configure"                                  \
        ../$GCC_UNPACKED/configure --target=$TARGET             \
                                   --prefix="$TOOLCHAIN_PREFIX" \
                                   --enable-languages=c,c++     \
                                   --with-newlib                \
                                   --disable-nls                \
                                   --enable-lto                 \
                                   --with-sysroot="$TOOLCHAIN_PREFIX" || exit 1
    build_step "GCC/Build"   make $MAKE_JOBS all-gcc     || exit 1
    build_step "GCC/Install" make $MAKE_JOBS install-gcc || exit 1

    build_step "GCC/LibGCC Build"   make $MAKE_JOBS all-target-libgcc     || exit 1
    build_step "GCC/LibGCC Install" make $MAKE_JOBS install-target-libgcc || exit 1

    build_step "LibStdC++/Build"   make $MAKE_JOBS all-target-libstdc++-v3     || exit 1
    build_step "LibStdC++/Install" make $MAKE_JOBS install-target-libstdc++-v3 || exit 1
dir_pop

# ---------------------------------------- Scrip Code: Reset Compilation Flags --------------------------------------- #

export CFLAGS=""
export CXXFLAGS=""

# ------------------------------------ Scrip Code: CMake Toolchains Configuration ------------------------------------ #

dir_push ../Meta
    build_step "CMake/Toolchain" bash BuildCMakeToolchain.sh || exit 1
dir_pop

# ----------------------------------- Script Code: Internal Fundamental Libs Build ----------------------------------- #

dir_push ../Build/Release
    build_step "Libs/Configure"                               \
        cmake -GNinja                                         \
              -DCMAKE_TOOLCHAIN_FILE=Build/CMakeToolchain.txt \
              -DCMAKE_BUILD_TYPE=Release                      \
              ../.. || exit 1
    build_step "Libs/CRTs"      ninja crt0 crti crtn || exit 1
    build_step "Libs/LibApi"    ninja LibApi         || exit 1
    build_step "Libs/LibC"      ninja LibC           || exit 1
dir_pop

# ---------------------------------------- Script Code: Third Party Libs Build --------------------------------------- #

dir_push ../Ports
    build_step "Ports/LibZ"        bash BuildPort.sh LibZ        || exit 1
    build_step "Ports/LibPNG"      bash BuildPort.sh LibPNG      || exit 1
    build_step "Ports/LibPixMan"   bash BuildPort.sh LibPixMan   || exit 1
    build_step "Ports/LibFreeType" bash BuildPort.sh LibFreeType || exit 1
    build_step "Ports/LibCairo"    bash BuildPort.sh LibCairo    || exit 1
dir_pop

# -------------------------------------------- Script Code: Last Cleaning -------------------------------------------- #

if [[ $SKIP_LAST_CLEAN -ne 1 ]]; then
    echo "Last Cleaning"
    rm -rf $BUILD_DIR || exit 1
else
    echo "Skipping Last Cleaning"
fi
