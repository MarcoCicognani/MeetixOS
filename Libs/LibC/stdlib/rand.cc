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

#include <cstdint>
#include <cstdlib>

static u64 g_seed;

extern "C" void srand(usize s) {
    g_seed = s - 1;
}

extern "C" int rand(void) {
    return static_cast<int>((g_seed = 9418247712843950125ULL * g_seed + 1) >> 33);
}
