/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include <csignal>

int main() {
    /* ignore interruption signals */
    std::signal(SIGINT, SIG_IGN);

    /* cause interruptions */
    while ( true ) {
        asm volatile("hlt");
    }
    __builtin_unreachable();
}
