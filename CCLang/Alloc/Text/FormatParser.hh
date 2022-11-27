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

#include <CCLang/Alloc/Text/FormatLexer.hh>
#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Option.hh>

class FormatParser final {
public:
    enum class Alignment {
        Default,
        Left,
        Center,
        Right
    };

    enum class ShowIntegerSign {
        KeepSpace,
        IfNegative,
        Yes
    };

    enum class ShowBase : bool {
        Yes,
        No
    };

    enum class ZeroPad : bool {
        Yes,
        No
    };

    enum class DisplayAs {
        Default,
        Binary,
        BinaryUpperCase,
        Octal,
        Decimal,
        Hex,
        HexUpperCase,
        Pointer,
        Char,
        String,
        Float,
        HexFloat,
        HexFloatUpperCase
    };

    struct Result {
        char            m_alignment_fill{ ' ' };
        Alignment       m_alignment{ Alignment::Default };
        ShowIntegerSign m_show_integer_sign{ ShowIntegerSign::IfNegative };
        ShowBase        m_show_base{ ShowBase::No };
        ZeroPad         m_zero_pad{ ZeroPad::No };
        Option<usize>   m_width{};
        Option<usize>   m_precision{};
        DisplayAs       m_display_as{ DisplayAs::Default };

        [[nodiscard]]
        auto display_as_is_numeric() const -> bool;
    };

public:
    /**
     * @brief Error safe Factory functions
     */
    [[nodiscard]]
    static auto from_lexer(FormatLexer& format_lexer) -> FormatParser;

    /**
     * @brief Parses the current format string from the given lexer
     */
    auto try_parse() -> ErrorOr<Result>;

private:
    explicit constexpr FormatParser(FormatLexer& format_lexer)
        : m_format_lexer(format_lexer) {
    }

    auto parse_alignment_fill(Result&) -> void;
    auto parse_alignment(Result&) -> void;
    auto parse_integer_sign(Result&) -> void;
    auto parse_show_base(Result&) -> void;
    auto parse_zero_pad(Result&) -> void;
    auto parse_width(Result&) -> void;
    auto parse_display_as(Result&) -> void;

private:
    FormatLexer& m_format_lexer;
};
