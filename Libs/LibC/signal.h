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

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "bugprone-reserved-identifier"
#pragma ide diagnostic   ignored "modernize-deprecated-headers"
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <Api/signal.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SIG_DFL ((sig_handler_t)0)  /* default action (none) */
#define SIG_IGN ((sig_handler_t)1)  /* ignore handler */
#define SIG_ERR ((sig_handler_t)-1) /* error return value */

TYPE_ALIAS(sig_atomic_t, __SIG_ATOMIC_TYPE__);
TYPE_ALIAS(sig_handler_t, void (*)(int));

sig_handler_t signal(int, sig_handler_t);

int raise(int);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
