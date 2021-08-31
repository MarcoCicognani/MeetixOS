#!/bin/bash
. ../../../MXbase.sh

TARGET=$1

# global variables
with TARGET		"all"
with CC			"g++"
with LD			"g++"
with CFLAGS		"-std=c++11"
with ARTIFACT	"ramdisk-writer"
with SRC		"src"
with INC		"inc"
with BIN		"bin"
with INSTALL_TARGET	"$TOOLCHAIN_BASE/bin"

echo "target: $TARGET"
requireTool $CC
requireTool $LD

##
# create and object of each source file in provided source directory
##
function compile()
{
	echo "compiling:"
	for src in $(find "$SRC" -iname "*.cpp"); do
		obj=`sourceToObject $src`
		list $obj
		$CC -c $src -o "$BIN/$obj" -I$INC $CFLAGS
		failOnError
	done
}

##
# link the object in the obj directory and create the executable
##
function link()
{
	echo "linking:"
	$LD -o $ARTIFACT $BIN/*.o
	failOnError
	list $ARTIFACT
}

##
# clean the object directory
##
function clean()
{
	echo "cleaning:"
	cleanDirectory $BIN
}

##
# install the created executable
##
function install()
{
	echo "installing to '$INSTALL_TARGET':"
	mv $ARTIFACT $INSTALL_TARGET
	failOnError
	list $ARTIFACT

	rm $BIN/*.o
}

# execute the command provided
if [[ $TARGET == "all" ]]; then
	clean
	compile
	link
	install

elif [[ $TARGET == "clean" ]]; then
	clean

else
	echo "unknown target: '$TARGET'"
	exit 1
fi

exit 0
