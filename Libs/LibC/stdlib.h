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

#include <Api/Common.h>
#include <malloc.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

CONST_VALUE(EXIT_FAILURE, int, 1);
CONST_VALUE(EXIT_SUCCESS, int, 0);

CONST_VALUE(RAND_MAX, int, 0x7fffffff);

CONST_VALUE(MB_CUR_MAX, size_t, 4);

struct div_t {
    int quot;
    int rem;
};
TYPE_ALIAS(div_t, struct div_t);

struct ldiv_t {
    long quot;
    long rem;
};
TYPE_ALIAS(ldiv_t, struct ldiv_t);

struct lldiv_t {
    long long quot;
    long long rem;
};
TYPE_ALIAS(lldiv_t, struct lldiv_t);

double atof(const char*);

int       atoi(const char*);
long      atol(const char*);
long long atoll(const char*);

float       strtof(const char*, char**);
double      strtod(const char*, char**);
long double strtold(const char*, char**);

long               strtol(const char*, char**, int);
unsigned long      strtoul(const char*, char**, int);
long long          strtoll(const char*, char**, int);
unsigned long long strtoull(const char*, char**, int);

int  rand(void);
void srand(int);

void abort(void) A_NORETURN;
void exit(int) A_NORETURN;
void _Exit(int) A_NORETURN;
void quick_exit(int) A_NORETURN;

int atexit(void (*)(void));
int at_quick_exit(void (*)(void));

char* getenv(const char*);
int   setenv(const char*, const char*, int);

int system(const char*);

void* bsearch(const void*, const void*, size_t, size_t, int (*)(const void*, const void*));
void  qsort(void*, size_t, size_t, int (*)(const void*, const void*));

int       abs(int);
long      labs(long);
long long llabs(long long);

div_t   div(int, int);
ldiv_t  ldiv(long int, long int);
lldiv_t lldiv(long long int, long long int);

int    mblen(const char*, size_t);
int    mbtowc(wchar_t*, const char*, size_t);
int    wctomb(char*, wchar_t);
size_t mbstowcs(wchar_t*, const char*, size_t);
size_t wcstombs(char*, const wchar_t*, size_t);

char* mktemp(char*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
