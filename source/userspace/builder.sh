#!/bin/bash

##################################################################################
# MeetiX OS By MeetiX OS Project [Marco Cicognani]             					 #
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

. $MEETIX_TOOLCHAIN_DIR/MXbase.sh

TARGET=$1
MXfs="../../../MXfs"
ARTIFACT_LOCAL=$OBJ/$ARTIFACT_NAME
ARTIFACT_TARGET=""

requireTool changes

##
# set global target for build
##
function setBuildTarget()
{
	if [[ $ARTIFACT_TYPE == "command" ]]; then ARTIFACT_TARGET=$MXfs"/cmd/"$ARTIFACT_NAME
	elif [[ $ARTIFACT_TYPE == "application" ]]; then ARTIFACT_TARGET=$MXfs"/app/"$ARTIFACT_NAME"/bin/"$ARTIFACT_NAME
	elif [[ $ARTIFACT_TYPE == "server" ]]; then ARTIFACT_TARGET=$MXfs"/sys/eva/server/"$ARTIFACT_NAME
	fi
}

##
#	remove the object from compile dir
##
function targetClean()
{
	remove $ARTIFACT_LOCAL
	cleanDirectory $OBJ
	changes "--clear"
}

##
#	compile all file that it find
##
function targetCompile()
{
	# check if headers have changed
	headersHaveChanged=0
	for file in $(find "$INC" -iname "*.h" -o -iname "*.hpp"); do
		changes -c $file
		if [ $? -eq 1 ]; then
			headersHaveChanged=1
		fi
		changes -s $file
	done

	# compile sources
	for file in $(find "$SRC" -iname "*.c" -o -iname "*.cpp"); do
		changes -c $file
		changed=$?
		if ( [ $headersHaveChanged -eq 1 ] || [ $changed -eq 1 ] ); then
			out=`sourceToObject $file`
			echo "[${GREEN}$COMPILER${RESET}] $file"
			$COMPILER -c $file -o "$OBJ/$out" $CFLAGS
			failOnError
			changes -s $file
		fi
	done
}

##
#	link object created from compilation
##
function targetLink()
{
	echo "[${CYAN}link${RESET}] $ARTIFACT_LOCAL"
	$COMPILER -o $ARTIFACT_LOCAL $OBJ/*.o -lgraphics -lgui -lio -lutils -ltasking $LDFLAGS
}

##
#	clean previous executable in install dir
##
function targetCleanTarget()
{
	echo "[${RED}remove${RESET}] $ARTIFACT_TARGET"
	rm $ARTIFACT_TARGET 2&> /dev/null
}

##
# copy the executable on ramdisk target
##
function targetInstall()
{
	echo "[${RED}install${RESET}]" $ARTIFACT_LOCAL
	cp $ARTIFACT_LOCAL $ARTIFACT_TARGET
}

echo ""
echo "${GREEN}Building: $ARTIFACT_NAME${RESET}"

# execute targets
if [[ $TARGET == "all" ]]; then
	setBuildTarget
	targetCompile
	targetLink
	targetInstall

elif [[ $TARGET == "clean" ]]; then
	setBuildTarget
	targetClean
	targetCleanTarget

else
	echo "unknown target: '$TARGET'"
	exit 1
fi

exit 0
