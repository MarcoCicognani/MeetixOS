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

set(ASM_SOURCES loader.asm)
set_source_files_properties(${ASM_SOURCES} PROPERTIES LANGUAGE ASM_NASM)

add_executable(Loader
        memory/MultibootMmapInterpreter.cpp
        memory/PagingInitializer.cpp
        memory/gdt/GdtManager.cpp
        ${ASM_SOURCES}
        init.cpp
        utils/string.cpp
        loader.cpp
        logger/logger.cpp
        multiboot/MultibootModuleAnalyzer.cpp
        kernelloader/KernelLoader.cpp)

target_include_directories(Loader PRIVATE . ../Shared)
target_link_libraries(Loader Shared)
target_link_options(Loader PRIVATE LINKER:-T ${CMAKE_CURRENT_SOURCE_DIR}/LinkScript.ld -nostdlib -nostartfiles)

install(TARGETS Loader DESTINATION ../ISO/boot)