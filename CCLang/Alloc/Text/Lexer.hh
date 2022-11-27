/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <CCLang/Forward.hh>

#include <CCLang/Core/Concept.hh>
#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/DenyCopy.hh>
#include <CCLang/Lang/DenyMove.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/StringView.hh>

class Lexer : public DenyCopy, public DenyMove {
public:
    /**
     * @brief Error safe Factory functions
     */
    [[nodiscard]]
    static constexpr auto from_view(StringView source_view) -> Lexer {
        return Lexer(source_view);
    }

    /**
     * @brief Consumes the next character and returns it, return terminator when end is reached
     */
    auto consume() -> char;

    /**
     * @brief Consumes the next character/word only if the following peek(s) return the same character/word
     */
    auto consume_specific(char c) -> bool;
    auto consume_specific(StringView word) -> bool;

    /**
     * @brief Consumes the characters until reached the newline/character/word/callback
     */
    auto consume_all() -> StringView;
    auto consume_line() -> StringView;
    auto consume_until(char) -> StringView;
    auto consume_until(StringView) -> StringView;
    auto consume_until(Predicate<char> auto predicate) -> StringView {
        usize start_index = m_index;
        while ( !is_end() && !predicate(peek()) )
            ++m_index;

        usize len = m_index - start_index;
        if ( len == 0 )
            return {};
        else
            return m_source_view.sub_string_view(start_index, len);
    }
    auto consume_while(Predicate<char> auto predicate) -> StringView {
        usize start_index = m_index;
        while ( !is_end() && predicate(peek()) )
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
    auto ignore(usize count = 1) -> void;

    /**
     * @brief Ignores the characters until reached the character/word/callback
     */
    auto ignore_until(char);
    auto ignore_until(StringView);
    auto ignore_until(Predicate<char> auto predicate) {
        while ( !is_end() && !predicate(peek()) )
            ++m_index;
    }
    auto ignore_while(Predicate<char> auto predicate) {
        while ( !is_end() && predicate(peek()) )
            ++m_index;
    }

    /**
     * @brief Peeks the next character
     */
    [[nodiscard]]
    auto peek(usize offset = 0) const -> char;

    /**
     * @brief Returns whether the following peek(s) matches the given argument
     */
    [[nodiscard]]
    auto next_is(char) const -> bool;
    [[nodiscard]]
    auto next_is(StringView) const -> bool;
    [[nodiscard]]
    auto next_is(Predicate<char> auto predicate) const -> bool {
        return predicate(peek());
    }

    /**
     * @brief Getters
     */
    [[nodiscard]]
    auto remaining() const -> StringView;
    [[nodiscard]]
    auto index() const -> usize;
    [[nodiscard]]
    auto left_char_count() const -> usize;
    [[nodiscard]]
    auto is_end() const -> bool;

protected:
    explicit constexpr Lexer(StringView source_view)
        : m_source_view(Cxx::move(source_view)) {
    }

    [[nodiscard]]
    auto source_view() const -> StringView;

private:
    StringView m_source_view;
    usize      m_index = 0;
};

static auto is_any_of(StringView chars) {
    return [chars](char c) { return chars.contains(c); };
}
