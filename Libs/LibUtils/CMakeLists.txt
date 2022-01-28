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

add_library(LibUtils
        STATIC
        Arguments.cc
        PropertyFileParser.cc
        SHA2.cc
        Environment.cc
        Utils.cc File.cc
        ArgsParser.cc)