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

#include <LibTC/Forward.hh>

#include <LibTC/Alloc/Text/Lexer.hh>
#include <LibTC/Lang/DenyCopy.hh>
#include <LibTC/Lang/DenyMove.hh>
#include <LibTC/Lang/StringView.hh>

class FormatLexer final : public Lexer {
    TCDenyCopy$(FormatLexer);
    TCDenyMove$(FormatLexer);

public:
    /**
     * @brief Error safe Factory functions
     */
    [[nodiscard]]
    static constexpr auto construct_from_view(StringView source_view) -> FormatLexer {
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
