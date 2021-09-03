#ifndef LIBC_H
#define LIBC_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

struct __locale_map;

struct __locale_struct {
	volatile int ctype_utf8;
	char *messages_name;
	struct __locale_map *volatile cat[4];
};

struct __libc {
	int can_do_threads;
	int threaded;
	int secure;
	volatile int threads_minus_1;
	size_t *auxv;
	FILE *ofl_head;
	volatile int ofl_lock[2];
	size_t tls_size;
	size_t page_size;
	volatile int uselocale_cnt;
	volatile int bytelocale_cnt_minus_1;
	struct __locale_struct global_locale;
};

#ifndef PAGE_SIZE
#define PAGE_SIZE libc.page_size
#endif

#ifdef __PIC__
#define ATTR_LIBC_VISIBILITY __attribute__((visibility("hidden")))
#else
#define ATTR_LIBC_VISIBILITY
#endif

extern struct __libc __libc ATTR_LIBC_VISIBILITY;
#define libc __libc

extern size_t __hwcap ATTR_LIBC_VISIBILITY;
extern size_t __sysinfo ATTR_LIBC_VISIBILITY;
extern char *__progname, *__progname_full;

/* Designed to avoid any overhead in non-threaded processes */
void __lock(volatile int *) ATTR_LIBC_VISIBILITY;
void __unlock(volatile int *) ATTR_LIBC_VISIBILITY;
int __lockfile(FILE *) ATTR_LIBC_VISIBILITY;
void __unlockfile(FILE *) ATTR_LIBC_VISIBILITY;
#define LOCK(x) __lock(x)
#define UNLOCK(x) __unlock(x)

void __synccall(void (*)(void *), void *);
int __setxid(int, int, int, int);

extern char **__environ;

#undef weak_alias
#define weak_alias(old, new) \
	extern __typeof(old) new __attribute__((weak, alias(#old)))

#undef LFS64_2
#define LFS64_2(x, y) weak_alias(x, y)

#undef LFS64
#define LFS64(x) LFS64_2(x, x##64)

#endif
