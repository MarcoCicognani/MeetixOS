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

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <LibC/ctype.h>

extern "C" {

const char _ctype_[256] = { _C,      _C,      _C,      _C,      _C,      _C, _C, _C, _C, _C | _S, _C | _S,
                            _C | _S, _C | _S, _C | _S, _C,      _C,      _C, _C, _C, _C, _C,      _C,
                            _C,      _C,      _C,      _C,      _C,      _C, _C, _C, _C, _C,      (char)(_S | _B),
                            _P,      _P,      _P,      _P,      _P,      _P, _P, _P, _P, _P,      _P,
                            _P,      _P,      _P,      _P,      _N,      _N, _N, _N, _N, _N,      _N,
                            _N,      _N,      _N,      _P,      _P,      _P, _P, _P, _P, _P,      _U | _X,
                            _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U, _U, _U, _U, _U,      _U,
                            _U,      _U,      _U,      _U,      _U,      _U, _U, _U, _U, _U,      _U,
                            _U,      _U,      _U,      _P,      _P,      _P, _P, _P, _P, _L | _X, _L | _X,
                            _L | _X, _L | _X, _L | _X, _L | _X, _L,      _L, _L, _L, _L, _L,      _L,
                            _L,      _L,      _L,      _L,      _L,      _L, _L, _L, _L, _L,      _L,
                            _L,      _L,      _P,      _P,      _P,      _P, _C };

#undef isalnum
#undef isalpha
#undef isascii
#undef isblank
#undef iscntrl
#undef isdigit
#undef isgraph
#undef islower
#undef isprint
#undef ispunct
#undef isspace
#undef isupper
#undef isxdigit
#undef toascii
#undef tolower
#undef toupper

int isalnum(int c) {
    return __inline_isalnum(c);
}

int isalpha(int c) {
    return __inline_isalpha(c);
}

int isascii(int c) {
    return __inline_isascii(c);
}

int isblank(int c) {
    return __inline_isblank(c);
}

int iscntrl(int c) {
    return __inline_iscntrl(c);
}

int isdigit(int c) {
    return __inline_isdigit(c);
}

int isgraph(int c) {
    return __inline_isgraph(c);
}

int islower(int c) {
    return __inline_islower(c);
}

int isprint(int c) {
    return __inline_isprint(c);
}

int ispunct(int c) {
    return __inline_ispunct(c);
}

int isspace(int c) {
    return __inline_isspace(c);
}

int isupper(int c) {
    return __inline_isupper(c);
}

int isxdigit(int c) {
    return __inline_isxdigit(c);
}

int toascii(int c) {
    return __inline_toascii(c);
}

int tolower(int c) {
    return __inline_tolower(c);
}

int toupper(int c) {
    return __inline_toupper(c);
}

} /* extern "C" */

#pragma clang diagnostic pop
