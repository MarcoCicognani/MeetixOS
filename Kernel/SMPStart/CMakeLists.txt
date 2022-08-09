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

set(CMAKE_ASM_NASM_COMPILE_OBJECT "${CMAKE_ASM_NASM_COMPILER} -f bin -o <OBJECT> -s <SOURCE>")

set(ASM_SOURCES smp.asm)
set_source_files_properties(${ASM_SOURCES} PROPERTIES LANGUAGE ASM_NASM)

add_library(SMPStart.ko STATIC ${ASM_SOURCES})
add_custom_command(
        TARGET SMPStart.ko
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:SMPStart.ko> ${CMAKE_INSTALL_PREFIX}/MeetiX/Kernel/Modules/SMPStart.ko
)
