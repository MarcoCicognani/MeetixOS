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

#include <Api.h>
#include <malloc.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------ C defines ----------------------------------------- */

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define RAND_MAX 0x7fffffff

#define MB_CUR_MAX ((usize)4)

/* ------------------------------------------- C types ------------------------------------------ */

typedef struct {
    int quot;
    int rem;
} div_t;

typedef struct {
    long quot;
    long rem;
} ldiv_t;

typedef struct {
    long long quot;
    long long rem;
} lldiv_t;

/* ------------------------------------ C function prototypes ----------------------------------- */

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
void srand(usize);

void abort(void) A_NORETURN;
void exit(int) A_NORETURN;
void _Exit(int) A_NORETURN;
void quick_exit(int) A_NORETURN;

int atexit(void (*)(void));
int at_quick_exit(void (*)(void));

char* getenv(const char*);
int   setenv(const char*, const char*, int);

int system(const char*);

bool parse_cli_args(int*, char***);

void* bsearch(const void*, const void*, usize, usize, int (*)(const void*, const void*));
void  qsort(void*, usize, usize, int (*)(const void*, const void*));

int       abs(int);
long      labs(long);
long long llabs(long long);

div_t   div(int, int);
ldiv_t  ldiv(long int, long int);
lldiv_t lldiv(long long int, long long int);

int   mblen(const char*, usize);
int   mbtowc(wchar_t*, const char*, usize);
int   wctomb(char*, wchar_t);
usize mbstowcs(wchar_t*, const char*, usize);
usize wcstombs(char*, const wchar_t*, usize);

char* mktemp(char*);

#ifdef __cplusplus
}
#endif
