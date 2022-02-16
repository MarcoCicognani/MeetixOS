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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _U 01   /* Upper case */
#define _L 02   /* Lower case */
#define _N 04   /* Digit */
#define _S 010  /* Space */
#define _P 020  /* Punctuator */
#define _C 040  /* Control character */
#define _X 0100 /* Blank */
#define _B 0200 /* Hexadecimal digit */

extern const char _ctype_[256];

static inline int __inline_isalnum(int c) {
    return _ctype_[(u8)(c)] & (_U | _L | _N);
}

static inline int __inline_isalpha(int c) {
    return _ctype_[(u8)(c)] & (_U | _L);
}

static inline int __inline_isascii(int c) {
    return (unsigned)c <= 127;
}

static inline int __inline_iscntrl(int c) {
    return _ctype_[(u8)(c)] & (_C);
}

static inline int __inline_isdigit(int c) {
    return _ctype_[(u8)(c)] & (_N);
}

static inline int __inline_isxdigit(int c) {
    return _ctype_[(u8)(c)] & (_N | _X);
}

static inline int __inline_isspace(int c) {
    return _ctype_[(u8)(c)] & (_S);
}

static inline int __inline_ispunct(int c) {
    return _ctype_[(u8)(c)] & (_P);
}

static inline int __inline_isprint(int c) {
    return _ctype_[(u8)(c)] & (_P | _U | _L | _N | _B);
}

static inline int __inline_isgraph(int c) {
    return _ctype_[(u8)(c)] & (_P | _U | _L | _N);
}

static inline int __inline_islower(int c) {
    return _ctype_[(u8)(c)] & (_L);
}

static inline int __inline_isupper(int c) {
    return _ctype_[(u8)(c)] & (_U);
}

static inline int __inline_isblank(int c) {
    return _ctype_[(u8)(c)] & (_B) || (c == '\t');
}

static inline int __inline_toascii(int c) {
    return c & 127;
}

static inline int __inline_tolower(int c) {
    if ( c >= 'A' && c <= 'Z' )
        return c | 0x20;
    else
        return c;
}

static inline int __inline_toupper(int c) {
    if ( c >= 'a' && c <= 'z' )
        return c & ~0x20;
    else
        return c;
}

int isalnum(int);
int isalpha(int);
int isascii(int);
int iscntrl(int);
int isdigit(int);
int isxdigit(int);
int isspace(int);
int ispunct(int);
int isprint(int);
int isgraph(int);
int islower(int);
int isupper(int);
int isblank(int);
int toascii(int);
int tolower(int);
int toupper(int);

#define isalnum  __inline_isalnum
#define isalpha  __inline_isalpha
#define isascii  __inline_isascii
#define iscntrl  __inline_iscntrl
#define isdigit  __inline_isdigit
#define isxdigit __inline_isxdigit
#define isspace  __inline_isspace
#define ispunct  __inline_ispunct
#define isprint  __inline_isprint
#define isgraph  __inline_isgraph
#define islower  __inline_islower
#define isupper  __inline_isupper
#define isblank  __inline_isblank
#define toascii  __inline_toascii
#define tolower  __inline_tolower
#define toupper  __inline_toupper

#ifdef __cplusplus
}
#endif
