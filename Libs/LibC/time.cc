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

#include <LibApi/Api.h>
#include <LibC/time.h>

extern "C" {

clock_t clock(void) {
    __NOT_IMPLEMENTED(clock);
    return 0;
}

auto time(time_t* out) -> time_t {
    auto millis = static_cast<time_t>(s_millis());

    *out = millis;
    return millis;
}

double difftime(time_t, time_t) {
    __NOT_IMPLEMENTED(difftime);
    return 0;
}

time_t mktime(struct tm*) {
    __NOT_IMPLEMENTED(mktime);
    return 0;
}

usize strftime(char*, usize, const char*, const struct tm*) {
    __NOT_IMPLEMENTED(strftime);
    return 0;
}

char* strptime(const char*, const char*, struct tm*) {
    __NOT_IMPLEMENTED(strptime);
    return nullptr;
}

struct tm* gmtime(const time_t*) {
    __NOT_IMPLEMENTED(gmtime);
    return nullptr;
}

struct tm* localtime(const time_t*) {
    __NOT_IMPLEMENTED(localtime);
    return nullptr;
}

char* asctime(const struct tm*) {
    __NOT_IMPLEMENTED(asctime);
    return nullptr;
}

char* ctime(const time_t*) {
    __NOT_IMPLEMENTED(ctime);
    return nullptr;
}
}