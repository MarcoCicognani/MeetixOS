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

__BEGIN_C

/* ------------------------------------------ C defines ----------------------------------------- */

extern __thread int g_errno;
#undef errno
#define errno g_errno

#define E2BIG           1  /* argument list too long */
#define EACCES          2  /* permission denied */
#define EADDRINUSE      3  /* address already in use */
#define EADDRNOTAVAIL   4  /* can't assign requested address */
#define EAFNOSUPPORT    5  /* address family not supported */
#define EAGAIN          6  /* resource unavailable, try again */
#define EALREADY        7  /* connection already in progress */
#define EBADF           8  /* bad file descriptor */
#define EBADMSG         9  /* bad message */
#define EBUSY           10 /* device or resource busy */
#define ECANCELED       11 /* operation cancelled */
#define ECHILD          12 /* no child processes */
#define ECONNABORTED    13 /* connection aborted */
#define ECONNREFUSED    14 /* connection refused */
#define ECONNRESET      15 /* connection reset */
#define EDEADLK         16 /* resource deadlock would occur */
#define EDESTADDRREQ    17 /* destination address required */
#define EDOM            18 /* mathematics argument out of domain of function */
#define EDQUOT          19 /* reserved */
#define EEXIST          20 /* file exists */
#define EFAULT          21 /* bad address */
#define EFBIG           22 /* file too large */
#define EHOSTUNREACH    23 /* host is unreachable */
#define EIDRM           24 /* identifier removed */
#define EILSEQ          26 /* illegal byte sequence */
#define EINPROGRESS     27 /* operation in progress */
#define EINTR           28 /* interrupted function */
#define EINVAL          29 /* invalid argument */
#define EIO             30 /* I/O error */
#define EISCONN         31 /* socket is connected */
#define EISDIR          32 /* is a directory */
#define ELOOP           33 /* too many levels of symbolic links */
#define EMFILE          34 /* file descriptor value too large */
#define EMLINK          35 /* too many links */
#define EMSGSIZE        36 /* message too large */
#define EMULTIHOP       37 /* reserved */
#define ENAMETOOLONG    38 /* filename too long */
#define ENETDOWN        39 /* network is down */
#define ENETRESET       40 /* connection aborted by network */
#define ENETUNREACH     41 /* network unreachable */
#define ENFILE          42 /* too many files open in system */
#define ENOBUFS         43 /* no buffer space available */
#define ENODEV          45 /* no such device */
#define ENOENT          46 /* no such file or directory */
#define ENOEXEC         47 /* executable file format error */
#define ENOLCK          48 /* no locks available */
#define ENOLINK         49 /* reserved */
#define ENOMEM          50 /* not enough space */
#define ENOMSG          51 /* no message of the desired type */
#define ENOPROTOOPT     52 /* protocol not available */
#define ENOSPC          53 /* no space left on device */
#define ENOSYS          56 /* function not supported */
#define ENOTCONN        57 /* the socket is not connected */
#define ENOTDIR         58 /* not a directory or a symbolic link to a directory */
#define ENOTEMPTY       59 /* directory not empty */
#define ENOTRECOVERABLE 60 /* state not recoverable */
#define ENOTSOCK        61 /* not a socket */
#define ENOTSUP         62 /* not supported (maybe the same value as [EOPNOTSUPP]) */
#define ENOTTY          63 /* inappropriate I/O control operation */
#define ENXIO           64 /* no such device or address */
#define EOPNOTSUPP      65 /* operation not supported on socket (maybe the same value as ENOTSUP) */
#define EOVERFLOW       66 /* value too large to be stored in data type */
#define EOWNERDEAD      67 /* previous owner died */
#define EPERM           68 /* operation not permitted */
#define EPIPE           69 /* broken pipe */
#define EPROTO          70 /* protocol error */
#define EPROTONOSUPPORT 71 /* protocol not supported */
#define EPROTOTYPE      72 /* protocol wrong type for socket */
#define ERANGE          73 /* result too large */
#define EROFS           74 /* read-only file system */
#define ESPIPE          75 /* invalid seek */
#define ESRCH           76 /* no such process */
#define ESTALE          77 /* reserved */
#define ETIMEDOUT       79 /* connection timed out */
#define ETXTBSY         80 /* text file busy */
#define EWOULDBLOCK     81 /* operation would block (maybe the same value as [EAGAIN]) */
#define EXDEV           82 /* cross-device link */

__END_C
