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

#ifndef __EVA_SIGNAL__
#define __EVA_SIGNAL__

#include <Api/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

// standard signals
#define SIGABRT 0
#define SIGFPE  1
#define SIGILL  2
#define SIGINT  3
#define SIGSEGV 4
#define SIGTERM 5

// number of defined signals
#define SIG_COUNT 32

#ifdef __cplusplus
}
#endif

#endif
