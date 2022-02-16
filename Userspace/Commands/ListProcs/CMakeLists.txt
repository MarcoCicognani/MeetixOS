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
        ListProcs.cc
        MemoryUsage.cc)
add_meetix_command(ListProcs)
target_link_libraries(ListProcs LibIO LibTasking)
