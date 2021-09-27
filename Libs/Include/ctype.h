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

#include <Api/Common.h>

__BEGIN_C

/* ------------------------------------------ C defines ----------------------------------------- */

#define _U (1 << 0) /* Upper case */
#define _L (1 << 1) /* Lower case */
#define _N (1 << 2) /* Digit */
#define _S (1 << 3) /* Space */
#define _P (1 << 4) /* Punctuator */
#define _C (1 << 5) /* Control character */
#define _B (1 << 6) /* Blank */
#define _X (1 << 7) /* Hexadecimal digit */

/* ------------------------------------ C function prototypes ----------------------------------- */

int isascii(int);
int isutf8(int);
int isalnum(int);
int isalpha(int);
int isblank(int);
int iscntrl(int);
int isdigit(int);
int isgraph(int);
int islower(int);
int isprint(int);
int ispunct(int);
int isspace(int);
int isupper(int);
int isxdigit(int);
int tolower(int);
int toupper(int);

__END_C
