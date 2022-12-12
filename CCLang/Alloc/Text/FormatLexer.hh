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

#include <CCLang/Alloc/Text/Lexer.hh>
#include <CCLang/Lang/DenyCopy.hh>
#include <CCLang/Lang/DenyMove.hh>
#include <CCLang/Lang/StringView.hh>

class FormatLexer final : public Lexer {
public:
    /**
     * @brief Error safe Factory functions
     */
    [[nodiscard]]
    static auto from_view(StringView source_view) -> FormatLexer;

    /**
     * @brief Consumes all the non format and escaped characters
     */
    [[nodiscard]]
    auto consume_literal() -> StringView;

    /**
     * @brief Consumes all the contiguous ASCII numbers
     */
    [[nodiscard]]
    auto consume_number(usize&) -> bool;

private:
    explicit FormatLexer(StringView source_view);
};
