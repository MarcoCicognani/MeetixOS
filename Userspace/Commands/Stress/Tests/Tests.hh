/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include "Tests/FileSystem/Create.hh"
#include "Tests/FileSystem/OpenClose.hh"
#include "Tests/FileSystem/ReadWrite.hh"
#include "Tests/Memory/Heap.hh"
#include "Tests/Memory/Stack.hh"
#include "Tests/Threads/Native.hh"
#include "Tests/Threads/POSIX.hh"

#include <cstdint>

// u32 flags_to_bitset(bool run_all, )