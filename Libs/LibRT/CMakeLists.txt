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

add_library(LibRT
        STATIC
        crt0.cc Runtime.cc)
target_link_libraries(LibRT LibTC)
add_custom_command(
        TARGET LibRT
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:LibRT> ${TOOLCHAIN_LIB}/librt.a
)
