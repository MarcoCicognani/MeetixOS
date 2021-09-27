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

/* ------------------------------------------ C defines ----------------------------------------- */

#define CLOCKS_PER_SEC 1000000L
#define TIME_UTC       1

/* ------------------------------------------- C types ------------------------------------------ */

typedef long time_t;
typedef long suseconds_t;

typedef void* timer_t;
typedef int   clockid_t;
typedef long  clock_t;

struct tm {
    int         tm_sec;   /* Seconds. [0-60] (1 leap second) */
    int         tm_min;   /* Minutes. [0-59] */
    int         tm_hour;  /* Hours.   [0-23] */
    int         tm_mday;  /* Day.     [1-31] */
    int         tm_mon;   /* Month.   [0-11] */
    int         tm_year;  /* Year [1970; ...] */
    int         tm_wday;  /* Day of week. [0-6], 0=Sunday */
    int         tm_yday;  /* Days in year. [0-365] */
    int         tm_isdst; /* Daylight saving [-1/0/1] */
    long        __tm_gmtoff;
    const char* __tm_zone;
};

/* ------------------------------------ C function prototypes ----------------------------------- */

clock_t    clock(void);
time_t     time(time_t*);
double     difftime(time_t, time_t);
time_t     mktime(struct tm*);
usize      strftime(char*, usize, const char*, const struct tm*);
char*      strptime(const char*, const char*, struct tm*);
struct tm* gmtime(const time_t*);
struct tm* localtime(const time_t*);
char*      asctime(const struct tm*);
char*      ctime(const time_t*);

__END_C
