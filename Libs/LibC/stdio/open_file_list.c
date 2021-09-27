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

FILE* g_open_stream_list      = NULL;
u8    open_file_list_lockatom = false;

void open_file_list_add(FILE* stream) {
    open_file_list_lock();

    g_open_stream_list->m_prev_stream = stream;
    stream->m_next_stream           = g_open_stream_list;
    g_open_stream_list                = stream;

    open_file_list_unlock();
}

void open_file_list_remove(FILE* stream) {
    open_file_list_lock();

    if ( stream == g_open_stream_list )
        g_open_stream_list = stream->m_next_stream;
    if ( stream->m_prev_stream )
        stream->m_prev_stream->m_next_stream = stream->m_next_stream;
    if ( stream->m_next_stream )
        stream->m_next_stream->m_prev_stream = stream->m_prev_stream;

    open_file_list_unlock();
}

void open_file_list_lock() {
    s_atomic_lock(&open_file_list_lockatom);
}

void open_file_list_unlock() {
    open_file_list_lockatom = false;
}
