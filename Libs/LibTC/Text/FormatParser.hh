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

#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Option.hh>
#include <LibTC/Text/FormatLexer.hh>

namespace TC {
namespace Text {

class FormatParser {
public:
    enum class Alignment;
    enum class ShowIntegerSign;
    enum class ShowBase;
    enum class ZeroPad;
    enum class DisplayAs;

    struct Result;

public:
    /**
     * @brief Error safe Factory functions
     */
    [[nodiscard]]
    static auto construct_from_lexer(FormatLexer& format_lexer) -> FormatParser;

    /**
     * @brief Parses the current format string from the given lexer
     */
    auto try_parse() -> ErrorOr<Result>;

private:
    explicit constexpr FormatParser(FormatLexer& format_lexer)
        : m_format_lexer{ format_lexer } {
    }

    auto parse_alignment_fill(Result& result) -> void;
    auto parse_alignment(Result& result) -> void;
    auto parse_integer_sign(Result& result) -> void;
    auto parse_show_base(Result& result) -> void;
    auto parse_zero_pad(Result& result) -> void;
    auto parse_width(Result& result) -> void;
    auto parse_display_as(Result& result) -> void;

private:
    FormatLexer& m_format_lexer;
};

enum class FormatParser::Alignment {
    Default,
    Left,
    Center,
    Right
};

enum class FormatParser::ShowIntegerSign {
    KeepSpace,
    IfNegative,
    Yes
};

enum class FormatParser::ShowBase {
    Yes,
    No
};

enum class FormatParser::ZeroPad {
    Yes,
    No
};

enum class FormatParser::DisplayAs {
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

struct FormatParser::Result {
    char            m_alignment_fill{ ' ' };
    Alignment       m_alignment{ Alignment::Default };
    ShowIntegerSign m_show_integer_sign{ ShowIntegerSign::IfNegative };
    ShowBase        m_show_base{ ShowBase::No };
    ZeroPad         m_zero_pad{ ZeroPad::No };
    Option<usize>   m_width{};
    Option<usize>   m_precision{};
    DisplayAs       m_display_as{ DisplayAs::Default };

    [[nodiscard]] auto display_as_is_numeric() const -> bool;
};

} /* namespace Text */

using Text::FormatParser;

} /* namespace TC */