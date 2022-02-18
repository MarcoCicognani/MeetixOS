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

#include <Api/Common.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------ C defines ----------------------------------------- */
#define no_argument       0
#define required_argument 1
#define optional_argument 2

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

/* ------------------------------------------- C types ------------------------------------------ */

struct option {
    const char* name;
    int         has_arg;
    int*        flag;
    int         val;
};

/* ------------------------------------ C function prototypes ----------------------------------- */

int getopt(int, const char**, const char*);
int getopt_long(int, const char**, const char*, struct option*, int*);

#ifdef __cplusplus
}
#endif
