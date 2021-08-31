#!/bin/bash

##################################################################################
# MeetiX OS By MeetiX OS Project [Marco Cicognani]           					 #
# This program is free software; you can redistribute it and/or  				 #
# modify it under the terms of the GNU General Public License                    #
# as published by the Free Software Foundation; either version 2 				 #
# of the License, or (at your option) any later version.						 #
# 																			     #
# This program is distributed in the hope that it will be useful,                #
# but WITHOUT ANY WARRANTY; without even the implied warranty of 				 #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 #
# GNU General Public License for more details.									 #
# 																				 #
# You should have received a copy of the GNU General Public License				 #
# along with this program; if not, write to the Free Software 					 #
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA #
##################################################################################

. ../../MXbase.sh

# copy the argument
target=$1

# include dirs
INC="inc"
INC_C="$INC/c"
INC_IO="$INC/io"
INC_API="$INC/api"
INC_GUI="$INC/gui"
INC_UTILS="$INC/utils"
INC_DRIVER="$INC/driver"
INC_TASKING="$INC/tasking"
INC_GRAPHIC="$INC/graphics"

# expected a path for kernel includes
if [[ $# -eq 2 ]]; then INC_KERNEL=$2
else INC_KERNEL="/home/marco/Scrivania/MeetiX-OS/source/kernel/inc"
fi

# object dirs
OBJ="obj"
OBJ_C="$OBJ/c"
OBJ_IO="$OBJ/io"
OBJ_CRT="$OBJ/crt"
OBJ_API="$OBJ/api"
OBJ_GUI="$OBJ/gui"
OBJ_UTILS="$OBJ/utils"
OBJ_DRIVER="$OBJ/driver"
OBJ_TASKING="$OBJ/tasking"
OBJ_GRAPHIC="$OBJ/graphics"

# source dirs
SRC="src"
SRC_C="$SRC/c"
SRC_IO="$SRC/io"
SRC_CRT="$SRC/crt"
SRC_API="$SRC/api"
SRC_GUI="$SRC/gui"
SRC_UTILS="$SRC/utils"
SRC_DRIVER="$SRC/driver"
SRC_TASKING="$SRC/tasking"
SRC_GRAPHIC="$SRC/graphics"

# final artifact dirs
ARTIFACT_NAME="lib"
CRT_NAMES=("crt0" "crti" "crtn")
ARTIFACT_NAME_C=$ARTIFACT_NAME"c"
ARTIFACT_NAME_IO=$ARTIFACT_NAME"io"
ARTIFACT_NAME_API=$ARTIFACT_NAME"api"
ARTIFACT_NAME_GUI=$ARTIFACT_NAME"gui"
ARTIFACT_NAME_UTILS=$ARTIFACT_NAME"utils"
ARTIFACT_NAME_DRIVER=$ARTIFACT_NAME"driver"
ARTIFACT_NAME_TASKING=$ARTIFACT_NAME"tasking"
ARTIFACT_NAME_GRAPHIC=$ARTIFACT_NAME"graphics"

ARTIFACT_EXTENSION=""

# compiler option
AR="i686-pc-meetix-ar"
ASSEMBLER="i686-pc-meetix-as"
COMPILER=""
CFLAGS=""

##
# return the the creation type library
##
function getLibType()
{
	if [[ $1 ]];
	then
		if ( [[ $1 == "static" ]] || [[ $1 == "stat" ]] || [[ $1 == "s" ]] ); then echo "static"
		elif ( [[ $1 == "dynamic" ]] || [[ $1 == "dyn" ]] || [[ $1 == "d" ]] ); then echo "dynamic"
		else echo "$MEETIX_LINK_STD"
		fi
	else echo "$MEETIX_LINK_STD"
	fi

}

##
# clean objects
##
function cleanSources()
{
	# clean all objects directory
	cleanDirectory $OBJ_C
	cleanDirectory $OBJ_IO
	cleanDirectory $OBJ_API
	cleanDirectory $OBJ_GUI
	cleanDirectory $OBJ_UTILS
	cleanDirectory $OBJ_DRIVER
	cleanDirectory $OBJ_TASKING
	cleanDirectory $OBJ_GRAPHIC

	# removing .a files
	remove $SYSROOT_LIB/$ARTIFACT_NAME_C".a"
	remove $SYSROOT_LIB/$ARTIFACT_NAME_C".l"
	remove $SYSROOT_LIB/$ARTIFACT_NAME_IO".a"
	remove $SYSROOT_LIB/$ARTIFACT_NAME_IO".l"
	remove $SYSROOT_LIB/$ARTIFACT_NAME_API".a"
	remove $SYSROOT_LIB/$ARTIFACT_NAME_API".l"
	remove $SYSROOT_LIB/$ARTIFACT_NAME_GUI".a"
	remove $SYSROOT_LIB/$ARTIFACT_NAME_GUI".l"
	remove $SYSROOT_LIB/$ARTIFACT_NAME_UTILS".a"
	remove $SYSROOT_LIB/$ARTIFACT_NAME_UTILS".l"
	remove $SYSROOT_LIB/$ARTIFACT_NAME_DRIVER".a"
	remove $SYSROOT_LIB/$ARTIFACT_NAME_DRIVER".l"
	remove $SYSROOT_LIB/$ARTIFACT_NAME_TASKING".a"
	remove $SYSROOT_LIB/$ARTIFACT_NAME_TASKING".l"
	remove $SYSROOT_LIB/$ARTIFACT_NAME_GRAPHIC".a"
	remove $SYSROOT_LIB/$ARTIFACT_NAME_GRAPHIC".l"

	# clear changes files
	changes "--clear"
}

##
# copy all lib headers into the include directory
##
function installHeader()
{
	echo "[${GREEN}Installing headers${RESET}] $SYSROOT_INCLUDE"

	# safe creation of include dir
	mkdir -p $SYSROOT_INCLUDE

	# copy kernel headers
	cp -r $INC_KERNEL/* $SYSROOT_INCLUDE

	# copy api headers
	cp -r $INC_API/* $SYSROOT_INCLUDE

	# copy C headers
	cp -r $INC_C/* $SYSROOT_INCLUDE

	# copy other headers
	for dir in $INC/*;
	do
		# only dirs and non c/api/kernel headers
		if ( [ -d $dir ] && [[ $dir != $INC_C ]] && [[ $dir != $INC_API ]] && [[ $dir != $INC_KERNEL ]] )
		then
			cp -r $dir $SYSROOT_INCLUDE
		fi
	done
}

##
# compile sources from provided include source object directories
##
function compile()
{
	# copy provided arguments
	local INCLUDE=$1
	local SOURCES=$2
	local OBJECTS=$3

	# check if headers have changed
	headersHaveChanged=0
	for file in $(find "$INCLUDE" -iname "*.h" -o -iname "*.hpp"); do
		changes -c $file
		if [ $? -eq 1 ]; then
			headersHaveChanged=1
		fi
		changes -s $file
	done

	# compile sources
	for file in $(find "$SOURCES" -iname "*.c" -o -iname "*.cpp"); do
		changes -c $file
		changed=$?
		if ( [ $headersHaveChanged -eq 1 ] || [ $changed -eq 1 ] );
		then
			out=`sourceToObject $file`

			echo "[${GREEN}$COMPILER${RESET}] $file"
			$COMPILER -c $file -o "$OBJECTS/$out" $CFLAGS
			failOnError

			changes -s $file
		fi
	done

	# assemble sources
	for file in $(find "$SOURCES" -iname "*.s");
	do
		changes -c $file
		changed=$?
		if ( [ $headersHaveChanged -eq 1 ] || [ $changed -eq 1 ] );
		then
			out=`sourceToObject $file`

			echo "[${GREEN}$ASSEMBLER${RESET}] $file"
			$ASSEMBLER -s $file -o "$OBJECTS/$out"
			failOnError

			changes -s $file
		fi
	done
}

##
# create the lib from object dir and provided name
##
function createLib()
{
	# copy provided arguments
	local TYPE=$1
	local OBJ_DIR=$2
	local OUT_NAME=$3

	# show information
	echo "[${GREEN}creating $TYPE $OUT_NAME${RESET}]"

	# create lib from provided type
	if [[ $TYPE == "static" ]];
	then
		# compress the dir
		$AR -r "$OUT_NAME.a" $OBJ_DIR/*.o >/dev/null 2>&1
		ARTIFACT_EXTENSION=".a"

	elif [[ $TYPE == "dynamic" ]];
	then
		# create dynamic linked library
		$COMPILER -shared -Wl,-soname,"lib$OUT_NAME.so" -o "lib$OUT_NAME.so" $OBJ_DIR/*.o
		ARTIFACT_EXTENSION=".l"
	fi

	# chmod
	chmod -R 777 $SYSROOT
}

##
# install in the lib dir the artifact
##
function installLib()
{
	# copy provided argument
	local ARTIFACT=$1

	# remove first the artifact if already exist
	remove $SYSROOT_LIB/$ARTIFACT

	# move the artifact to dir
	echo "[${RED}Installing${RESET}] $ARTIFACT"
	mv $ARTIFACT$ARTIFACT_EXTENSION $SYSROOT_LIB
}

##
#	compile the crts
##
function buildCrts()
{
	echo "${GREEN}building CRTs${RESET}"
	mkdir -p $OBJ_CRT
	for name in ${CRT_NAMES[@]}; do
		echo "[${GREEN}$ASSEMBLER${RESET}] $file"
		$ASSEMBLER $SRC_CRT/"$name.S" -o "$OBJ_CRT/$name.o"
		failOnError
	done
}

##
# build, pack and install the api lib
##
function buildLibApi()
{
	COMPILER="i686-pc-meetix-gcc"
	CFLAGS="-I$INC_API"
	local LIB_TYPE=$(getLibType $1)
	compile $INC_API $SRC_API $OBJ_API
	createLib $LIB_TYPE $OBJ_API $ARTIFACT_NAME_API
	installLib $ARTIFACT_NAME_API
}

##
# build, pack and install the
##
function buildLibC()
{
	# build the main libc
	COMPILER="i686-pc-meetix-gcc"
	CFLAGS="-std=c11 -I$INC_C -I$INC_API -I$SRC_C/musl -Wno-narrowing"
	local LIB_TYPE=$(getLibType $1)
	compile $INC_C $SRC_C $OBJ_C
	buildCrts

	# create the libs
	createLib $LIB_TYPE $OBJ_C $ARTIFACT_NAME_C
	installLib $ARTIFACT_NAME_C

	# install the crts
	echo "[${RED}install${RESET}] $CRT_SRC"
	for name in ${CRT_NAMES[@]}; do
		mv $OBJ_CRT/$name.o $SYSROOT_LIB/$name.o
	done

	# install an empty libm
	echo "[${RED}install${RESET}] empty libm"
	$AR -r "$SYSROOT_LIB/libm.a"
}

##
# build, pack and install the graphics lib
##
function buildLibGraphics()
{
	COMPILER="i686-pc-meetix-g++"
	CFLAGS="-std=c++11 -I$SYSROOT_INCLUDE/freetype2 -I$INC_GRAPHIC -I$SRC_GRAPHIC"
	local LIB_TYPE=$(getLibType $1)
	compile $INC_GRAPHIC $SRC_GRAPHIC $OBJ_GRAPHIC
	createLib $LIB_TYPE $OBJ_GRAPHIC $ARTIFACT_NAME_GRAPHIC
	installLib $ARTIFACT_NAME_GRAPHIC
}

##
# build, pack and install the gui lib
##
function buildLibGui()
{
	COMPILER="i686-pc-meetix-g++"
	CFLAGS="-std=c++11 -I$SYSROOT_INCLUDE/freetype2 -I$INC_GUI -I$SRC_GUI"
	local LIB_TYPE=$(getLibType $1)
	compile $INC_GUI $SRC_GUI $OBJ_GUI
	createLib $LIB_TYPE $OBJ_GUI $ARTIFACT_NAME_GUI
	installLib $ARTIFACT_NAME_GUI
}

##
# build, pack and install the io lib
##
function buildLibIO()
{
	COMPILER="i686-pc-meetix-g++"
	CFLAGS="-std=c++11 -I$SYSROOT_INCLUDE/freetype2 -I$INC_IO -I$SRC_IO"
	local LIB_TYPE=$(getLibType $1)
	compile $INC_IO $SRC_IO $OBJ_IO
	createLib $LIB_TYPE $OBJ_IO $ARTIFACT_NAME_IO
	installLib $ARTIFACT_NAME_IO
}

##
# build, pack and install the driver lib
##
function buildLibDriver()
{
	COMPILER="i686-pc-meetix-g++"
	CFLAGS="-std=c++11 -I$SYSROOT_INCLUDE/freetype2 -I$INC_DRIVER -I$SRC_DRIVER"
	local LIB_TYPE=$(getLibType $1)
	compile $INC_DRIVER $SRC_DRIVER $OBJ_DRIVER
	createLib $LIB_TYPE $OBJ_DRIVER $ARTIFACT_NAME_DRIVER
	installLib $ARTIFACT_NAME_DRIVER
}

##
# build, pack and install the tasking lib
##
function buildLibTasking()
{
	COMPILER="i686-pc-meetix-g++"
	CFLAGS="-std=c++11 -I$SYSROOT_INCLUDE/freetype2 -I$INC_TASKING -I$SRC_TASKING"
	local LIB_TYPE=$(getLibType $1)
	compile $INC_TASKING $SRC_TASKING $OBJ_TASKING
	createLib $LIB_TYPE $OBJ_TASKING $ARTIFACT_NAME_TASKING
	installLib $ARTIFACT_NAME_TASKING
}

##
# build, pack and install the utils lib
##
function buildLibUtils()
{
	COMPILER="i686-pc-meetix-g++"
	CFLAGS="-std=c++1y -I$SYSROOT_INCLUDE/freetype2 -I$INC_UTILS -I$SRC_UTILS"
	local LIB_TYPE=$(getLibType $1)
	compile $INC_UTILS $SRC_UTILS $OBJ_UTILS
	createLib $LIB_TYPE $OBJ_UTILS $ARTIFACT_NAME_UTILS
	installLib $ARTIFACT_NAME_UTILS
}

##
# show the help
##
function help()
{
	echo "${GREEN}MeetiX Project library builder${RESET}"
	echo "You can provide the name of the library that you want to build with type"
	echo "Avaible libs are:"
	echo "${CYAN}c${RESET}................[Musl C library, ported from Ghost OS]"
	echo "${CYAN}api${RESET}..............[Low level C library with Evangelion Syscalls]"
	echo "${CYAN}io${RESET}...............[High level C++ library to manage the system io]"
	echo "${CYAN}gui${RESET}..............[High level C++ library to manage the graphical user interface]"
	echo "${CYAN}utils${RESET}............[High level C++ library with generic utils]"
	echo "${CYAN}driver${RESET}...........[High level C++ library to interact and create with drivers]"
	echo "${CYAN}graphic${RESET}..........[High level C++ library to use low level graphic]"
	echo "${CYAN}tasking${RESET}..........[High level C++ library to manage the so tasking]"
	echo ""
	echo "Avaible library types are:"
	echo "${RED}static/stat/s${RESET}.....[To create statically linked library]"
	echo "${RED}dynamic/dyn/d${RESET}.....[To create dynamically linked library]"
	echo "If you not provide a type as second argument the default is '${GREEN}$MEETIX_LINK_STD${RESET}'"
	echo ""
	echo "Other avaible commands are:"
	echo "${CYAN}header${RESET}...........[Install all library headers on '${GREEN}$SYSROOT_INCLUDE${RESET}' directory]"
	echo "${CYAN}all${RESET}..............[Install the headers and build all libraries]"
	echo "${CYAN}clean${RESET}............[Clean all object and created libraries]"

}

# check mode
if [[ $target == "all" ]];
then
	installHeader $2
	buildLibApi $2
	buildLibC $2
	buildLibGraphics $2
	buildLibGui $2
	buildLibIO $2
	buildLibDriver $2
	buildLibTasking $2
	buildLibUtils $2
elif [[ $target == "c" ]];       then buildLibC $2
elif [[ $target == "io" ]];      then buildLibIO $2
elif [[ $target == "gui" ]];     then buildLibGui $2
elif [[ $target == "api" ]];     then buildLibApi $2
elif [[ $target == "utils" ]];   then buildLibUtils $2
elif [[ $target == "header" ]];  then installHeader $2
elif [[ $target == "driver" ]];  then buildLibDriver $2
elif [[ $target == "graphic" ]]; then buildLibGraphics $2
elif [[ $target == "tasking" ]]; then buildLibTasking $2
elif [[ $target == "clean" ]];   then cleanSources $2
elif [[ $target == "help" ]];    then help
else
	echo "${RED}Unknow target${RESET}: $target"
	exit 1
fi
