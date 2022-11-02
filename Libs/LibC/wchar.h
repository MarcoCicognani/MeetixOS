/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright () 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
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

#include <file.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

CONST_VALUE(WEOF, wchar_t, 0xffffffffu);

struct tm;
struct mbstate_t {
    size_t __place_older;
};
TYPE_ALIAS(mbstate_t, struct mbstate_t);
TYPE_ALIAS(wint_t, __WINT_TYPE__);

int      fwprintf(FILE*, const wchar_t*, ...);
int      fwscanf(FILE*, const wchar_t*, ...);
int      swprintf(wchar_t*, size_t, const wchar_t*, ...);
int      swscanf(const wchar_t*, const wchar_t*, ...);
int      vfwprintf(FILE*, const wchar_t*, va_list);
int      vfwscanf(FILE*, const wchar_t*, va_list);
int      vswprintf(wchar_t*, size_t, const wchar_t*, va_list);
int      vswscanf(const wchar_t*, const wchar_t*, va_list);
int      vwprintf(const wchar_t*, va_list);
int      vwscanf(const wchar_t*, va_list);
int      wprintf(const wchar_t*, ...);
int      wscanf(const wchar_t*, ...);
wint_t   fgetwc(FILE*);
wchar_t* fgetws(wchar_t*, int, FILE*);
wint_t   fputwc(wchar_t, FILE*);
int      fputws(const wchar_t*, FILE*);
int      fwide(FILE*, int);
wint_t   getwc(FILE*);
wint_t   getwchar();
wint_t   putwc(wchar_t, FILE*);
wint_t   putwchar(wchar_t);
wint_t   ungetwc(wint_t, FILE*);

double      wcstod(const wchar_t*, wchar_t**);
float       wcstof(const wchar_t*, wchar_t**);
long double wcstold(const wchar_t*, wchar_t**);

long int               wcstol(const wchar_t*, wchar_t**, int);
long long int          wcstoll(const wchar_t*, wchar_t**, int);
unsigned long int      wcstoul(const wchar_t*, wchar_t**, int);
unsigned long long int wcstoull(const wchar_t*, wchar_t**, int);

wchar_t* wcscpy(wchar_t*, const wchar_t*);
wchar_t* wcsncpy(wchar_t*, const wchar_t*, size_t);
wchar_t* wmemcpy(wchar_t*, const wchar_t*, size_t);
wchar_t* wmemmove(wchar_t*, const wchar_t*, size_t);
wchar_t* wcscat(wchar_t*, const wchar_t*);
wchar_t* wcsncat(wchar_t*, const wchar_t*, size_t);
int      wcscmp(const wchar_t*, const wchar_t*);
int      wcscoll(const wchar_t*, const wchar_t*);
int      wcsncmp(const wchar_t*, const wchar_t*, size_t);
size_t   wcsxfrm(wchar_t*, const wchar_t*, size_t);
int      wmemcmp(const wchar_t*, const wchar_t*, size_t);
wchar_t* wcschr(const wchar_t*, wchar_t);
size_t   wcscspn(const wchar_t*, const wchar_t*);
wchar_t* wcspbrk(const wchar_t*, const wchar_t*);
wchar_t* wcsrchr(const wchar_t*, wchar_t);
size_t   wcsspn(const wchar_t*, const wchar_t*);
wchar_t* wcsstr(const wchar_t*, const wchar_t*);
wchar_t* wcstok(wchar_t*, const wchar_t*, wchar_t**);
wchar_t* wmemchr(const wchar_t*, wchar_t, size_t);
size_t   wcslen(const wchar_t*);
wchar_t* wmemset(wchar_t*, wchar_t, size_t);
size_t   wcsftime(wchar_t*, size_t, const wchar_t*, const struct tm*);
wint_t   btowc(int);
int      wctob(wint_t);
int      mbsinit(const mbstate_t*);
size_t   mbrlen(const char*, size_t, mbstate_t*);
size_t   mbrtowc(wchar_t*, const char*, size_t, mbstate_t*);
size_t   wcrtomb(char*, wchar_t, mbstate_t*);
size_t   mbsrtowcs(wchar_t*, const char**, size_t, mbstate_t*);
size_t   wcsrtombs(char*, const wchar_t**, size_t, mbstate_t*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
