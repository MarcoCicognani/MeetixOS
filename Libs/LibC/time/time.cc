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

#include <Api.h>
#include <ctime>

extern "C" time_t time(time_t* out) {
    auto millis = static_cast<time_t>(s_millis());
    *out        = millis;
    return millis;
}
