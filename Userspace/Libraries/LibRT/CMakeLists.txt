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

set(LIBSTC_SOURCES
        ../../../LibLS/Alloc/Details.cc
        ../../../LibLS/Alloc/String.cc
        ../../../LibLS/Alloc/StringBuilder.cc
        ../../../LibLS/Alloc/StringStorage.cc
        ../../../LibLS/Alloc/Text/Format.cc
        ../../../LibLS/Alloc/Text/FormatLexer.cc
        ../../../LibLS/Alloc/Text/FormatParser.cc
        ../../../LibLS/Alloc/Text/Formatter.cc
        ../../../LibLS/Alloc/Text/Lexer.cc
        ../../../LibLS/Core/Assertions.cc
        ../../../LibLS/Core/Error.cc
        ../../../LibLS/Core/Hashing.cc
        ../../../LibLS/Core/SourceLocation.cc
        ../../../LibLS/Lang/StringView.cc)

# --- crt0.cc ---
# These two following commands compile the crt0.cc file then install it into toolchain's lib.
# crt0.o is a special object file which the linker needs to create executables, it must contains the _start() entry point
#
add_library(LibCRT0 STATIC crt0.cc)
add_custom_command(
        TARGET LibCRT0
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:LibCRT0> ${TOOLCHAIN_LIB}/crt0.o
)

add_library(LibRT
        STATIC
        ABISupport.cc
        crt0Impl.cc
        Heap.cc
        New.cc
        ${LIBSTC_SOURCES})
add_custom_command(
        TARGET LibRT
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:LibRT> ${TOOLCHAIN_LIB}/librt.a
)
