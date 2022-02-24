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

add_executable(Video.sv Video.cpp)
target_include_directories(Video.sv PRIVATE .)
target_link_libraries(Video.sv LibApi LibC LibIO LibUtils LibGraphics)
install(TARGETS Video.sv DESTINATION MeetiX/Kernel/Servers)
