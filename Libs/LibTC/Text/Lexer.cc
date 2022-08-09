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

auto Lexer::consume() -> char {
    VERIFY_FALSE(is_end());
    return m_source_view[m_index++];
}

auto Lexer::consume_specific(char c) -> bool {
    if ( !next_is(c) )
        return false;
    else {
        ignore();
        return true;
    }
}

auto Lexer::consume_specific(StringView word) -> bool {
    if ( !next_is(word) )
        return false;
    else {
        ignore(word.len());
        return true;
    }
}

auto Lexer::consume_all() -> StringView {
    if ( is_end() )
        return {};

    auto remaining_view = m_source_view.sub_string_view(m_index, m_source_view.len() - m_index);
    m_index             = m_source_view.len();
    return remaining_view;
}

auto Lexer::consume_line() -> StringView {
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

auto Lexer::consume_until(char c) -> StringView {
    usize start_index = m_index;
    while ( !is_end() && next_is(c) )
        ++m_index;

    usize len = m_index - start_index;
    if ( len == 0 )
        return {};
    else
        return m_source_view.sub_string_view(start_index, len);
}

auto Lexer::consume_until(StringView word) -> StringView {
    usize start_index = m_index;
    while ( !is_end() && next_is(word) )
        ++m_index;

    usize len = m_index - start_index;
    if ( len == 0 )
        return {};
    else
        return m_source_view.sub_string_view(start_index, len);
}

auto Lexer::ignore(usize count) -> void {
    m_index += min(count, m_source_view.len() - m_index);
}

auto Lexer::ignore_until(char stop_c) {
    while ( !is_end() && !next_is(stop_c) )
        ++m_index;

    ignore();
}

auto Lexer::ignore_until(StringView stop_word) {
    while ( !is_end() && !next_is(stop_word) )
        ++m_index;

    ignore(stop_word.len());
}

auto Lexer::peek(usize offset) const -> char {
    if ( m_index + offset < m_source_view.len() )
        return m_source_view[m_index + offset];
    else
        return '\0';
}

auto Lexer::next_is(char c) const -> bool {
    return peek() == c;
}

auto Lexer::next_is(StringView word) const -> bool {
    for ( usize i = 0; i < word.len(); ++i ) {
        if ( peek(i) != word[i] )
            return false;
    }
    return true;
}

auto Lexer::remaining() const -> StringView {
    return m_source_view.sub_string_view(m_index);
}

auto Lexer::source_view() const -> StringView {
    return m_source_view;
}

auto Lexer::index() const -> usize {
    return m_index;
}

auto Lexer::left_char_count() const -> usize {
    return m_source_view.len() - m_index;
}

auto Lexer::is_end() const -> bool {
    return m_index >= m_source_view.len();
}

} /* namespace TC::Text */
