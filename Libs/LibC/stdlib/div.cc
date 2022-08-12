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

#include <LibC/stdlib.h>

extern "C" div_t div(int numer, int denom) {
    div_t result;
    result.quot = numer / denom;
    result.rem  = numer % denom;
    return result;
}
