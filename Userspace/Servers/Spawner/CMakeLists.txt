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

add_executable(Spawner.sv Elf32Loader.cpp power.cpp spawner.cpp)
target_include_directories(Spawner.sv PRIVATE .)
target_link_libraries(Spawner.sv LibApi LibC LibIO LibUtils LibTasking)
install(TARGETS Spawner.sv DESTINATION MeetiX/Kernel/Servers)
