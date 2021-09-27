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

#include <stdio.h>
#include <string.h>

#define STDIO_BUFFER_LEN 1024

FILE  _stdin;
FILE* g_stdin_ptr = &_stdin;
char  _stdin_buf[STDIO_BUFFER_LEN];

FILE  _stdout;
FILE* g_stdout_ptr = &_stdout;
char  _stdout_buf[STDIO_BUFFER_LEN];

FILE  _stderr;
FILE* g_stderr_ptr = &_stderr;
char  _stderr_buf[STDIO_BUFFER_LEN];

void stdio_init() {
    /* this initialization method avoids the use of malloc in the early
     * stage and leaves the task of allocating enough space to the OS,
     * allowing the program to fail on load instead of here, where it
     * could not be handled properly */

    memset(stdin, 0, sizeof(FILE));
    fdopen_static(STDIN_FILENO, "r", stdin);
    setvbuf(stdin, _stdin_buf, _IOLBF, STDIO_BUFFER_LEN);

    memset(stdout, 0, sizeof(FILE));
    fdopen_static(STDOUT_FILENO, "w", stdout);
    setvbuf(stdout, _stdout_buf, _IOLBF, STDIO_BUFFER_LEN);

    memset(stderr, 0, sizeof(FILE));
    fdopen_static(STDERR_FILENO, "w", stderr);
    setvbuf(stderr, _stderr_buf, _IONBF, STDIO_BUFFER_LEN);
}

void stdio_fini() {
    /* flush & close everything */
    FILE* stream = g_open_stream_list;
    while ( stream ) {
        FILE* next_stream = stream->m_next_stream;
        if ( s_atomic_try_lock(&next_stream->m_lock) ) {
            fflush_unlocked(stream);
            fclose_static_unlocked(stream);
            next_stream->m_lock = false;
        }
        stream = next_stream;
    }
}
