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

#include "stdio_internal.hh"

#include <stdio.h>
#include <string.h>

#define STDIO_BUFFER_LEN 1024

FILE  g_stdin;
FILE* g_stdin_ptr = &g_stdin;
char  g_stdin_buf[STDIO_BUFFER_LEN];

FILE  g_stdout;
FILE* g_stdout_ptr = &g_stdout;
char  g_stdout_buf[STDIO_BUFFER_LEN];

FILE  g_stderr;
FILE* g_stderr_ptr = &g_stderr;
char  g_stderr_buf[STDIO_BUFFER_LEN];

void stdio_init() {
    /* this initialization method avoids the use of malloc in the early
     * stage and leaves the task of allocating enough space to the OS,
     * allowing the program to fail on load instead of here, where it
     * could not be handled properly */

    memset(stdin, 0, sizeof(FILE));
    fdopen_static(STDIN_FILENO, "r", stdin);
    setvbuf(stdin, g_stdin_buf, _IOLBF, STDIO_BUFFER_LEN);

    memset(stdout, 0, sizeof(FILE));
    fdopen_static(STDOUT_FILENO, "w", stdout);
    setvbuf(stdout, g_stdout_buf, _IOLBF, STDIO_BUFFER_LEN);

    memset(stderr, 0, sizeof(FILE));
    fdopen_static(STDERR_FILENO, "w", stderr);
    setvbuf(stderr, g_stderr_buf, _IONBF, STDIO_BUFFER_LEN);
}

void stdio_fini() {
    /* flush & close everything */
    auto stream = g_open_stream_list;
    while ( stream ) {
        auto next_stream = stream->m_next_stream;
        if ( s_atomic_try_lock(&stream->m_lock) ) {
            fflush_unlocked(stream);
            fclose_static_unlocked(stream);
            stream->m_lock = false;
        }
        stream = next_stream;
    }
}
