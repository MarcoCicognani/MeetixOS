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
#include <file.h>
#include <stdarg.h>
#include <stddef.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------ C defines ----------------------------------------- */

#define _IOFBF 1 /* full buffering */
#define _IOLBF 2 /* line buffering */
#define _IONBF 3 /* no buffering */

#define BUFSIZ    0x2000
#define BUFSIZMIN 128

#define EOF (-1)

#define FOPEN_MAX 1024
#define TMP_MAX   FOPEN_MAX
#define L_tmpnam  PATH_MAX

#ifdef SEEK_CUR
#    undef SEEK_CUR
#endif
#define SEEK_CUR 1

#ifdef SEEK_END
#    undef SEEK_END
#endif
#define SEEK_END 2

#ifdef SEEK_SET
#    undef SEEK_SET
#endif
#define SEEK_SET 3

extern FILE* g_stderr_ptr;
#define stderr g_stderr_ptr

extern FILE* g_stdin_ptr;
#define stdin g_stdin_ptr

extern FILE* g_stdout_ptr;
#define stdout g_stdout_ptr

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

/* ------------------------------------------- C types ------------------------------------------ */

typedef off_t fpos_t;

/* ------------------------------------ C function prototypes ----------------------------------- */

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
int  setvbuf(FILE*, char*, int, usize);

/* FILE stream read operations */

int   fgetc(FILE*);
int   getc(FILE*);
char* fgets(char*, int, FILE*);
usize fread(void*, usize, usize, FILE*);
bool  readline(FILE*, char*, usize);
int   fscanf(FILE*, const char*, ...);
int   vfscanf(FILE*, const char*, va_list);

/* FILE stream write operations */

int   putc(int, FILE*);
int   fputc(int, FILE*);
int   fputs(const char*, FILE*);
int   ungetc(int, FILE*);
int   fungetc(int, FILE*);
usize fwrite(const void*, usize, usize, FILE*);
int   fprintf(FILE*, const char*, ...) A_PRINTF(2, 3);
int   vfprintf(FILE*, const char*, va_list);

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

int cbprintf(void*, isize (*)(void*, const char*, usize), const char*, ...) A_PRINTF(3, 4);
int vcbprintf(void*, isize (*)(void*, const char*, usize), const char*, va_list);

/* Standard I/O read functions */

int scanf(const char*, ...);
int getchar();

/* Standard I/O buffer write functions */

int sprintf(char*, const char*, ...) A_PRINTF(2, 3);
int vsprintf(char*, const char*, va_list);
int snprintf(char*, usize, const char*, ...) A_PRINTF(3, 4);
int vsnprintf(char*, usize, const char*, va_list);

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
}
#endif
