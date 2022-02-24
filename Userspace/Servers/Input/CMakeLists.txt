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

add_executable(Input.sv Input.cpp Ps2IrqTriggered.cpp Ps2Polling.cpp)
target_include_directories(Input.sv PRIVATE .)
target_link_libraries(Input.sv LibApi LibC LibIO LibUtils)
install(TARGETS Input.sv DESTINATION MeetiX/Kernel/Servers)
