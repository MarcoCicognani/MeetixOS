#!/bin/bash

##################################################################################
# MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]					 #
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

echo ""
echo "${GREEN}Building SERVERS${RESET}"

# compile sources
for entry in ./*;
do
    if [ -d $entry ];
	then
        cd $entry
        bash build.sh $1
        cd ..
    fi
done
