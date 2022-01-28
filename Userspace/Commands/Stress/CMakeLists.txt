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
        Stress.cc
        Tests/Test.cc
        Tests/FileSystem/Create.cc
        Tests/FileSystem/OpenClose.cc
        Tests/FileSystem/ReadWrite.cc
        Tests/Memory/Heap.cc
        Tests/Memory/Stack.cc
        Tests/Threads/Native.cc
        Tests/Threads/POSIX.cc
        Tests/Manager.cc)
add_command(Stress)
