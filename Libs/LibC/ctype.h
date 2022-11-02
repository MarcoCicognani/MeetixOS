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
#pragma ide diagnostic   ignored "bugprone-reserved-identifier"
#pragma ide diagnostic   ignored "modernize-deprecated-headers"
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

CONST_VALUE(_U, uint8_t, 0001); /* Upper case */
CONST_VALUE(_L, uint8_t, 0002); /* Lower case */
CONST_VALUE(_N, uint8_t, 0004); /* Digit */
CONST_VALUE(_S, uint8_t, 0010); /* Space */
CONST_VALUE(_P, uint8_t, 0020); /* Punctuator */
CONST_VALUE(_C, uint8_t, 0040); /* Control character */
CONST_VALUE(_X, uint8_t, 0100); /* Blank */
CONST_VALUE(_B, uint8_t, 0200); /* Hexadecimal digit */

extern const char _ctype_[256];

static inline int __inline_isalnum(int c) {
    return _ctype_[(uint8_t)(c)] & (_U | _L | _N);
}

static inline int __inline_isalpha(int c) {
    return _ctype_[(uint8_t)(c)] & (_U | _L);
}

static inline int __inline_isascii(int c) {
    return (uint32_t)c <= 127;
}

static inline int __inline_iscntrl(int c) {
    return _ctype_[(uint8_t)(c)] & _C;
}

static inline int __inline_isdigit(int c) {
    return _ctype_[(uint8_t)(c)] & _N;
}

static inline int __inline_isxdigit(int c) {
    return _ctype_[(uint8_t)(c)] & (_N | _X);
}

static inline int __inline_isspace(int c) {
    return _ctype_[(uint8_t)(c)] & _S;
}

static inline int __inline_ispunct(int c) {
    return _ctype_[(uint8_t)(c)] & _P;
}

static inline int __inline_isprint(int c) {
    return _ctype_[(uint8_t)(c)] & (_P | _U | _L | _N | _B);
}

static inline int __inline_isgraph(int c) {
    return _ctype_[(uint8_t)(c)] & (_P | _U | _L | _N);
}

static inline int __inline_islower(int c) {
    return _ctype_[(uint8_t)(c)] & _L;
}

static inline int __inline_isupper(int c) {
    return _ctype_[(uint8_t)(c)] & _U;
}

static inline int __inline_isblank(int c) {
    return _ctype_[(uint8_t)(c)] & _B || (c == '\t');
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
} /* extern "C" */
#endif

#pragma clang diagnostic pop
