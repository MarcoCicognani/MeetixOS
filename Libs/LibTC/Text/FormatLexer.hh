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
#include <LibTC/Text/Lexer.hh>

namespace TC {
namespace Text {

class FormatLexer : public Lexer {
    TC_DENY_COPY(FormatLexer);
    TC_DENY_MOVE(FormatLexer);

public:
    /**
     * @brief Error safe Factory functions
     */
    [[nodiscard]] static constexpr auto construct_from_view(StringView source_view) -> FormatLexer {
        return FormatLexer{ source_view };
    }

    /**
     * @brief Consumes all the non format and escaped characters
     */
    auto consume_literal() -> StringView;

    /**
     * @brief Consumes all the contiguous ASCII numbers
     */
    auto consume_number(usize& value) -> bool;

private:
    explicit constexpr FormatLexer(StringView source_view) noexcept
        : Lexer{ source_view } {
    }
};

} /* namespace Text */

using Text::FormatLexer;

} /* namespace TC */