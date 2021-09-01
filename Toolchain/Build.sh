#!/bin/bash

# colors
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
RESET=$(tput sgr0)

# creating dirs names
TOOLCHAIN_BASE="$(pwd)/Local"
SYSROOT=${TOOLCHAIN_BASE}
SYSROOT_BIN=${SYSROOT}/bin
SYSROOT_INCLUDE=${SYSROOT}/include
SYSROOT_LIB=${SYSROOT}/lib
BUILD_DIR=Build

# define the base source
REMOTE_TOOLCHAIN=https://ftp.gnu.org/gnu

# define GCC source stuffs
GCC_UNPACKED=gcc-11.2.0
GCC_REMOTE=${REMOTE_TOOLCHAIN}/gcc/gcc-11.2.0/${GCC_UNPACKED}.tar.gz
GCC_PATCH=Patches/GccPatch.diff

# define Binutils source stuffs
BINUTILS_UNPACKED=binutils-2.37
BINUTILS_REMOTE=${REMOTE_TOOLCHAIN}/binutils/${BINUTILS_UNPACKED}.tar.gz
BINUTILS_PATCH=Patches/BinutilsPatch.diff

# system target
TARGET=i686-pc-meetix

# Checks whether the given tool is installed
require_tool() {
    echo "Checking ${GREEN}${1}${RESET}"
    if ! type "${1}" >/dev/null 2>&1; then
        echo >&2 "${RED}Error: missing required tool '${1}'${RESET}"
        exit 1
    fi
}

# Overrides the builtin pushd command
push_dir() {
    pushd "$@" >/dev/null || exit 1
}

# Overrides the builtin popd command
pop_dir() {
    popd >/dev/null || exit 1
}

# Parse parameters
for arg in "$@"; do
    if [[ "${arg}" == "--rebuild" ]]; then
        echo "${RED}Cleaning previous installation${RESET}"
        rm -rf "${TOOLCHAIN_BASE}" "${BUILD_DIR}"
    else
        echo "${RED}Unknown parameter: ${arg}${RESET}"
        exit 1
    fi
done

# get from system cpu core number
if [ -f "/proc/cpuinfo" ]; then
    BUILD_JOBS=$(grep -c '^processor[[:space:]]*:' < "/proc/cpuinfo")
else
    BUILD_JOBS=1
fi

# Show how many jobs will be used
echo "${GREEN}Building Toolchains with ${BUILD_JOBS} jobs${RESET}"
MAKE_JOBS=-j${BUILD_JOBS}

# Check tools
require_tool g++
require_tool patch
require_tool wget
require_tool tar

# Safe creation of build dir
echo "Building toolchain into ${GREEN}${BUILD_DIR}${RESET}"
mkdir -p ${BUILD_DIR} || exit 1

# Download the tarballs
wget ${BINUTILS_REMOTE} -o ${BUILD_DIR}/${BINUTILS_UNPACKED} || exit 1
wget ${GCC_REMOTE} -o ${BUILD_DIR}/${GCC_UNPACKED} || exit 1

# Export for security the path
export PATH=${PATH}:${SYSROOT_BIN}
export MEETIX_TOOLCHAIN_DIR=${TOOLCHAIN_BASE}

# Print some information and create devel directory
echo "Toolchain will be installed to: ${GREEN}${TOOLCHAIN_BASE}${RESET}]"
mkdir -p "${TOOLCHAIN_BASE}" || exit 1
mkdir -p "${SYSROOT_BIN}" || exit 1
mkdir -p "${SYSROOT_LIB}" || exit 1
mkdir -p "${SYSROOT_INCLUDE}" || exit 1

# Unpack archives
echo "Unpacking ${GREEN}${BUILD_DIR}/${GCC_UNPACKED}${RESET}"
tar -xf "${BUILD_DIR}/${GCC_UNPACKED}.tar.gz" || exit 1

echo "Unpacking ${GREEN}${BUILD_DIR}/${BINUTILS_UNPACKED}${RESET}"
tar -xf "${BUILD_DIR}/${BINUTILS_UNPACKED}.tar.gz" || exit 1

# Install the MeetiX libraries TODO CMake
echo "Installing ${GREEN}MeetiX libraries${RESET}"
cp -r i686/libs/* "${TOOLCHAIN_BASE}" || exit 1

# Apply patches
echo "Patching ${GREEN}${GCC_UNPACKED}${RESET}"
patch -d ${BUILD_DIR}/${GCC_UNPACKED} -p 1 <${GCC_PATCH} || exit 1

echo "Patching ${GREEN}${BINUTILS_UNPACKED}${RESET}"
patch -d ${BUILD_DIR}/${BINUTILS_UNPACKED} -p 1 <${BINUTILS_PATCH} || exit 1

# Create object directories on the toolchain TODO CMake
echo "Preparing Libraries Environment"
push_dir "${TOOLCHAIN_BASE}/libsource/libs"
    mkdir -p obj || exit 1
    push_dir obj
        OBJ_DIRS=(api c crt driver graphics gui io tasking utils)
        for dir_name in "${OBJ_DIRS[@]}"; do
            mkdir -p "${dir_name}" || exit 1
        done
    pop_dir
pop_dir

# Build tools
echo "Building ${GREEN}changes tool${RESET}"
push_dir "${TOOLCHAIN_BASE}/libsource/tools/changes"
    CC=g++ bash build.sh all || exit 1
pop_dir

echo "Building ${GREEN}ramdisk-writer tool${RESET}"
push_dir "${TOOLCHAIN_BASE}/libsource/tools/ramdisk-writer"
    CC=g++ bash build.sh all || exit 1
pop_dir

echo "Building ${GREEN}pkg-config tool${RESET}"
push_dir "${TOOLCHAIN_BASE}/libsource/tools/pkg-config"
    bash build.sh || exit 1
pop_dir

# Install library headers
echo "Installing ${GREEN}MeetiX Libraries Headers${RESET}"
KERNEL_INC=$(pwd)"/../source/kernel/inc"
push_dir "${TOOLCHAIN_BASE}/libsource/libs"
    bash build.sh header "${KERNEL_INC}" || exit 1
pop_dir

# Build binutils
BUILD_BINUTILS=${BUILD_DIR}/BuildBinutils
mkdir -p ${BUILD_BINUTILS} || exit 1
push_dir ${BUILD_BINUTILS}
    echo "Configuring ${GREEN}${BINUTILS_UNPACKED}${RESET}"
    ../${BINUTILS_UNPACKED}/configure --target=${TARGET}            \
                                      --prefix="${TOOLCHAIN_BASE}"  \
                                      --disable-nls                 \
                                      --disable-werror              \
                                      --with-sysroot="${TOOLCHAIN_BASE}" || exit 1

    echo "Building ${GREEN}${BINUTILS_UNPACKED}${RESET}"
    make ${MAKE_JOBS} all || exit 1

    echo "Installing ${GREEN}${BINUTILS_UNPACKED}${RESET}"
    make ${MAKE_JOBS} install || exit 1
pop_dir

# Build GCC
BUILD_GCC=${BUILD_DIR}/BuildGcc
mkdir -p ${BUILD_GCC} || exit 1
push_dir ${BUILD_GCC}
    echo "Configuring ${GREEN}${GCC_UNPACKED}${RESET}"
    ../${GCC_UNPACKED}/configure --target=${TARGET}               \
                               --prefix="${TOOLCHAIN_BASE}"       \
                               --enable-languages=c,c++           \
                               --with-sysroot="${TOOLCHAIN_BASE}" || exit 1

    echo "Building ${GREEN}${GCC_UNPACKED}${RESET}"
    make ${MAKE_JOBS} all-gcc || exit 1

    echo "Installing ${GREEN}${GCC_UNPACKED}${RESET}"
    make ${MAKE_JOBS} install-gcc || exit 1

    echo "Building ${GREEN}libgcc${RESET}"
    make ${MAKE_JOBS} all-target-libgcc || exit 1

    echo "Installing ${GREEN}libgcc${RESET}"
    make ${MAKE_JOBS} install-target-libgcc || exit 1
pop_dir

# add to path the compilers dir
echo "Updating ${GREEN}PATH${RESET} with ${GREEN}${TOOLCHAIN_BASE}/bin${RESET}"
echo "export PATH=\$PATH:${TOOLCHAIN_BASE}/bin" >../source/env.sh
{
    echo "export MEETIX_TOOLCHAIN_DIR=${TOOLCHAIN_BASE}";
    echo "export MEETIX_LIB=${MEETIX_TOOLCHAIN_DIR}/libsource/libs";
    echo "export MEETIX_LINK_STD=static";
} >>../source/env.sh

# load as effective changes the new environment variables
source ../source/env.sh

# Build C and API library
push_dir "${TOOLCHAIN_BASE}/libsource/libs"
    echo "Building ${GREEN}libc${RESET}"
    bash build.sh c || exit 1

    echo "Building ${GREEN}libapi${RESET}"
    bash build.sh api "${KERNEL_INC}" || exit 1
pop_dir

# Build libstdc++-v3
push_dir ${BUILD_GCC}
    echo "Building ${GREEN}libstdc++ v3${RESET}"
    make ${MAKE_JOBS} all-target-libstdc++-v3 || exit 1

    echo "Installing ${GREEN}libstdc++ v3${RESET}"
    make ${MAKE_JOBS} install-target-libstdc++-v3 || exit 1
pop_dir

# Build cross port libs
push_dir "${TOOLCHAIN_BASE}/libsource/ports"
    echo "Building ${GREEN}zlib${RESET}"
    bash port.sh zlib || exit 1

    echo "Building ${GREEN}libpng${RESET}"
    bash port.sh libpng || exit 1

    echo "Building ${GREEN}pixman${RESET}"
    bash port.sh pixman || exit 1

    echo "Building ${GREEN}freetype${RESET}"
    bash port.sh freetype || exit 1

    echo "Building ${GREEN}cairo${RESET}"
    bash port.sh cairo || exit 1
pop_dir

# Build MeetiX native libraries
push_dir "${TOOLCHAIN_BASE}/libsource/libs"
    echo "Building ${GREEN}libGraphic${RESET}"
    bash build.sh graphic || exit 1

    # build gui library
    echo "Building ${GREEN}libGUI${RESET}"
    bash build.sh gui || exit 1

    # build io library
    echo "Building ${GREEN}libIO${RESET}"
    bash build.sh io || exit 1

    # build tasking library
    echo "Building ${GREEN}libTasking${RESET}"
    bash build.sh tasking || exit 1

    # build utils library
    echo "Building ${GREEN}libUtils${RESET}"
    bash build.sh utils || exit 1
pop_dir

# Clean build stuffs
echo "Last Cleaning"
rm -rf ${BUILD_DIR} || exit 1
rm -rf "${TOOLCHAIN_BASE}/ports/build" || exit 1

# Finished
echo "${GREEN}Toolchain successfully built${RESET}"
