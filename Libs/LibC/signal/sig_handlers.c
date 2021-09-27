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

#include "signal_internal.h"

#include <stdlib.h>

void sig_handler_SIG_INT(A_UNUSED int sig) {
    exit(0);
}

void sig_handler_SIG_IGN(A_UNUSED int sig) {
}
