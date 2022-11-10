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
#pragma clang diagnostic push
#pragma ide diagnostic   ignored "modernize-deprecated-headers"
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

CONST_VALUE(no_argument, int, 0);
CONST_VALUE(required_argument, int, 1);
CONST_VALUE(optional_argument, int, 2);

extern char* g_optarg;
#define optarg g_optarg

extern int g_optind;
#define optind g_optind

extern int g_opterr;
#define opterr g_opterr

extern int g_optopt;
#define optopt g_optopt

extern int g_optreset;
#define optreset g_optreset

struct option {
    const char* name;
    int         has_arg;
    int*        flag;
    int         val;
};

int getopt(int, const char**, const char*);
int getopt_long(int, const char**, const char*, struct option*, int*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
