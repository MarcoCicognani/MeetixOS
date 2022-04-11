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

#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Option.hh>
#include <LibTC/Text/FormatLexer.hh>

namespace TC {
namespace Text {

class FormatParser {
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

    enum class ShowBase {
        Yes,
        No
    };

    enum class ZeroPad {
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

    struct Specifications {
        char            m_alignment_fill{ ' ' };
        Alignment       m_alignment{ Alignment::Default };
        ShowIntegerSign m_show_integer_sign{ ShowIntegerSign::IfNegative };
        ShowBase        m_show_base{ ShowBase::No };
        ZeroPad         m_zero_pad{ ZeroPad::No };
        Option<usize>   m_width{};
        Option<usize>   m_precision{};
        DisplayAs       m_display_as{ DisplayAs::Default };

        [[nodiscard]] bool display_as_is_numeric() const;
    };

public:
    /**
     * @brief Constructors
     */
    explicit FormatParser(FormatLexer& format_lexer);

    /**
     * @brief Parses the current format string from the given lexer
     */
    ErrorOr<Specifications> try_parse();

private:
    void parse_alignment_fill(Specifications& specifications);
    void parse_alignment(Specifications& specifications);
    void parse_integer_sign(Specifications& specifications);
    void parse_show_base(Specifications& specifications);
    void parse_zero_pad(Specifications& specifications);
    void parse_width(Specifications& specifications);
    void parse_display_as(Specifications& specifications);

private:
    FormatLexer& m_format_lexer;
};

} /* namespace Text */

using Text::FormatParser;

} /* namespace TC */