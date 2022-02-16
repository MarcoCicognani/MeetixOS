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
#include <cstring>
#include <iostream>
#include <LibIO/Shell.hh>
#include <LibTasking/LockGuard.hh>
#include <unistd.h>
#include <Utils/Utils.hh>

namespace IO {

Shell& Shell::instance() {
    static Shell* s_instance = nullptr;
    if ( !s_instance )
        s_instance = new Shell{};
    return *s_instance;
}

int Shell::read_char() {
    Tasking::LockGuard guard{ m_buffer_lock };

    /* if there are chars in the m_buffer, take these */
    int c;
    if ( m_buffer && m_buffered_chars > 0 ) {
        c = m_buffer[0];
        memcpy(m_buffer, &m_buffer[1], (m_buffer_len - 1) * sizeof(int));
        --m_buffered_chars;
    } else
        c = read_unbuffered();

    return c;
}

void Shell::write_char(int c) {
    std::cout << SHELLKEY_ESC << "{" << c << "p" << std::flush;
}

void Shell::move_cursor_up(int n) {
    std::cout << SHELLKEY_ESC << "[" << n << "A" << std::flush;
}

void Shell::move_cursor_down(int n) {
    std::cout << SHELLKEY_ESC << "[" << n << "B" << std::flush;
}

void Shell::move_cursor_forward(int n) {
    std::cout << SHELLKEY_ESC << "[" << n << "C" << std::flush;
}

void Shell::move_cursor_back(int n) {
    std::cout << SHELLKEY_ESC << "[" << n << "D" << std::flush;
}

void Shell::clear() {
    std::cout << SHELLKEY_ESC << "[2J" << std::flush;
}

void Shell::scroll(int value) {
    if ( value >= 0 )
        std::cout << SHELLKEY_ESC << "[" << value << "S" << std::flush;
    else
        std::cout << SHELLKEY_ESC << "[" << -value << "T" << std::flush;
}

void Shell::set_echo(bool do_echo) {
    std::cout << SHELLKEY_ESC << "{" << (do_echo ? 1 : 0) << "e" << std::flush;
}

void Shell::set_mode(Mode mode) {
    std::cout << SHELLKEY_ESC << "{" << static_cast<int>(mode) << "m" << std::flush;
}

void Shell::set_cursor(const CursorPosition& position) {
    std::cout << SHELLKEY_ESC << "[" << position.m_y << ";" << position.m_x << "f" << std::flush;
}

void Shell::set_control_process(Pid pid) {
    std::cout << SHELLKEY_ESC << "{" << pid << "c" << std::flush;
}

void Shell::set_scroll_area_to_screen() {
    std::cout << SHELLKEY_ESC << "[r" << std::flush;
}

void Shell::set_scroll_area(int start, int end) {
    std::cout << SHELLKEY_ESC << "[" << start << ";" << end << "r" << std::flush;
}

void Shell::set_cursor_visible(bool visible) {
    std::cout << SHELLKEY_ESC << "{0;" << (visible ? 1 : 0) << "C" << std::flush;
}

Shell::CursorPosition Shell::cursor() {
    /* send the request to the shell */
    std::cout << SHELLKEY_ESC << "[6n" << std::flush;

    /* read the response */
    read_buffered_until_esc();

    auto c = read_unbuffered();
    if ( c == '[' ) {
        int parameters[SHELL_STREAM_CONTROL_MAX_PARAMETERS] = { 0 };

        /* read into parameters */
        int mode = read_escaped_parameters(parameters);
        if ( mode == 'R' )
            return { parameters[1], parameters[0] };
    }
    return { -1, -1 };
}

Shell::Dimension Shell::dimension() {
    /* send the request to the shell */
    std::cout << SHELLKEY_ESC << "{i" << std::flush;

    /* read the response */
    read_buffered_until_esc();

    int c = read_unbuffered();
    if ( c == '{' ) {
        int parameters[SHELL_STREAM_CONTROL_MAX_PARAMETERS] = { 0 };

        /* read into parameters */
        int mode = read_escaped_parameters(parameters);
        if ( mode == 'i' )
            return { parameters[0], parameters[1] };
    }
    return { -1, -1 };
}

int Shell::read_unbuffered() {
    auto c = getc(stdin);
    if ( c == SHELLKEY_SUB ) {
        auto b1 = getc(stdin);
        if ( b1 < 0 )
            return -1;

        auto b2 = getc(stdin);
        if ( b2 < 0 )
            return -1;

        return (b2 << 8) | b1;
    }
    return c;
}

void Shell::buffer_char(int c) {
    Tasking::LockGuard guard{ m_buffer_lock };

    /* allocate the buffer when is null */
    if ( !m_buffer ) {
        m_buffer         = new int[m_buffer_len];
        m_buffered_chars = 0;
    }

    /* put char in m_buffer */
    if ( m_buffered_chars < m_buffer_len )
        m_buffer[m_buffered_chars++] = c;
}

void Shell::read_buffered_until_esc() {
    int c;
    while ( (c = read_unbuffered()) != SHELLKEY_ESC )
        buffer_char(c);
}

int Shell::read_escaped_parameters(int* parameters) {
    int c;
    int param_count = 0;
    while ( true ) {
        c = read_unbuffered();

        if ( c >= '0' && c <= '9' ) {
            if ( param_count == 0 )
                param_count = 1;
            if ( param_count <= SHELL_STREAM_CONTROL_MAX_PARAMETERS ) {
                parameters[param_count - 1] = parameters[param_count - 1] * 10;
                parameters[param_count - 1] = parameters[param_count - 1] + c - '0';
            }
        }

        else if ( c == ';' )
            ++param_count;
        else
            break;
    }
    return c;
}

} /* namespace IO */