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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

CONST_VALUE(CLOCKS_PER_SEC, ssize_t, 1000000L);
CONST_VALUE(TIME_UTC, int, 1);

TYPE_ALIAS(time_t, long);
TYPE_ALIAS(suseconds_t, long);

TYPE_ALIAS(timer_t, void*);
TYPE_ALIAS(clockid_t, int);
TYPE_ALIAS(clock_t, long);

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

clock_t    clock(void);
time_t     time(time_t*);
double     difftime(time_t, time_t);
time_t     mktime(struct tm*);
size_t     strftime(char*, size_t, const char*, const struct tm*);
char*      strptime(const char*, const char*, struct tm*);
struct tm* gmtime(const time_t*);
struct tm* localtime(const time_t*);
char*      asctime(const struct tm*);
char*      ctime(const time_t*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
