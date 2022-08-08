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

cmake_minimum_required(VERSION 3.16.3)
project(MeetixOS C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

include(Meta/CMake/CMakeUtils.txt)

# Project sub-directories
add_subdirectory(Kernel)
add_subdirectory(Libs)
add_subdirectory(Userspace)

# Custom target for build ISO
add_custom_target(repack
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Meta/Boot ${CMAKE_BINARY_DIR}/Image/ISO/boot/grub
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Root ${CMAKE_INSTALL_PREFIX}
        COMMAND ${CMAKE_COMMAND} -E env TOOLCHAIN_BIN=${TOOLCHAIN_BIN}
        ${CMAKE_SOURCE_DIR}/Meta/Repack.sh --out-dir=${CMAKE_BINARY_DIR}
                                           --iso-root=${CMAKE_BINARY_DIR}/Image/ISO
                                           --ramdisk-root=${CMAKE_INSTALL_PREFIX}
        DEPENDS install
        USES_TERMINAL)

# Run and run-debug with build
add_custom_target(run
        COMMAND ${CMAKE_SOURCE_DIR}/Meta/Run.sh --iso-dir=${CMAKE_BINARY_DIR}
        DEPENDS repack
        USES_TERMINAL)
add_custom_target(run_debug
        COMMAND ${CMAKE_SOURCE_DIR}/Meta/Run.sh --iso-dir=${CMAKE_BINARY_DIR} --debug
        DEPENDS repack
        USES_TERMINAL)

# Run and run-debug without build
add_custom_target(just_run
        COMMAND ${CMAKE_SOURCE_DIR}/Meta/Run.sh --iso-dir=${CMAKE_BINARY_DIR}
        USES_TERMINAL)
add_custom_target(just_run_debug
        COMMAND ${CMAKE_SOURCE_DIR}/Meta/Run.sh --iso-dir=${CMAKE_BINARY_DIR} --debug
        USES_TERMINAL)
