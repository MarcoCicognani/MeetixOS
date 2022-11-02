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

#include <LibApi/Api/User.h>
#include <LibC/assert.h>
#include <LibC/wchar.h>

extern "C" {

int fwprintf(FILE*, const wchar_t*, ...) {
    __NOT_IMPLEMENTED(fwprintf);
    return 0;
}

int fwscanf(FILE*, const wchar_t*, ...) {
    __NOT_IMPLEMENTED(fwscanf);
    return 0;
}

int swprintf(wchar_t*, size_t, const wchar_t*, ...) {
    __NOT_IMPLEMENTED(swprintf);
    return 0;
}

int swscanf(const wchar_t*, const wchar_t*, ...) {
    __NOT_IMPLEMENTED(swscanf);
    return 0;
}

int vfwprintf(FILE*, const wchar_t*, va_list) {
    __NOT_IMPLEMENTED(vfwprintf);
    return 0;
}

int vfwscanf(FILE*, const wchar_t*, va_list) {
    __NOT_IMPLEMENTED(vfwscanf);
    return 0;
}

int vswprintf(wchar_t*, size_t, const wchar_t*, va_list) {
    __NOT_IMPLEMENTED(vswprintf);
    return 0;
}

int vswscanf(const wchar_t*, const wchar_t*, va_list) {
    __NOT_IMPLEMENTED(vswscanf);
    return 0;
}

int vwprintf(const wchar_t*, va_list) {
    __NOT_IMPLEMENTED(vwprintf);
    return 0;
}

int vwscanf(const wchar_t*, va_list) {
    __NOT_IMPLEMENTED(vwscanf);
    return 0;
}

int wprintf(const wchar_t*, ...) {
    __NOT_IMPLEMENTED(wprintf);
    return 0;
}

int wscanf(const wchar_t*, ...) {
    __NOT_IMPLEMENTED(wscanf);
    return 0;
}

wint_t fgetwc(FILE*) {
    __NOT_IMPLEMENTED(fgetwc);
    return 0;
}

wchar_t* fgetws(wchar_t*, int, FILE*) {
    __NOT_IMPLEMENTED(fgetws);
    return nullptr;
}

wint_t fputwc(wchar_t, FILE*) {
    __NOT_IMPLEMENTED(fputwc);
    return 0;
}

int fputws(const wchar_t*, FILE*) {
    __NOT_IMPLEMENTED(fputws);
    return 0;
}

int fwide(FILE*, int) {
    __NOT_IMPLEMENTED(fwide);
    return 0;
}

wint_t getwc(FILE*) {
    __NOT_IMPLEMENTED(getwc);
    return 0;
}

wint_t getwchar() {
    __NOT_IMPLEMENTED(getwchar);
    return 0;
}

wint_t putwc(wchar_t, FILE*) {
    __NOT_IMPLEMENTED(putwc);
    return 0;
}

wint_t putwchar(wchar_t) {
    __NOT_IMPLEMENTED(putwchar);
    return 0;
}

wint_t ungetwc(wint_t, FILE*) {
    __NOT_IMPLEMENTED(ungetwc);
    return 0;
}

double wcstod(const wchar_t*, wchar_t**) {
    __NOT_IMPLEMENTED(wcstod);
    return 0;
}

float wcstof(const wchar_t*, wchar_t**) {
    __NOT_IMPLEMENTED(wcstof);
    return 0;
}

long double wcstold(const wchar_t*, wchar_t**) {
    __NOT_IMPLEMENTED(double);
    return 0;
}

long int wcstol(const wchar_t*, wchar_t**, int) {
    __NOT_IMPLEMENTED(int);
    return 0;
}

long long int wcstoll(const wchar_t*, wchar_t**, int) {
    __NOT_IMPLEMENTED(long);
    return 0;
}

unsigned long int wcstoul(const wchar_t*, wchar_t**, int) {
    __NOT_IMPLEMENTED(long);
    return 0;
}

unsigned long long int wcstoull(const wchar_t*, wchar_t**, int) {
    __NOT_IMPLEMENTED(long);
    return 0;
}

wchar_t* wcscpy(wchar_t*, const wchar_t*) {
    __NOT_IMPLEMENTED(wcscpy);
    return nullptr;
}

wchar_t* wcsncpy(wchar_t*, const wchar_t*, size_t) {
    __NOT_IMPLEMENTED(wcsncpy);
    return nullptr;
}

wchar_t* wmemcpy(wchar_t*, const wchar_t*, size_t) {
    __NOT_IMPLEMENTED(wmemcpy);
    return nullptr;
}

wchar_t* wmemmove(wchar_t*, const wchar_t*, size_t) {
    __NOT_IMPLEMENTED(wmemmove);
    return nullptr;
}

wchar_t* wcscat(wchar_t*, const wchar_t*) {
    __NOT_IMPLEMENTED(wcscat);
    return nullptr;
}

wchar_t* wcsncat(wchar_t*, const wchar_t*, size_t) {
    __NOT_IMPLEMENTED(wcsncat);
    return nullptr;
}

int wcscmp(const wchar_t*, const wchar_t*) {
    __NOT_IMPLEMENTED(wcscmp);
    return 0;
}

int wcscoll(const wchar_t*, const wchar_t*) {
    __NOT_IMPLEMENTED(wcscoll);
    return 0;
}

int wcsncmp(const wchar_t*, const wchar_t*, size_t) {
    __NOT_IMPLEMENTED(wcsncmp);
    return 0;
}

size_t wcsxfrm(wchar_t*, const wchar_t*, size_t) {
    __NOT_IMPLEMENTED(wcsxfrm);
    return 0;
}

int wmemcmp(const wchar_t*, const wchar_t*, size_t) {
    __NOT_IMPLEMENTED(wmemcmp);
    return 0;
}

wchar_t* wcschr(const wchar_t*, wchar_t) {
    __NOT_IMPLEMENTED(wcschr);
    return nullptr;
}

size_t wcscspn(const wchar_t*, const wchar_t*) {
    __NOT_IMPLEMENTED(wcscspn);
    return 0;
}

wchar_t* wcspbrk(const wchar_t*, const wchar_t*) {
    __NOT_IMPLEMENTED(wcspbrk);
    return nullptr;
}

wchar_t* wcsrchr(const wchar_t*, wchar_t) {
    __NOT_IMPLEMENTED(wcsrchr);
    return nullptr;
}

size_t wcsspn(const wchar_t*, const wchar_t*) {
    __NOT_IMPLEMENTED(wcsspn);
    return 0;
}

wchar_t* wcsstr(const wchar_t*, const wchar_t*) {
    __NOT_IMPLEMENTED(wcsstr);
    return nullptr;
}

wchar_t* wcstok(wchar_t*, const wchar_t*, wchar_t**) {
    __NOT_IMPLEMENTED(wcstok);
    return nullptr;
}

wchar_t* wmemchr(const wchar_t*, wchar_t, size_t) {
    __NOT_IMPLEMENTED(wmemchr);
    return nullptr;
}

size_t wcslen(const wchar_t*) {
    __NOT_IMPLEMENTED(wcslen);
    return 0;
}

wchar_t* wmemset(wchar_t*, wchar_t, size_t) {
    __NOT_IMPLEMENTED(wmemset);
    return nullptr;
}

size_t wcsftime(wchar_t*, size_t, const wchar_t*, const struct tm*) {
    __NOT_IMPLEMENTED(wcsftime);
    return 0;
}

wint_t btowc(int) {
    __NOT_IMPLEMENTED(btowc);
    return 0;
}

int wctob(wint_t) {
    __NOT_IMPLEMENTED(wctob);
    return 0;
}

int mbsinit(const mbstate_t*) {
    __NOT_IMPLEMENTED(mbsinit);
    return 0;
}

size_t mbrlen(const char*, size_t, mbstate_t*) {
    __NOT_IMPLEMENTED(mbrlen);
    return 0;
}

size_t mbrtowc(wchar_t*, const char*, size_t, mbstate_t*) {
    __NOT_IMPLEMENTED(mbrtowc);
    return 0;
}

size_t wcrtomb(char*, wchar_t, mbstate_t*) {
    __NOT_IMPLEMENTED(wcrtomb);
    return 0;
}

size_t mbsrtowcs(wchar_t*, const char**, size_t, mbstate_t*) {
    __NOT_IMPLEMENTED(mbsrtowcs);
    return 0;
}

size_t wcsrtombs(char*, const wchar_t**, size_t, mbstate_t*) {
    __NOT_IMPLEMENTED(wcsrtombs);
    return 0;
}

} /* extern "C" */

#pragma clang diagnostic pop
