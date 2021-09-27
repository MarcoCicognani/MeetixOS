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

#include <Api/Common.h>

__BEGIN_C

/* ------------------------------------ C function prototypes ----------------------------------- */

void sig_handler_SIG_INT(int signal);
void sig_handler_SIG_IGN(int signal);

__END_C
