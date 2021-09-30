/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <Api/Common.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

__BEGIN_C

/* ------------------------------------------ C defines ----------------------------------------- */

#define F_OK 0 /* file existence */
#define X_OK 1 /* execute or search permission */
#define W_OK 2 /* write permission */
#define R_OK 3 /* read permission */

#define no_argument       0
#define required_argument 1
#define optional_argument 2 /* unsupported */

extern char** g_environ;
#define environ g_environ

extern char* g_optarg;
#define optarg g_optarg

extern int g_optind;
#define optind g_optind

extern int g_opterr;
#define opterr g_opterr

extern int g_optopt;
#define optopt g_optopt

/* ------------------------------------------- C types ------------------------------------------ */

struct option {
    const char* name;
    int         has_arg;
    int*        flag; /* unsupported */
    int         val;
};

/* ------------------------------------ C function prototypes ----------------------------------- */

int chdir(const char*);

isize read(int, void*, usize);
isize write(int, const void*, usize);

off_t    lseek(int fd, off_t offset, int);
long int tell(int);

int close(int);

void*    sbrk(intptr_t);
unsigned sleep(unsigned);

pid_t getpid();
pid_t getppid();

char* getcwd(char*, usize);
int   isatty(int);
int   access(const char*, int);
int   fcntl(int, int, ...);
int   rmdir(const char*);
int   symlink(const char*, const char*);

int   getopt(int, const char**, const char*);
int   getopt_long(int, const char**, const char*, struct option*, int*);
usize getopt_to_size(const char*);
bool  getopt_is_help(int, char**);

__END_C
