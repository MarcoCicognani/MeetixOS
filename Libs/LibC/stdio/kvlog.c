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

#include <malloc.h>
#include <stdio.h>
#include <string.h>

void kvlog(const char* message, va_list l) {
    usize message_len = strlen(message);
    usize buffer_len  = message_len * 4;
    char* buf         = (char*)malloc(buffer_len);

    vsnprintf(buf, buffer_len, message, l);
    s_log(buf);
    free(buf);
}
