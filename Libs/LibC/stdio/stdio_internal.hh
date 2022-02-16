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

/* ------------------------------------ C function prototypes ----------------------------------- */

void stdio_init();
void stdio_fini();

extern FILE* g_open_stream_list;

void open_file_list_add(FILE*);
void open_file_list_remove(FILE*);
void open_file_list_lock();
void open_file_list_unlock();

int fclose_static(FILE*);
int fclose_static_unlocked(FILE*);

int parse_mode_flags(const char*);

int fflush_unlocked(FILE*);
int fflush_write(FILE*);
int fflush_write_unlocked(FILE*);
int fflush_read(FILE*);
int fflush_read_unlocked(FILE*);

FILE* fopen_static(const char*, const char*, FILE*);
int   fdopen_static(int, const char*, FILE*);

int setdefbuf_unlocked(FILE*);

usize fwrite_unlocked(const void*, usize, usize, FILE*);
usize fread_unlocked(void*, usize, usize, FILE*);

int   fseeko_unlocked(FILE*, off_t, int);
off_t ftello_unlocked(FILE*);

int fputc_unlocked(int, FILE*);
int fgetc_unlocked(FILE*);
int fungetc_unlocked(int, FILE*);

int setvbuf_unlocked(FILE*, char*, int, usize);

int vfprintf_unlocked(FILE*, const char*, va_list);

void clearerr_unlocked(FILE*);

isize stdio_impl_read(void*, usize, FILE*);
isize stdio_impl_write(const void*, usize, FILE*);
int   stdio_impl_seek(FILE*, off_t, int);
off_t stdio_impl_tell(FILE*);
int   stdio_impl_fileno(FILE*);
int   stdio_impl_close(FILE*);
FILE* stdio_impl_reopen(const char*, const char*, FILE*);
int   stdio_impl_eof(FILE*);
int   stdio_impl_error(FILE*);
void  stdio_impl_seterr(FILE*);
void  stdio_impl_clearerr(FILE*);
