#!/bin/bash
# MeetiX common build functionality

# global colors
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
CYAN=$(tput setaf 6)
RESET=$(tput sgr0)

#
# Prints a header
#
headline() {
    printf "${RED}$1${RESET}\n"
}

targetHeadline() {
    printf "${RED}TARGET: \e[0;7m$1\e[0m\n\n"
}

#
# Overrides a variable if it is not set
#
#   with CC "g++"
#
with() {
    if [ -z "${!1}" ]; then eval $1='$2'; fi
}

#
# Requires the specified tool, quits with an error otherwise
#
#   requireTool "g++"
#
requireTool() {
    if ! type "$1" >/dev/null 2>&1; then
        echo >&2 "error: missing required tool '$1'"
        exit 1
    fi
}

#
# Prints a list entry
#
#   list "example.txt"
#
list() {
    headline " - '$1'"
}

#
# Cleans the given directory
#
#   cleanDirectory "bin"
#
cleanDirectory() {
    echo "[${RED}cleaning directory${RESET}] "$1
    if [ -d "$1" ]; then
        rm -rf $1
    fi
    mkdir "$1"
}

#
# Removes the given file
#
#	remove "file.txt"
#
remove() {
    echo "[${RED}removing${RESET}] "$1
    if [ -e "$1" ]; then
        rm $1 2 &>/dev/null
    fi
}

#
# Fails if the previous command was erroneous
#
#   $CC -c ...
#   failOnError
#
failOnError() {
    if [[ $? != 0 ]]; then
        echo "${RED}Build failed${RESET}"
        exit 1
    fi
}

#
# Converts a source path to an object
#
#   sourceToObject "src/myclass.cpp"
#
sourceToObject() {
    echo $1.o | sed "s/\//_/g"
}

# Global variables
with TOOLCHAIN_BASE $MEETIX_TOOLCHAIN_DIR
SYSROOT=$TOOLCHAIN_BASE
SYSROOT_BIN="$SYSROOT/bin"
SYSROOT_INCLUDE="$SYSROOT/include"
SYSROOT_LIB="$SYSROOT/lib"
