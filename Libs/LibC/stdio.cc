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
#include <LibC/stdio.h>
#include <LibC/stdio.hh>

auto stdio_init() -> void {
    return;
}

auto stdio_fini() -> void {
    return;
}

extern "C" {

int remove(const char*) {
    __NOT_IMPLEMENTED(remove);
    return 0;
}

int unlink(const char*) {
    __NOT_IMPLEMENTED(unlink);
    return 0;
}

int rename(const char*, const char*) {
    __NOT_IMPLEMENTED(rename);
    return 0;
}

FILE* tmpfile() {
    __NOT_IMPLEMENTED(tmpfile);
    return nullptr;
}

char* tmpnam(char*) {
    __NOT_IMPLEMENTED(tmpnam);
    return nullptr;
}

FILE* fopen(const char*, const char*) {
    __NOT_IMPLEMENTED(fopen);
    return nullptr;
}

FILE* freopen(const char*, const char*, FILE*) {
    __NOT_IMPLEMENTED(freopen);
    return nullptr;
}

FILE* fdopen(int, const char*) {
    __NOT_IMPLEMENTED(fdopen);
    return nullptr;
}

void setbuf(FILE*, char*) {
    __NOT_IMPLEMENTED(setbuf);
}

int setvbuf(FILE*, char*, int, size_t) {
    __NOT_IMPLEMENTED(setvbuf);
    return 0;
}

int fgetc(FILE*) {
    __NOT_IMPLEMENTED(fgetc);
    return 0;
}

int getc(FILE*) {
    __NOT_IMPLEMENTED(getc);
    return 0;
}

char* fgets(char*, int, FILE*) {
    __NOT_IMPLEMENTED(fgets);
    return nullptr;
}

size_t fread(void*, size_t, size_t, FILE*) {
    __NOT_IMPLEMENTED(fread);
    return 0;
}

bool readline(FILE*, char*, size_t) {
    __NOT_IMPLEMENTED(readline);
    return false;
}

int fscanf(FILE*, const char*, ...) {
    __NOT_IMPLEMENTED(fscanf);
    return 0;
}

int vfscanf(FILE*, const char*, va_list) {
    __NOT_IMPLEMENTED(vfscanf);
    return 0;
}

int putc(int, FILE*) {
    __NOT_IMPLEMENTED(putc);
    return 0;
}

int fputc(int, FILE*) {
    __NOT_IMPLEMENTED(fputc);
    return 0;
}

int fputs(const char*, FILE*) {
    __NOT_IMPLEMENTED(fputs);
    return 0;
}

int ungetc(int, FILE*) {
    __NOT_IMPLEMENTED(ungetc);
    return 0;
}

int fungetc(int, FILE*) {
    __NOT_IMPLEMENTED(fungetc);
    return 0;
}

size_t fwrite(const void*, size_t, size_t, FILE*) {
    __NOT_IMPLEMENTED(fwrite);
    return 0;
}

int fprintf(FILE*, const char*, ...) {
    __NOT_IMPLEMENTED(fprintf);
    return 0;
}

int vfprintf(FILE*, const char*, va_list) {
    __NOT_IMPLEMENTED(vfprintf);
    return 0;
}

int fseek(FILE*, long int, int) {
    __NOT_IMPLEMENTED(fseek);
    return 0;
}

int fseeko(FILE*, off_t, int) {
    __NOT_IMPLEMENTED(fseeko);
    return 0;
}

int fsetpos(FILE*, const fpos_t*) {
    __NOT_IMPLEMENTED(fsetpos);
    return 0;
}

int fgetpos(FILE*, fpos_t*) {
    __NOT_IMPLEMENTED(fgetpos);
    return 0;
}

long int ftell(FILE*) {
    __NOT_IMPLEMENTED(int);
    return 0;
}

off_t ftello(FILE*) {
    __NOT_IMPLEMENTED(ftello);
    return 0;
}

void rewind(FILE*) {
    __NOT_IMPLEMENTED(rewind);
}

void clearerr(FILE*) {
    __NOT_IMPLEMENTED(clearerr);
}

void fseterr(FILE*) {
    __NOT_IMPLEMENTED(fseterr);
}

int feof(FILE*) {
    __NOT_IMPLEMENTED(feof);
    return 0;
}

int ferror(FILE*) {
    __NOT_IMPLEMENTED(ferror);
    return 0;
}

int fileno(FILE*) {
    __NOT_IMPLEMENTED(fileno);
    return 0;
}

int fflush(FILE*) {
    __NOT_IMPLEMENTED(fflush);
    return 0;
}

int fclose(FILE*) {
    __NOT_IMPLEMENTED(fclose);
    return 0;
}

int printf(const char*, ...) {
    __NOT_IMPLEMENTED(printf);
    return 0;
}

int println(const char*, ...) {
    __NOT_IMPLEMENTED(println);
    return 0;
}

int vprintf(const char*, va_list) {
    __NOT_IMPLEMENTED(vprintf);
    return 0;
}

int putchar(int) {
    __NOT_IMPLEMENTED(putchar);
    return 0;
}

int puts(const char*) {
    __NOT_IMPLEMENTED(puts);
    return 0;
}

int scanf(const char*, ...) {
    __NOT_IMPLEMENTED(scanf);
    return 0;
}

int getchar() {
    __NOT_IMPLEMENTED(getchar);
    return 0;
}

int sprintf(char*, const char*, ...) {
    __NOT_IMPLEMENTED(sprintf);
    return 0;
}

int vsprintf(char*, const char*, va_list) {
    __NOT_IMPLEMENTED(vsprintf);
    return 0;
}

int snprintf(char*, size_t, const char*, ...) {
    __NOT_IMPLEMENTED(snprintf);
    return 0;
}

int vsnprintf(char*, size_t, const char*, va_list) {
    __NOT_IMPLEMENTED(vsnprintf);
    return 0;
}

int sscanf(const char*, const char*, ...) {
    __NOT_IMPLEMENTED(sscanf);
    return 0;
}

int vscanf(const char*, va_list) {
    __NOT_IMPLEMENTED(vscanf);
    return 0;
}

int vsscanf(const char* s, const char*, va_list) {
    __NOT_IMPLEMENTED(vsscanf);
    return 0;
}

void klog(const char*, ...) {
    __NOT_IMPLEMENTED(klog);
}

void kvlog(const char*, va_list) {
    __NOT_IMPLEMENTED(kvlog);
}

FILE* popen(const char*, const char*) {
    __NOT_IMPLEMENTED(popen);
    return nullptr;
}

void perror(const char*) {
    __NOT_IMPLEMENTED(perror);
}

int pclose(FILE*) {
    __NOT_IMPLEMENTED(pclose);
    return 0;
}

} /* extern "C" */

#pragma clang diagnostic pop
