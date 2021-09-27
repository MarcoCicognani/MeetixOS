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

#include <ctype.h>

int isutf8(int c) {
    return (c == 0x09 || c == 0x0A || c == 0x0D || (0x20 <= c && c <= 0x7E));
}
