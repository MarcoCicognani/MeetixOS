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
#include <stddef.h>

__BEGIN_C

/* ------------------------------------------- C types ------------------------------------------ */

typedef void* iconv_t;

/* ------------------------------------ C function prototypes ----------------------------------- */

iconv_t iconv_open(const char*, const char*);
usize   iconv(iconv_t, char**, size_t*, char**, usize*);
int     iconv_close(iconv_t);

__END_C
