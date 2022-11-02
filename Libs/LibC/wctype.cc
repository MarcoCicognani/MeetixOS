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
#pragma ide diagnostic ignored "modernize-use-trailing-return-type"

#include <LibApi/Api/User.h>
#include <LibC/assert.h>
#include <LibC/wctype.h>

extern "C" {

int iswalpha(wint_t) {
    __NOT_IMPLEMENTED(iswalpha);
    return 0;
}

int iswblank(wint_t) {
    __NOT_IMPLEMENTED(iswblank);
    return 0;
}

int iswcntrl(wint_t) {
    __NOT_IMPLEMENTED(iswcntrl);
    return 0;
}

int iswdigit(wint_t) {
    __NOT_IMPLEMENTED(iswdigit);
    return 0;
}

int iswgraph(wint_t) {
    __NOT_IMPLEMENTED(iswgraph);
    return 0;
}

int iswlower(wint_t) {
    __NOT_IMPLEMENTED(iswlower);
    return 0;
}

int iswprint(wint_t) {
    __NOT_IMPLEMENTED(iswprint);
    return 0;
}

int iswpunct(wint_t) {
    __NOT_IMPLEMENTED(iswpunct);
    return 0;
}

int iswspace(wint_t) {
    __NOT_IMPLEMENTED(iswspace);
    return 0;
}

int iswupper(wint_t) {
    __NOT_IMPLEMENTED(iswupper);
    return 0;
}

int iswxdigit(wint_t) {
    __NOT_IMPLEMENTED(iswxdigit);
    return 0;
}

int iswctype(wint_t, wctype_t) {
    __NOT_IMPLEMENTED(iswctype);
    return 0;
}

wctype_t wctype(const char*) {
    __NOT_IMPLEMENTED(wctype);
    return nullptr;
}

wint_t towlower(wint_t) {
    __NOT_IMPLEMENTED(towlower);
    return 0;
}

wint_t towupper(wint_t) {
    __NOT_IMPLEMENTED(towupper);
    return 0;
}

wint_t towctrans(wint_t, wctrans_t) {
    __NOT_IMPLEMENTED(towctrans);
    return 0;
}

wctrans_t wctrans(const char*) {
    __NOT_IMPLEMENTED(wctrans);
    return nullptr;
}

} /* extern "C" */

#pragma clang diagnostic pop
