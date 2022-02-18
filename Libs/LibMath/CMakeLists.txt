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

add_library(LibMath STATIC math.cc)

add_custom_command(
        TARGET LibMath
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:LibMath> ${TOOLCHAIN_ROOT}/lib/libm.a
)