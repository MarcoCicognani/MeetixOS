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

#include <stdint.h>
#include <stdlib.h>

static u64 g_seed;

void srand(unsigned s) {
    g_seed = s - 1;
}

int rand(void) {
    return (int)((g_seed = 9418247712843950125ULL * g_seed + 1) >> 33);
}
