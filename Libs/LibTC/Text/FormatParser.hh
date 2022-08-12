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

    struct Specifications;

public:
    /**
     * @brief Constructors
     */
    explicit FormatParser(FormatLexer& format_lexer);

    /**
     * @brief Parses the current format string from the given lexer
     */
    auto try_parse() -> ErrorOr<Specifications>;

private:
    auto parse_alignment_fill(Specifications& specifications) -> void;
    auto parse_alignment(Specifications& specifications) -> void;
    auto parse_integer_sign(Specifications& specifications) -> void;
    auto parse_show_base(Specifications& specifications) -> void;
    auto parse_zero_pad(Specifications& specifications) -> void;
    auto parse_width(Specifications& specifications) -> void;
    auto parse_display_as(Specifications& specifications) -> void;

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

struct FormatParser::Specifications {
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