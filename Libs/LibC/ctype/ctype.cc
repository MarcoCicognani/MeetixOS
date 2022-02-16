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

#include <cctype>

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
}
