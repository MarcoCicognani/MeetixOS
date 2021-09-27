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
#include <stdint.h>

__BEGIN_C

/* ------------------------------------ C function prototypes ----------------------------------- */

int strcasecmp(const char*, const char*);
int strncasecmp(const char*, const char*, usize);

__END_C
