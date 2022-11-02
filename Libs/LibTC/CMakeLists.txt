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
        Alloc/String.cc
        Alloc/StringBuilder.cc
        Alloc/StringStorage.cc
        Lang/StringView.cc
        Alloc/Text/Format.cc
        Alloc/Text/FormatLexer.cc
        Alloc/Text/FormatParser.cc
        Alloc/Text/Formatter.cc
        Alloc/Text/Lexer.cc
        Core/Error.cc
        Core/Hashing.cc
        Core/SourceLocation.cc Prelude.cc Alloc/AllocLayout.cc)
