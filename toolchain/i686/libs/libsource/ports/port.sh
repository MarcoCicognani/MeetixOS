#!/bin/bash
. ../../MXbase.sh
echo "${GREEN}MeetiX OS port installer script${RESET}"

PACKAGE="$1"
BUILD_ROOT="build"
HOST="i686-pc-meetix"
REQUIRES_INSTALL_IN_SOURCE_DIR=0

echo "> ${GREEN}building package${RESET} '$PACKAGE'"

# quit on failure
function fail()
{
	echo "${RED}! $1${RESET}"
	exit 1
}


# check if package parameter was given
if [ -z "$PACKAGE" ]; then
	fail "please supply a package to install"
fi

# check if curl is installed
requireTool curl

# make sure working directory exists
if [ ! -d "$BUILD_ROOT" ]; then
	echo "> ${GREEN}creating empty build directory at${GREEN} '$BUILD_ROOT'${RESET}"
	mkdir -p "$BUILD_ROOT"
fi

# check if port exists
if [ ! -d "$PACKAGE" ]; then
	fail "port not found!"
fi

# set & clear working directory for build
BUILD_DIR="$BUILD_ROOT/$PACKAGE"
echo "> ${GREEN}cleaning build directory${RESET} '$BUILD_DIR'"
if [ -d "$BUILD_DIR" ]; then
	rm -rf "$BUILD_DIR"
fi
mkdir -p "$BUILD_DIR"


# check required port parts
if [ ! -f "$PACKAGE/package.sh" ]; then
	fail "port did not contain package shell"
fi



# include package bash
. $PACKAGE/package.sh
if [ -z "$REMOTE_ARCHIVE" ]; then fail "port did not specify REMOTE_ARCHIVE" 
fi
if [ -z "$ARCHIVE" ];       then fail "port did not specify ARCHIVE" 
fi
if [ -z "$UNPACKED_DIR" ]; then fail "port did not specify UNPACKED_DIR" 
fi

# attempt to download source archive
echo "> ${GREEN}downloading source from${RESET} '$REMOTE_ARCHIVE'..."
curl "$REMOTE_ARCHIVE" -k -o $BUILD_DIR/$ARCHIVE #>>/dev/null 2>&1
if [ $? != 0 ]; then 
	fail "unable to download remote archive" 
fi

# run port unpack task
$(cd "$BUILD_DIR"; portUnpack  | sed 's/^/    /')

# check if unpacked-dir exists
if [ ! -d "$BUILD_DIR/$UNPACKED_DIR" ]; then 
	fail "port did not specify proper unpacked directory" 
fi

# patch it
if [ -f "$PACKAGE/patch.diff" ]; 
then
	echo "> ${GREEN}applying patch${RESET}"
	cd "$BUILD_DIR/$UNPACKED_DIR"
	patch -p1 < "../../../$PACKAGE/patch.diff" | sed 's/^/    /'
	cd "../../.."

else echo "> ${GREEN}no patch in port${RESET}"
fi

# run port install task
echo "> ${GREEN}building port${RESET}"
BACK=$(pwd)
if [ $REQUIRES_INSTALL_IN_SOURCE_DIR == 1 ]; 
	then
	cd "$BUILD_DIR/$UNPACKED_DIR"
else
	cd "$BUILD_DIR"
	mkdir "build"
	cd "build"
fi
portInstall | sed 's/^/    /'
cd $BACK

# clean up
rm -rf $BUILD_ROOT/$PACKAGE

# finish successfully
exit 0
