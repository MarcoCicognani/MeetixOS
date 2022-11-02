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

add_library(LibC
        STATIC
        abi_support.cc
        assert.cc
        ctype.cc
        dirent.cc
        dlfcn.cc
        errno.cc
        fcntl.cc
        fenv.cc
        getopt.cc
        lib.cc
        libgen.cc
        locale.cc
        malloc.c
        pthread.cc
        sched.cc
        setjmp.S
        signal.cc
        stdio.cc
        stdlib.cc
        string.cc
        strings.cc
        sys/param.cc
        sys/stat.cc
        sys/time.cc
        time.cc
        unistd.cc
        wchar.cc
        wctype.cc)

target_compile_options(LibC PRIVATE -Wno-narrowing)
target_link_libraries(LibC LibApi LibMath)
add_custom_command(
        TARGET LibC
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:LibC> ${TOOLCHAIN_LIB}/libc.a
)
