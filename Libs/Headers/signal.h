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
#include <Api/signal.h>

__BEGIN_C

/* ------------------------------------------ C defines ----------------------------------------- */

#define SIG_DFL ((sig_handler_t)0)  /* default action (none) */
#define SIG_IGN ((sig_handler_t)1)  /* ignore handler */
#define SIG_ERR ((sig_handler_t)-1) /* error return value */

/* ------------------------------------------- C types ------------------------------------------ */

typedef __SIG_ATOMIC_TYPE__ sig_atomic_t;
typedef void (*sig_handler_t)(int);

/* ------------------------------------ C function prototypes ----------------------------------- */

sig_handler_t signal(int, sig_handler_t);
int           raise(int);

__END_C
