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
#pragma ide diagnostic   ignored "modernize-deprecated-headers"
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <file.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

CONST_VALUE(_IOFBF, int, 1); /* full buffering */
CONST_VALUE(_IOLBF, int, 2); /* line buffering */
CONST_VALUE(_IONBF, int, 3); /* no buffering */

CONST_VALUE(BUFSIZ, int, 0x2000);
CONST_VALUE(BUFSIZMIN, int, 128);

CONST_VALUE(EOF, int, (-1));

CONST_VALUE(FOPEN_MAX, int, 1024);
CONST_VALUE(TMP_MAX, int, FOPEN_MAX);
CONST_VALUE(L_tmpnam, int, PATH_MAX);

CONST_VALUE(SEEK_CUR, int, 1);
CONST_VALUE(SEEK_END, int, 2);
CONST_VALUE(SEEK_SET, int, 3);

extern FILE* g_stderr_ptr;
#define stderr g_stderr_ptr

extern FILE* g_stdin_ptr;
#define stdin g_stdin_ptr

extern FILE* g_stdout_ptr;
#define stdout g_stdout_ptr

CONST_VALUE(STDIN_FILENO, int, 0);
CONST_VALUE(STDOUT_FILENO, int, 1);
CONST_VALUE(STDERR_FILENO, int, 2);

TYPE_ALIAS(fpos_t, off_t);

/* path operations */

int remove(const char*);
int unlink(const char*);
int rename(const char*, const char*);

FILE* tmpfile();
char* tmpnam(char*);

/* FILE stream opening */

FILE* fopen(const char*, const char*);
FILE* freopen(const char*, const char*, FILE*);
FILE* fdopen(int, const char*);

/* FILE stream buffering management */

void setbuf(FILE*, char*);
int  setvbuf(FILE*, char*, int, size_t);

/* FILE stream read operations */

int    fgetc(FILE*);
int    getc(FILE*);
char*  fgets(char*, int, FILE*);
size_t fread(void*, size_t, size_t, FILE*);
bool   readline(FILE*, char*, size_t);
int    fscanf(FILE*, const char*, ...);
int    vfscanf(FILE*, const char*, va_list);

/* FILE stream write operations */

int    putc(int, FILE*);
int    fputc(int, FILE*);
int    fputs(const char*, FILE*);
int    ungetc(int, FILE*);
int    fungetc(int, FILE*);
size_t fwrite(const void*, size_t, size_t, FILE*);
int    fprintf(FILE*, const char*, ...) A_PRINTF(2, 3);
int    vfprintf(FILE*, const char*, va_list);

/* FILE stream cursor position writers */

int fseek(FILE*, long int, int);
int fseeko(FILE*, off_t, int);
int fsetpos(FILE*, const fpos_t*);

/* FILE stream cursor position getters */

int      fgetpos(FILE*, fpos_t*);
long int ftell(FILE*);
off_t    ftello(FILE*);

/* FILE stream operations */

void rewind(FILE*);
void clearerr(FILE*);
void fseterr(FILE*);
int  feof(FILE*);
int  ferror(FILE*);
int  fileno(FILE*);
int  fflush(FILE*);
int  fclose(FILE*);

/* Standard I/O write functions */

int printf(const char*, ...) A_PRINTF(1, 2);
int println(const char*, ...) A_PRINTF(1, 2);
int vprintf(const char*, va_list);
int putchar(int);
int puts(const char*);

/* Standard I/O read functions */

int scanf(const char*, ...);
int getchar();

/* Standard I/O buffer write functions */

int sprintf(char*, const char*, ...) A_PRINTF(2, 3);
int vsprintf(char*, const char*, va_list);
int snprintf(char*, size_t, const char*, ...) A_PRINTF(3, 4);
int vsnprintf(char*, size_t, const char*, va_list);

/* Standard I/O buffer read functions */

int sscanf(const char*, const char*, ...);
int vscanf(const char*, va_list);
int vsscanf(const char* s, const char*, va_list);

/* Custom MeetiX functions to print formatted to s_log() */

void klog(const char*, ...) A_PRINTF(1, 2);
void kvlog(const char*, va_list);

/* Pipe interface through FILE stream */

FILE* popen(const char*, const char*);
void  perror(const char*);
int   pclose(FILE*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
