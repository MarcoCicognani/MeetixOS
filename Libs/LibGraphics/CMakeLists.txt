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

add_library(LibGraphics
        STATIC
        Video.cc
        Text/Layouter.cc
        Text/FontManager.cc
        Text/Font.cc
        Text/FontLoader.cc
        Context.cc
        Cairo.cc Color.cc)

target_include_directories(LibGraphics PRIVATE ${TOOLCHAIN_INCLUDE}/freetype2)
