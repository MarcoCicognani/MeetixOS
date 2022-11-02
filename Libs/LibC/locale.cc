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

#include <LibApi/Api.h>
#include <LibC/locale.h>

extern "C" {

char* setlocale(int, const char*) {
    __NOT_IMPLEMENTED(localeconv);
    return nullptr;
}

struct lconv* localeconv() {
    __NOT_IMPLEMENTED(localeconv);
    return nullptr;
}

} /* extern "C" */

#pragma clang diagnostic pop
