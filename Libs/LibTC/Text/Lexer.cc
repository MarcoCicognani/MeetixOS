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

#include <LibTC/Assertions.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/Math.hh>
#include <LibTC/Text/Lexer.hh>

namespace TC::Text {

Lexer::Lexer(StringView source_view)
    : m_source_view{ move(source_view) } {
}

char Lexer::consume() {
    VERIFY_FALSE(is_end());
    return m_source_view[m_index++];
}

bool Lexer::consume_specific(char c) {
    if ( !next_is(c) )
        return false;
    else {
        ignore();
        return true;
    }
}

bool Lexer::consume_specific(StringView word) {
    if ( !next_is(word) )
        return false;
    else {
        ignore(word.len());
        return true;
    }
}

StringView Lexer::consume_all() {
    if ( is_end() )
        return {};

    auto remaining_view = m_source_view.sub_string_view(m_index, m_source_view.len() - m_index);
    m_index             = m_source_view.len();
    return remaining_view;
}

StringView Lexer::consume_line() {
    usize start_index = m_index;
    while ( !is_end() && peek() != '\r' && peek() != '\n' )
        ++m_index;

    usize len = m_index - start_index;
    consume_specific('\r');
    consume_specific('\n');
    if ( len == 0 )
        return {};
    else
        return m_source_view.sub_string_view(start_index, len);
}

StringView Lexer::consume_until(char c) {
    usize start_index = m_index;
    while ( !is_end() && next_is(c) )
        ++m_index;

    usize len = m_index - start_index;
    if ( len == 0 )
        return {};
    else
        return m_source_view.sub_string_view(start_index, len);
}

StringView Lexer::consume_until(StringView word) {
    usize start_index = m_index;
    while ( !is_end() && next_is(word) )
        ++m_index;

    usize len = m_index - start_index;
    if ( len == 0 )
        return {};
    else
        return m_source_view.sub_string_view(start_index, len);
}

void Lexer::ignore(usize count) {
    m_index += min(count, m_source_view.len() - m_index);
}

void Lexer::ignore_until(char stop_c) {
    while ( !is_end() && !next_is(stop_c) )
        ++m_index;

    ignore();
}

void Lexer::ignore_until(StringView stop_word) {
    while ( !is_end() && !next_is(stop_word) )
        ++m_index;

    ignore(stop_word.len());
}

char Lexer::peek(usize offset) const {
    if ( m_index + offset < m_source_view.len() )
        return m_source_view[m_index + offset];
    else
        return '\0';
}

bool Lexer::next_is(char c) const {
    return peek() == c;
}

bool Lexer::next_is(StringView word) const {
    for ( usize i = 0; i < word.len(); ++i ) {
        if ( peek(i) != word[i] )
            return false;
    }
    return true;
}

StringView Lexer::remaining() const {
    return m_source_view.sub_string_view(m_index);
}

StringView Lexer::source_view() const {
    return m_source_view;
}

usize Lexer::index() const {
    return m_index;
}

usize Lexer::left_char_count() const {
    return m_source_view.len() - m_index;
}

bool Lexer::is_end() const {
    return m_index >= m_source_view.len();
}

} /* namespace TC::Text */
