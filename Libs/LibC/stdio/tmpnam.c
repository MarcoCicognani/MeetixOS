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
#include <errno.h>
#include <inttypes.h>
#include <malloc.h>
#include <stdio.h>

bool  g_lock    = false;
char* g_buffer  = NULL;
u64   g_next_id = 0;

char* tmpnam(char* buffer) {
    s_atomic_lock((u8*)&g_lock);

    if ( !buffer ) {
        /* allocate the internal buffer if necessary */
        if ( !g_buffer ) {
            g_buffer = (char*)malloc(L_tmpnam);

            if ( !g_buffer ) {
                errno  = ENOMEM;
                g_lock = false;
                return NULL;
            }
        }

        /* use internal buffer */
        buffer = g_buffer;
    }

    /* create temporary name */
    snprintf(buffer, L_tmpnam, "/Tmp/%" PRIu64 "-%" PRIu32, ++g_next_id, s_get_pid());
    g_lock = false;

    return buffer;
}
