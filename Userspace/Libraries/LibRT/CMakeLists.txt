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

set(CCLANG_SOURCES
        ../../../CCLang/Alloc/New.cc
        ../../../CCLang/Alloc/RefCounted.cc
        ../../../CCLang/Alloc/String.cc
        ../../../CCLang/Alloc/StringBuilder.cc
        ../../../CCLang/Alloc/StringStorage.cc
        ../../../CCLang/Alloc/Text/Format.cc
        ../../../CCLang/Alloc/Text/FormatLexer.cc
        ../../../CCLang/Alloc/Text/FormatParser.cc
        ../../../CCLang/Alloc/Text/Formatter.cc
        ../../../CCLang/Alloc/Text/Lexer.cc
        ../../../CCLang/Core/Assertions.cc
        ../../../CCLang/Core/CharTypes.cc
        ../../../CCLang/Core/Error.cc
        ../../../CCLang/Core/Find.cc
        ../../../CCLang/Core/SourceLocation.cc
        ../../../CCLang/Lang/Cxx.cc
        ../../../CCLang/Lang/IntTypes/i8.cc
        ../../../CCLang/Lang/IntTypes/i16.cc
        ../../../CCLang/Lang/IntTypes/i32.cc
        ../../../CCLang/Lang/IntTypes/i64.cc
        ../../../CCLang/Lang/IntTypes/isize.cc
        ../../../CCLang/Lang/IntTypes/u8.cc
        ../../../CCLang/Lang/IntTypes/u16.cc
        ../../../CCLang/Lang/IntTypes/u32.cc
        ../../../CCLang/Lang/IntTypes/u64.cc
        ../../../CCLang/Lang/IntTypes/usize.cc
        ../../../CCLang/Lang/Panic.cc
        ../../../CCLang/Lang/StringView.cc)

# --- crt0.cc ---
# These two following commands compile the crt0.cc file then install it into toolchain's lib directory.
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
        CxxABI.cc
        crt0Impl.cc
        Heap.cc
        ${CCLANG_SOURCES} Panic.cc)
add_custom_command(
        TARGET LibRT
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:LibRT> ${TOOLCHAIN_LIB}/librt.a
)
