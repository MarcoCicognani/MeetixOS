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

CONST_VALUE(LC_ALL, int, 0);
CONST_VALUE(LC_COLLATE, int, 1);
CONST_VALUE(LC_CTYPE, int, 2);
CONST_VALUE(LC_MONETARY, int, 3);
CONST_VALUE(LC_NUMERIC, int, 4);
CONST_VALUE(LC_TIME, int, 5);

struct lconv {
    char* decimal_point;      /* default to "." */
    char* thousands_sep;      /* default to "" */
    char* grouping;           /* default to "" */
    char* mon_decimal_point;  /* default to "" */
    char* mon_thousands_sep;  /* default to "" */
    char* mon_grouping;       /* default to "" */
    char* positive_sign;      /* default to "" */
    char* negative_sign;      /* default to "" */
    char* currency_symbol;    /* default to "" */
    char  frac_digits;        /* default to CHAR_MAX */
    char  p_cs_precedes;      /* default to CHAR_MAX */
    char  n_cs_precedes;      /* default to CHAR_MAX */
    char  p_sep_by_space;     /* default to CHAR_MAX */
    char  n_sep_by_space;     /* default to CHAR_MAX */
    char  p_sign_posn;        /* default to CHAR_MAX */
    char  n_sign_posn;        /* default to CHAR_MAX */
    char* int_curr_symbol;    /* default to "" */
    char  int_frac_digits;    /* default to CHAR_MAX */
    char  int_p_cs_precedes;  /* default to CHAR_MAX */
    char  int_n_cs_precedes;  /* default to CHAR_MAX */
    char  int_p_sep_by_space; /* default to CHAR_MAX */
    char  int_n_sep_by_space; /* default to CHAR_MAX */
    char  int_p_sign_posn;    /* default to CHAR_MAX */
    char  int_n_sign_posn;    /* default to CHAR_MAX */
};

char*         setlocale(int, const char*);
struct lconv* localeconv();

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
