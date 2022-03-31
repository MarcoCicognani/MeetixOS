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

set(SOURCES
        spawner.cpp
        power.cpp
        Elf32Loader.cpp)
add_meetix_server(Spawner)
target_link_libraries(Spawner.sv LibTasking)
