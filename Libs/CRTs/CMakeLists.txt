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

add_library(crt0 STATIC crt0.S)
add_custom_command(
        TARGET crt0
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:crt0> ${TOOLCHAIN_ROOT}/lib/crt0.o
)
