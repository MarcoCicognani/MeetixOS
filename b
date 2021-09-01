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

. env.sh
. $MEETIX_TOOLCHAIN_DIR/MXbase.sh

# ovverride function for pushd and popd
function pushd()
{
    command pushd "$@" > /dev/null
}
function popd()
{
    command popd "$@" > /dev/null
}

##
#	call compile of application directory
##
function userspace()
{
	# move to usespace
	pushd "userspace"

	# copy argument
	local buildtarget=$1

	# check if we have to clean
	if [[ $buildtarget == "clean" ]];
	then
		bash compile.sh $buildtarget
		cd ..

	# check for only app build
	elif [[ $buildtarget == "app" ]];
	then
		pushd "applications"
		bash compile.sh "all"
		popd

	# check for only command build
	elif [[ $buildtarget == "cmd" ]];
	then
		pushd "commands"
		bash compile.sh "all"
		popd

	# check for only server build
	elif [[ $buildtarget == "srv" ]];
	then
		pushd "servers"
		bash compile.sh "all"
		popd

	# compile all userspace sources
	elif [[ $buildtarget == "all" ]];
	then
 		bash compile.sh $buildtarget
		popd

	else
		# find the directory with the application
		local finded=$(find . -iname "$buildtarget" -type d)

		# copy the argument for the target
		local argument=$2

		# check find result
		if [[ $finded ]];
		then
			pushd "$finded"
			if [[ $argument ]]; then bash build.sh "$argument"
			else bash build.sh "all"
			fi
			popd

		else
			echo "${RED}$buildtarget application not found${RESET}"
			exit 1
		fi
	fi
}

##
# call the library builder
##
function libraries()
{
	if [[ $1 == "port" ]];
	then
		# go to sysroot port dir
		pushd "$MEETIX_TOOLCHAIN_DIR/libsource/ports"
		if [[ $2 == "all" ]];
		then
			echo "${GREEN}Build thirdy part library${RESET}"
			bash port.sh "zlib"
			failOnError
			bash port.sh "pixman"
			failOnError
			bash port.sh "libpng"
			failOnError
			bash port.sh "freetype"
			failOnError
			bash port.sh "cairo"
			failOnError

		elif [[ $2 == "clean" ]];
		then
			echo "${GREEN}Cleaning thirdy part library${RESET}"
			rm -rf "build"

		else bash port.sh $2
		fi
		popd

	else
		# go to sysroot library source
		pushd "$MEETIX_TOOLCHAIN_DIR/libsource/libs"
		bash build.sh $1 $2
		popd
	fi
}

##
#	call builder of kernel directory
##
function kernel()
{
	# copy argument
	local ktarget=$2

	if [[ $ktarget == "" ]]; then
		ktarget="eva"
	fi

	pushd "kernel"
	bash build.sh $ktarget
	popd
}

##
# return language for new Project
##
function getLanguage()
{
	# copy the argument
	local anguage=$1

	# return only if the provided language is supported
	if [[ $language == "c" || $language == "c++" || $language == "js" ]]; then echo "$language"
	else echo "${RED}Provided an unknow language name${RESET}"
		exit 1
	fi
}

##
# return type for new project
##
function getType()
{
	if [[ $1 == "app" ]]; then echo "application"
	elif [[ $1 == "cmd" ]]; then echo "command"
	elif [[ $1 == "srv" ]]; then echo "server"
	fi
}

##
# create dirs
##
function create()
{
	# create the fundamentals dir for project
	mkdir $1
	mkdir $1"/obj"
	mkdir $1"/src"

	echo "$1"
}

##
# create application directory for new Project
##
function createApp()
{
	# create path
	local dirName="userspace/applications/"$1

	# create the dirs on the ramdisk directory
	mkdir "MXfs/app/"$1
	mkdir "MXfs/app/"$1"/bin"
	mkdir "MXfs/app/"$1"/deps"
	mkdir "MXfs/app/"$1"/deps/icon"

	# return the created path
	echo $(create $dirName)
}

##
# create command directory for new Project
##
function createCommand()
{
	# create path
	local dirName="userspace/commands/"$1

	# return the created path
	echo $(create $dirName)
}

##
# create server directory for new Project
##
function createServer()
{
	# create path
	local dirName="userspace/servers/"$1

	# return the created path
	echo $(create $dirName)
}

##
# create the project with provided type
##
function switchProjectType()
{
	# copy arguments
	local type=$1
	local name=$2

	if [[ $type == "app" ]]; then echo $(createApp $name)
	elif [[ $type == "cmd" ]]; then echo $(createCommand $name)
	elif [[ $type == "srv" ]]; then echo $(createServer $name)
	else echo ""
	fi
}

##
# create the build.sh for the new project
##
function createScript()
{
	# copy arguments
	local dir=$1
	local name=$2
	local type=$(getType $3)
	local language=$(getLanguage $4)
	local builddir=$(pwd)

	# go to new project directory
	cd $dir
	cat > "build.sh" <<EOF
#!/bin/sh
. $MEETIX_TOOLCHAIN_DIR/MXbase.sh

# Define build setup
SRC="src"
INC="src"
OBJ="obj"
ARTIFACT_NAME="$name"
ARTIFACT_TYPE="$type"
COMPILER="i686-mx-$language"
CFLAGS="-std=${language}11 -I$SRC"
LDFLAGS=""

# Include build tasks
. ../../builder.sh
EOF

	return 0
}

##
#  create new developer Project
##
function createNew()
{
	echo "${GREEN}MeetiX Project Creator script${RESET}"

	# check for arguments
	if [ $# -eq 3 ];
	then
		# copy to local variables the arguments
		local type=${1:6}
		local name=${2:6}
		local language=${3:10}

		# save the created dir
		local createdDir=$(switchProjectType $type $name)

		# call type switcher
		if [[ ! -z $createdDir ]]
		then
			# create the script and the main files for the project
			if [[ $(createScript $createdDir $name $type $language) -eq 0 ]];
			then
				echo "${GREEN}Succesful created new project of type $type, named $name with language $language${RESET}"

			# show error if script creation fails
			else
				echo "${RED}Provided an unknow type of language${RESET}"
				exit 1
			fi

		# show error if switch type fails
		else
			echo "${RED}Provided an unknow type of project${RESET}"
			exit 1
		fi

	# show help
	else
		echo "Usage:"
		echo "${RED}bash build.sh new [app/cmd/srv] [name of project] [c/c++/js]${RESET}"
		echo ""

	fi
}

##
# show an help about functionalities of this script
##
function help()
{
	echo "${GREEN}MeetiX OS Main Builder Script${RESET}"
	echo "usage: ${GREEN}$0 [command][arguments...]${RESET}"
	echo "${GREEN}Avaible commands:${RESET}"
	echo "	${CYAN}all${RESET}                                                                [build the kernel and the applications, build the iso and launch the qemu emulator]"
	echo "	${CYAN}lib${RESET}  ${GREEN}[libname/port][portName/all/clean]${RESET}                            [build the libraries on sysroot '${GREEN}$MEETIX_TOOLCHAIN_DIR/libsource${RESET}']"
	echo "	${CYAN}user${RESET} ${GREEN}[app]${RESET}                                                         [build the sources on '${GREEN}userspace${RESET}' directory or the provided application name and install on MXfs directory]"
	echo "	${CYAN}app${RESET}                                                                [build the sources of applications on '${GREEN}userspace${RESET}' directory and install on MXfs directory]"
	echo "	${CYAN}cmd${RESET}                                                                [build the sources of commands on '${GREEN}userspace${RESET}' directory and install on MXfs directory]"
	echo "	${CYAN}svr${RESET}                                                                [build the sources of servers on '${GREEN}userspace${RESET}' directory and install on MXfs directory]"
	echo "	${CYAN}clean${RESET}                                                              [remove the objects and executables files of userspace]"
	echo "	${CYAN}new${RESET} ${GREEN}-type=[app/cmd/svr]${RESET} ${GREEN}-name=[nameOfProject]${RESET} ${GREEN}-language=[c/c++/js]${RESET} [create a new MeetiX user project]"
	echo "	${CYAN}eva${RESET} ${GREEN}[clean/repack/qemu/vbox]${RESET}                                       [build the kernel and the loader]"
}

# copy first argument to local variable
target=$1

# switch arguments
if [[ $target == "all" ]]; then
	kernel "eva"
	userspace "all"
	kernel "" "repack"

elif [[ $target == "lib" ]]; then
	libraries $2 $3

elif [[ $target == "user" ]]; then
	userspace $2

elif [[ $target == "app" ]]; then
	userspace $target

elif [[ $target == "cmd" ]]; then
	userspace $target

elif [[ $target == "svr" ]]; then
	userspace $target

elif [[ $target == "new" ]]; then
	createNew $2 $3 $4

elif [[ $target == "help" ]]; then
	help

elif ( [[ $target == "eva" ]] ); then
	kernel $target $2 $3

else
	echo "${RED}No command provided${RESET}"
fi
