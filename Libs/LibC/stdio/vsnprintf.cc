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
#include <string.h>

typedef struct {
    char* m_out_buffer;
    usize m_out_buffer_len;
    usize m_written_bytes;
    usize m_produced;
} vcbprintf_vsnprintf_callback_data_t;

static isize vcbprintf_vsnprintf_callback(void* param, const char* str, usize len) {
    auto data = reinterpret_cast<vcbprintf_vsnprintf_callback_data_t*>(param);

    if ( data->m_produced < data->m_out_buffer_len ) {
        auto space    = data->m_out_buffer_len - data->m_produced;
        auto writable = len < space ? len : space;

        memcpy(data->m_out_buffer + data->m_produced, str, writable);
        data->m_written_bytes += writable;
    }

    data->m_produced += len;
    return static_cast<isize>(len);
}

extern "C" int vsnprintf(char* s, usize n, const char* format, va_list arg) {
    vcbprintf_vsnprintf_callback_data_t info;
    info.m_out_buffer     = s;
    info.m_out_buffer_len = n ? n - 1 : 0;
    info.m_written_bytes  = 0;
    info.m_produced       = 0;

    int result = vcbprintf(&info, vcbprintf_vsnprintf_callback, format, arg);
    if ( n )
        info.m_out_buffer[info.m_written_bytes] = '\0';
    return result;
}
