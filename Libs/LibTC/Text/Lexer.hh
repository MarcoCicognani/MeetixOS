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

#pragma once

#include <LibTC/Collection/StringView.hh>
#include <LibTC/DenyCopy.hh>
#include <LibTC/DenyMove.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/IntTypes.hh>

namespace TC {
namespace Text {

class Lexer {
    TC_DENY_COPY(Lexer);
    TC_DENY_MOVE(Lexer);

public:
    /**
     * @brief Constructors
     */
    explicit Lexer(StringView source_view);

    /**
     * @brief Consumes the next character and returns it, return terminator when end is reached
     */
    char consume();

    /**
     * @brief Consumes the next character/word only if the following peek(s) return the same character/word
     */
    bool consume_specific(char c);
    bool consume_specific(StringView word);

    /**
     * @brief Consumes the characters until reached the newline/character/word/callback
     */
    StringView consume_all();
    StringView consume_line();
    StringView consume_until(char c);
    StringView consume_until(StringView word);

    template<typename CallBack>
    StringView consume_until(CallBack call_back) {
        usize start_index = m_index;
        while ( !is_end() && !call_back(peek()) )
            ++m_index;

        usize len = m_index - start_index;
        if ( len == 0 )
            return {};
        else
            return m_source_view.sub_string_view(start_index, len);
    }
    template<typename CallBack>
    StringView consume_while(CallBack call_back) {
        usize start_index = m_index;
        while ( !is_end() && call_back(peek()) )
            ++m_index;

        usize len = m_index - start_index;
        if ( len == 0 )
            return {};
        else
            return m_source_view.sub_string_view(start_index, len);
    }

    /**
     * @brief Ignores count characters
     */
    void ignore(usize count = 1);

    /**
     * @brief Ignores the characters until reached the character/word/callback
     */
    void ignore_until(char stop_c);
    void ignore_until(StringView stop_word);

    template<typename CallBack>
    void ignore_until(CallBack call_back) {
        while ( !is_end() && !call_back(peek()) )
            ++m_index;
    }
    template<typename CallBack>
    void ignore_while(CallBack call_back) {
        while ( !is_end() && call_back(peek()) )
            ++m_index;
    }

    /**
     * @brief Peeks the next character
     */
    [[nodiscard]] char peek(usize offset = 0) const;

    /**
     * @brief Returns whether the following peek(s) matches the given argument
     */
    [[nodiscard]] bool next_is(char c) const;
    [[nodiscard]] bool next_is(StringView word) const;

    template<typename CallBack>
    [[nodiscard]] bool next_is(CallBack call_back) const {
        return call_back(peek());
    }

    /**
     * @brief Getters
     */
    [[nodiscard]] StringView remaining() const;
    [[nodiscard]] usize      index() const;
    [[nodiscard]] usize      left_char_count() const;
    [[nodiscard]] bool       is_end() const;

protected:
    [[nodiscard]] StringView source_view() const;

private:
    StringView m_source_view{};
    usize      m_index{ 0 };
};

static auto is_any_of(StringView characters) {
    return [characters](char c) { return characters.contains(c); };
}

} /* namespace Text */

using Text::Lexer;

} /* namespace TC */
