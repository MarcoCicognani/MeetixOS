#
# @brief
# This file is part of the MeetiX Operating System.
# Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
#
# @developers
# Marco Cicognani (marco.cicognani@meetixos.org)
#
# @license
# GNU General Public License version 3
#

set(SOURCES
        environment.cpp
        interpreter.cpp
        mx.cpp
        mxscript.cpp
        parser.cpp
        shell.cpp)
add_meetix_command(MxSh)
target_link_libraries(MxSh LibIO LibTasking)
