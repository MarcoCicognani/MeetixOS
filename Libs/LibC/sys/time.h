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

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif
