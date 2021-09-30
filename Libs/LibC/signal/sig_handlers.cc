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

#include "signal_internal.hh"

#include <stdlib.h>

void sig_handler_SIG_INT(int) {
    exit(0);
}

void sig_handler_SIG_IGN(int) {
}
