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

#include "OptionsParser.hh"

#include <LibC/getopt.h>

extern "C" {
char* g_optarg   = nullptr;
int   g_optind   = 1;
int   g_opterr   = 1;
int   g_optopt   = 0;
int   g_optreset = 0;

int getopt(int argc, const char** argv, const char* short_options) {
    option null_long_option{ nullptr, 0, nullptr, 0 };

    OptionsParser parser{ argc, argv, short_options, &null_long_option, nullptr };
    return parser.next_option();
}

int getopt_long(int            argc,
                const char**   argv,
                const char*    short_options,
                struct option* long_options,
                int*           out_long_option_index) {
    OptionsParser parser{ argc, argv, short_options, long_options, out_long_option_index };
    return parser.next_option();
}
}
