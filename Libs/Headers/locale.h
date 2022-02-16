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
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------ C defines ----------------------------------------- */

#define LC_ALL      0
#define LC_COLLATE  1
#define LC_CTYPE    2
#define LC_MONETARY 3
#define LC_NUMERIC  4
#define LC_TIME     5

/* ------------------------------------------- C types ------------------------------------------ */

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

/* ------------------------------------ C function prototypes ----------------------------------- */

char*         setlocale(int, const char*);
struct lconv* localeconv();

#ifdef __cplusplus
}
#endif
