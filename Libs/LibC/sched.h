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

#pragma once

#include <Api/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------ C function prototypes ----------------------------------- */

int sched_yield(void);

#ifdef __cplusplus
}
#endif
