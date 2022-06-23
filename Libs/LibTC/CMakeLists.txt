#
# @brief
# This file is part of the MeetiX Operating System.
# Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
#
# @developers
# Marco Cicognani (marco.cicognani@meetixos.org)
#
# @license
# GNU General Public License version 3
#

add_library(LibTC STATIC
        Collection/String.cc
        Collection/StringBuilder.cc
        Collection/StringStorage.cc
        Collection/StringView.cc
        Text/Format.cc
        Text/FormatLexer.cc
        Text/FormatParser.cc
        Text/Formatter.cc
        Text/Lexer.cc
        Error.cc
        Hashing.cc
        SourceLocation.cc)