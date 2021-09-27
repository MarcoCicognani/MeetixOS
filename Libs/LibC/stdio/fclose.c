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

#include "stdio_internal.h"

#include <malloc.h>
#include <stdio.h>

int fclose(FILE* stream) {
    int res = fclose_static(stream);
    if ( res != 0 )
        return EOF;

    /* destroy the stream */
    free(stream);
    return 0;
}
