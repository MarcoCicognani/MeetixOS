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

#include <stdio.h>

int remove(A_UNUSED const char* filename) {
    __NOT_IMPLEMENTED(remove);
    return 0;
}

int unlink(const char* filename) {
    return remove(filename);
}
