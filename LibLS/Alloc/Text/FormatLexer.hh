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

#include <LibLS/Forward.hh>

#include <LibLS/Alloc/Text/Lexer.hh>
#include <LibLS/Lang/DenyCopy.hh>
#include <LibLS/Lang/DenyMove.hh>
#include <LibLS/Lang/StringView.hh>

class FormatLexer final : public Lexer {
    TCDenyCopy$(FormatLexer);
    TCDenyMove$(FormatLexer);

public:
    /**
     * @brief Error safe Factory functions
     */
    [[nodiscard]]
    static constexpr auto new_from_view(StringView source_view) -> FormatLexer {
        return FormatLexer{ source_view };
    }

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
    explicit constexpr FormatLexer(StringView source_view)
        : Lexer{ source_view } {
    }
};
