#!/bin/bash

# colors
RED=`tput setaf 1`
GREEN=`tput setaf 2`
CYAN=`tput setaf 6`
RESET=`tput sgr0`

# creating dirs names
DIRNAME="MeetiXOSProject"
TOOLCHAIN_BASE="/opt/$DIRNAME"
SYSROOT=$TOOLCHAIN_BASE
SYSROOT_BIN="$SYSROOT/bin"
SYSROOT_INCLUDE="$SYSROOT/include"
SYSROOT_LIB="$SYSROOT/lib"
BUILD_DIR="build"
BUILD_LOG="$(pwd)$BUILD_LOG"

# define the base source
REMOTE="https://www.meetixos.org"
REMOTE_TOOLCHAIN="$REMOTE/toolchain"

# define archives names
GCC_ARCHIVE="$REMOTE_TOOLCHAIN/gcc.tar.gz"
GCC_PATCH="i686/gcc-4.9.1.patch"
GCC_UNPACKED="gcc-4.9.1"

# define archives names
BINUTILS_ARCHIVE="$REMOTE_TOOLCHAIN/binutils.tar.gz"
BINUTILS_PATCH="i686/binutils-2.24.patch"
BINUTILS_UNPACKED="binutils-2.24"

# define archives names
AUTOMAKE_ARCHIVE="$REMOTE_TOOLCHAIN/automake.tar.bz2"
AUTOMAKE_UNPACKED="automake-1.11.1"
AUTOMAKE=""

# define archives names
AUTOCONF_ARCHIVE="$REMOTE_TOOLCHAIN/autoconf.tar.bz2"
AUTOCONF_UNPACKED="autoconf-2.64"
AUTOCONF=""

# system target
TARGET="i686-mx"

# step of build
STEP_HELP="0"
STEP_CLEAN="0"
STEP_DOWNLOAD="1"
STEP_UNPACK="1"
STEP_PATCH="1"

# builds steps
STEP_BUILD_AUTOMAKE="1"
STEP_BUILD_AUTOCONF="1"
STEP_BUILD_BINUTILS="1"
STEP_BUILD_GCC="1"
STEP_BUILD_PORTS="1"
STEP_INSTALL_LOCAL="0"

# tools
DOWNLOADER="curl"
PATCH="patch"
CXX="g++"

# Make sure only root can run our script
if [ "$(id -u)" != "0" ]; then
   echo "${RED}This script can only run as root${RESET}"
   exit 1
fi

# function to check install of provided tool
function requireTool()
{
	if ! type "$1" >/dev/null 2>&1
	then
		>&2 echo "${RED}error: missing required tool '$1'${RESET}"
		exit 1
	fi
}

#exit from script if previous command fail
function failOnError()
{
	if [[ $? != 0 ]]; then
		echo "${RED}Build failed${RESET}"
		exit 1
	fi
}

# override a variable
function with()
{
    if [ -z "${!1}" ]; then
        eval $1='$2';
    fi
}

# download a file
function download()
{
    if [ ! -f "i686/$2.$3" ];
    then
        echo "[${GREEN}Downloading${RESET}] $2"
        $DOWNLOADER $1 -o "i686/$2.$3" -k
    else
        echo "${CYAN}$2 already downloaded${RESET}"
    fi
}

# ovverride function for pushd and popd
function pushd()
{
    command pushd "$@" > /dev/null
}
function popd()
{
    command popd "$@" > /dev/null
}

# Parse parameters
for var in "$@";
do
    if [ ${var:0:1} != "-" ]; then TOOLCHAIN_BASE=$var
    elif [ $var == "--skip-download" ]; then STEP_DOWNLOAD="0"
    elif [ $var == "--install-local" ]; then STEP_INSTALL_LOCAL="1"
	elif [ $var == "--skip-unpack" ]; then STEP_UNPACK="0"
	elif [ $var == "--skip-patch" ]; then STEP_PATCH="0"
	elif [ $var == "--skip-build-automake" ]; then STEP_BUILD_AUTOMAKE="0"
	elif [ $var == "--skip-build-autoconf" ]; then STEP_BUILD_AUTOCONF="0"
	elif [ $var == "--skip-build-binutils" ]; then STEP_BUILD_BINUTILS="0"
	elif [ $var == "--skip-build-gcc" ]; then STEP_BUILD_GCC="0"
	elif [ $var == "--clean" ]; then STEP_CLEAN="1"
    elif [ $var == "--help" ]; then STEP_HELP="1"
	else
        echo "${RED}unknown parameter: $var${RESET}"
		exit 1
	fi
done

# install in the local directory
if [ $STEP_INSTALL_LOCAL == "1" ];
then
    TOOLCHAIN_BASE="$(pwd)/$DIRNAME"
    SYSROOT=$TOOLCHAIN_BASE
fi

# check for help
if [ $STEP_HELP == "1" ];
then
    echo "${GREEN}MeetiX OS Project toolchain builder${RESET}"
    echo "Avaible options:"
    echo "  ${CYAN}--verbose${RESET}                 show all the verbose output of the builds"
    echo "  ${CYAN}--install-local${RESET}           install the toolchains on the current directory '${RED}$(pwd)${RESET}'"
    echo "  ${CYAN}--skip-download${RESET}           skip the download of packages"
    echo "  ${CYAN}--skip-unpack${RESET}             skip the unpacking of the downloaded packages"
    echo "  ${CYAN}--skip-patch${RESET}              skip the patching of the GCC and binutils"
    echo "  ${CYAN}--skip-build-automake${RESET}     skip the build of custom Automake"
    echo "  ${CYAN}--skip-build-autoconf${RESET}     skip the build of custom Autoconf"
    echo "  ${CYAN}--skip-build-binutils${RESET}     skip the build of custom Binutils"
    echo "  ${CYAN}--skip-build-gcc${RESET}          skip the build of custom GCC"
    echo "  ${CYAN}--clean${RESET}                   clean the build directory"
    echo "  ${CYAN}--help${RESET}                    show this help"
    echo ""
    echo "${RED}if you provide an unfiltered option the script take it as the install directory, make sure to provide an existing path${RESET}"

    exit 0
fi

# get from system cpu core number
if [ -f "/proc/cpuinfo" ]; then
	cpus=$(cat "/proc/cpuinfo" | grep '^processor[[:space:]]*:' | wc -l)
else
	cpus="1"
fi

# show cpu for build
echo "${GREEN}Building Toolchains with $cpus jobs${RESET}"
MULTITHREAD="-j$cpus"

# check tools
echo "[${GREEN}Checking${RESET}] $CXX"
requireTool $CXX

echo "[${GREEN}Checking${RESET}] $PATCH"
requireTool $PATCH

echo "[${GREEN}Checking${RESET}] $DOWNLOADER"
requireTool $DOWNLOADER


# clean if necessary
if [ $STEP_CLEAN == "1" ];
then
	echo "[${GREEN}Cleaning${RESET}] $BUILD_DIR"
    rm -rf "$BUILD_DIR"
fi

# download if necessary
if [ $STEP_DOWNLOAD == "1" ];
then
	echo ""
    download $AUTOMAKE_ARCHIVE $AUTOMAKE_UNPACKED "tar.bz2"
    download $AUTOCONF_ARCHIVE $AUTOCONF_UNPACKED "tar.bz2"
    download $BINUTILS_ARCHIVE $BINUTILS_UNPACKED "tar.gz"
    download $GCC_ARCHIVE $GCC_UNPACKED "tar.gz"
	echo ""
else
	echo "${CYAN}Skipping download${RESET}"
fi

# safe creation of build dir
echo "[${GREEN}Creating${RESET}] $BUILD_DIR"
mkdir -p $BUILD_DIR

# compile a custom version of automake without installing on system
if [ $STEP_BUILD_AUTOMAKE == "1" ];
then
	# create the install prefix
    AUTOMAKE_PREFIX="$(pwd)/build/atmk"

    echo "[${GREEN}Unpacking${RESET}] $AUTOMAKE_UNPACKED"
    tar -xf i686/automake-1.11.1.tar.bz2 -C build

    echo "[${GREEN}Configuring${RESET}] $AUTOMAKE_UNPACKED"
    pushd build/$AUTOMAKE_UNPACKED
    ./configure --prefix=$AUTOMAKE_PREFIX >>MeetiX.log 2>&1
    failOnError

    echo "[${GREEN}Building${RESET}] $AUTOMAKE_UNPACKED"
    make $MULTITHREAD >>MeetiX.log 2>&1
    failOnError

    echo "[${GREEN}Installing${RESET}] $AUTOMAKE_UNPACKED"
    make install >>MeetiX.log 2>&1
    failOnError

    popd
    export AUTOMAKE="$(pwd)/build/atmk/bin/automake-1.11"
    echo ""
else
	echo "${CYAN}Skipping build automake${RESET}"
fi

# compile a custom version of autoconf without installing on system
if [ $STEP_BUILD_AUTOCONF == "1" ];
then
	# create the install prefix
    AUTOCONF_PREFIX="$(pwd)/build/atcf"

    echo "[${GREEN}Unpacking${RESET}] $AUTOCONF_UNPACKED"
    tar -xf i686/autoconf-2.64.tar.bz2 -C build

    echo "[${GREEN}Configuring${RESET}] $AUTOCONF_UNPACKED"
    pushd build/$AUTOCONF_UNPACKED
    ./configure --prefix=$AUTOCONF_PREFIX >>MeetiX.log 2>&1
    failOnError

    echo "[${GREEN}Building${RESET}] $AUTOCONF_UNPACKED"
    make $MULTITHREAD >>MeetiX.log 2>&1
    failOnError

    echo "[${GREEN}Installing${RESET}] $AUTOCONF_UNPACKED"
    make install >>MeetiX.log 2>&1
    failOnError

    popd
    export AUTOCONF="$(pwd)/build/atcf/bin/autoconf"
    echo ""
else
	echo "${CYAN}Skipping build autoconf${RESET}"
fi

# export for security the path
export PATH=$PATH:$SYSROOT_BIN
export MEETIX_TOOLCHAIN_DIR=$TOOLCHAIN_BASE

# Print some information and create devel directory
echo "[${GREEN}Toolchain will be installed to: $TOOLCHAIN_BASE${RESET}]"
mkdir -p $TOOLCHAIN_BASE
mkdir -p $SYSROOT
mkdir -p $SYSROOT_BIN
mkdir -p $SYSROOT_LIB
mkdir -p $SYSROOT_INCLUDE

# Clean
echo "[${GREEN}Cleaning${RESET}] $AUTOMAKE_UNPACKED"
rm -rf build/automake-1.11.1
failOnError

echo "[${GREEN}Cleaning${RESET}] $AUTOCONF_UNPACKED"
rm -rf build/autoconf-2.64
failOnError


# Unpack archives
if [ $STEP_UNPACK == "1" ];
then
	echo "[${GREEN}Unpacking${RESET}] $GCC_UNPACKED"
	tar -xf "i686/$GCC_UNPACKED.tar.gz" -C build
	failOnError

	echo "[${GREEN}Unpacking${RESET}] $BINUTILS_UNPACKED"
	tar -xf "i686/$BINUTILS_UNPACKED.tar.gz" -C build
	failOnError

    echo "[${GREEN}Installing${RESET}] MeetiX libraries"
    cp -r i686/libs/* $TOOLCHAIN_BASE
    failOnError
    echo ""

else
	echo "${CYAN}Skipping unpacking${RESET}"
fi


# Apply patches
if [ $STEP_PATCH == "1" ];
then
	echo "[${GREEN}Patching${RESET}] $GCC_UNPACKED"
	$PATCH -d build/$GCC_UNPACKED -p 1 < $GCC_PATCH	                >>MeetiX.log 2>&1
	pushd "build/$GCC_UNPACKED/libstdc++-v3"
	echo "[${GREEN}$AUTOCONF_UNPACKED${RESET}] libstdc++-v3"
	$AUTOCONF >>MeetiX.log 2>&1
	popd

	echo "[${GREEN}Patching${RESET}] $BINUTILS_UNPACKED"
	$PATCH -d build/$BINUTILS_UNPACKED -p 1 < $BINUTILS_PATCH		>>MeetiX.log 2>&1
	pushd "build/$BINUTILS_UNPACKED/ld"
	echo "[${GREEN}$AUTOMAKE_UNPACKED${RESET}] ld"
	$AUTOMAKE >>MeetiX.log 2>&1
	popd
	echo ""

else echo "[${CYAN}Skipping patching${RESET}]"
fi

# create object directories on the toolchain
echo "[${GREEN}Mkdir${RESET}] object directories"
pushd "$TOOLCHAIN_BASE/libsource/libs"
mkdir -p "obj"
pushd "obj"
objdirs=( "api" "c" "crt" "driver" "graphics" "gui" "io" "tasking" "utils" )
for odname in ${objdirs[@]}; do
	mkdir -p $odname
done
popd
popd

# Build tools
echo "[${GREEN}Building${RESET}] changes"
pushd "$TOOLCHAIN_BASE/libsource/tools/changes"
    CC=$CXX bash build.sh all		>>MeetiX.log 2>&1
    failOnError
popd

echo "[${GREEN}Building${RESET}] ramdisk-writer"
pushd "$TOOLCHAIN_BASE/libsource/tools/ramdisk-writer"
    CC=$CXX bash build.sh all		>>MeetiX.log 2>&1
    failOnError
popd

echo "[${GREEN}Installing${RESET}] pkg-config"
pushd "$TOOLCHAIN_BASE/libsource/tools/pkg-config"
    bash build.sh						>>MeetiX.log 2>&1
    failOnError
popd


# Install headers
echo "[${GREEN}Installing${RESET}] library headers"
export KERNEL_INC=`pwd`"/../source/kernel/inc"
pushd "$TOOLCHAIN_BASE/libsource/libs"
    bash build.sh "header" $KERNEL_INC >>MeetiX.log 2>&1
    failOnError
popd

# Build binutils
if [ $STEP_BUILD_BINUTILS == "1" ];
then
	echo ""
	BUILD_BINUTILS="$BUILD_DIR/build-binutils"
	echo "[${GREEN}Mkdir${RESET}] $BUILD_BINUTILS"
	mkdir -p $BUILD_BINUTILS
	pushd $BUILD_BINUTILS

	echo "[${GREEN}Configuring${RESET}] $BINUTILS_UNPACKED"
	../$BINUTILS_UNPACKED/configure --target=$TARGET --prefix=$TOOLCHAIN_BASE --disable-nls --disable-werror --with-sysroot=$SYSROOT >>MeetiX.log 2>&1
	failOnError

	echo "[${GREEN}Building${RESET}] $BINUTILS_UNPACKED"
	make all $MULTITHREAD			   >>MeetiX.log 2>&1
	failOnError

	echo "[${GREEN}Installing${RESET}] $BINUTILS_UNPACKED"
	make install						>>MeetiX.log 2>&1
	failOnError

	popd

else echo "${CYAN}Skipping build of Binutils${RESET}"
fi


# Build GCC
if [ $STEP_BUILD_GCC == "1" ];
then
	echo ""
	BUILD_GCC="$BUILD_DIR/build-gcc"
	echo "[${GREEN}Mkdir${RESET}] $BUILD_GCC"
	mkdir -p $BUILD_GCC
	pushd $BUILD_GCC

	echo "[${GREEN}Configuring${RESET}] $GCC_UNPACKED"
	../$GCC_UNPACKED/configure --target=$TARGET --prefix=$TOOLCHAIN_BASE --enable-languages=c,c++ --with-sysroot=$SYSROOT --disable--libssp --with-native-system-header-dir="/include" >>MeetiX.log 2>&1
	failOnError

	echo "[${GREEN}Building${RESET}] core"
	make all-gcc $MULTITHREAD			>>MeetiX.log 2>&1
	failOnError

	echo "[${GREEN}Installing${RESET}] core"
	make install-gcc					>>MeetiX.log 2>&1
	failOnError

    echo "[${GREEN}Building${RESET}] libgcc"
	make all-target-libgcc $MULTITHREAD	>>MeetiX.log 2>&1
	failOnError

    echo "[${GREEN}Installing${RESET}] libgcc"
	make install-target-libgcc			>>MeetiX.log 2>&1
	failOnError
	popd
	echo ""

else echo "${CYAN}Skipping build of GCC${RESET}"
fi

# add to path the compilers dir
echo "[${GREEN}PATH${RESET}] $TOOLCHAIN_BASE/bin/"
echo "export PATH=$PATH:$TOOLCHAIN_BASE/bin/" >>$HOME/.bash_profile
echo "export MEETIX_TOOLCHAIN_DIR=$TOOLCHAIN_BASE" >>$HOME/.bash_profile
echo "export MEETIX_LIB=$MEETIX_TOOLCHAIN_DIR/libsource/libs" >>$HOME/.bash_profile
echo "export MEETIX_LINK_STD=static" >>$HOME/.bash_profile

# load as effective changes the new environment variables
source $HOME/.bash_profile

# Build C library
pushd "$TOOLCHAIN_BASE/libsource/libs"
echo "[${GREEN}Building${RESET}] libc"
bash build.sh "c"						    >>MeetiX.log 2>&1
failOnError

# Build kernel api library
echo "[${GREEN}Building${RESET}] kernel api"
bash build.sh "api" KERNEL_INC			    >>MeetiX.log 2>&1
failOnError
popd

# Build libstdc++-v3
echo "[${GREEN}Building${RESET}] libstdc++-v3"
pushd "build/build-gcc"
make all-target-libstdc++-v3 $MULTITHREAD	>>MeetiX.log 2>&1
failOnError

echo "[${GREEN}Installing${RESET}] libstdc++-v3"
make install-target-libstdc++-v3	        >>MeetiX.log 2>&1
failOnError
popd

# build cross port libs
if [ $STEP_BUILD_PORTS == "1" ];
then
	echo ""
    pushd "$TOOLCHAIN_BASE/libsource/ports"

    echo "[${GREEN}Building${RESET}] zlib"
    bash port.sh "zlib"                >>MeetiX.log 2>&1
    failOnError

    echo "[${GREEN}Building${RESET}] pixman"
    bash port.sh "pixman"                >>MeetiX.log 2>&1
    failOnError

    echo "[${GREEN}Building${RESET}] libpng"
    bash port.sh "libpng"                >>MeetiX.log 2>&1
    failOnError

    echo "[${GREEN}Building${RESET}] freetype"
    bash port.sh "freetype"                >>MeetiX.log 2>&1
    failOnError

    echo "[${GREEN}Building${RESET}] cairo"
    bash port.sh "cairo"                >>MeetiX.log 2>&1
    failOnError

    popd
    echo ""

else echo "${CYAN}Skipping ports build${RESET}"
fi

# build graphics library
echo "[${GREEN}Building${RESET}] libGraphics"
pushd "$TOOLCHAIN_BASE/libsource/libs"
bash build.sh "graphic"              >>MeetiX.log 2>&1
failOnError

# build gui library
echo "[${GREEN}Building${RESET}] libGui"
bash build.sh "gui"                  >>MeetiX.log 2>&1
failOnError

# build io library
echo "[${GREEN}Building${RESET}] libIo"
bash build.sh "io"                   >>MeetiX.log 2>&1
failOnError

# build tasking library
echo "[${GREEN}Building${RESET}] libTasking"
bash build.sh "tasking"              >>MeetiX.log 2>&1
failOnError

# build utils library
echo "[${GREEN}Building${RESET}] libUtils"
bash build.sh "utils"                >>MeetiX.log 2>&1
failOnError
popd

# clean
echo ""
echo "[${GREEN}Cleaning${RESET}]"
rm "MeetiX.log"
rm -rf "build"
rm -rf "$TOOLCHAIN_BASE/ports/build"
find $TOOLCHAIN_BASE -name "MeetiX.log" -type f -delete

# set user as owner of the TOOLCHAIN_BASE directory
echo "[${GREEN}Set $SUDO_USER as owner of $TOOLCHAIN_BASE${RESET}]"
sudo chown $SUDO_USER:$(id -gn $SUDO_USER) -R $TOOLCHAIN_BASE

# Finished
echo "${GREEN}Toolchain successfully built${RESET}"
