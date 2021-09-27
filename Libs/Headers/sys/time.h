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
#include <time.h>

__BEGIN_C

/* ------------------------------------------- C types ------------------------------------------ */

struct timeval {
    time_t      tv_sec;  /* seconds */
    suseconds_t tv_usec; /* micro seconds */
};

struct timezone {
    int tz_minuteswest; /* minutes west of Greenwich */
    int tz_dsttime;     /* type of DST correction */
};

/* ------------------------------------ C function prototypes ----------------------------------- */

int gettimeofday(struct timeval*, struct timezone*);

__END_C