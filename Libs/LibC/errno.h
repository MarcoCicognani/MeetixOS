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
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#ifdef __cplusplus
extern "C" {
#endif

enum ErrnoCode {
    ENOERR,          /* no error */
    E2BIG,           /* argument list too long */
    EACCES,          /* permission denied */
    EADDRINUSE,      /* address already in use */
    EADDRNOTAVAIL,   /* can't assign requested address */
    EAFNOSUPPORT,    /* address family not supported */
    EAGAIN,          /* resource unavailable, try again */
    EALREADY,        /* connection already in progress */
    EBADF,           /* bad file descriptor */
    EBADMSG,         /* bad message */
    EBUSY,           /* device or resource busy */
    ECANCELED,       /* operation cancelled */
    ECHILD,          /* no child processes */
    ECONNABORTED,    /* connection aborted */
    ECONNREFUSED,    /* connection refused */
    ECONNRESET,      /* connection reset */
    EDEADLK,         /* resource deadlock would occur */
    EDESTADDRREQ,    /* destination address required */
    EDOM,            /* mathematics argument out of domain of function */
    EDQUOT,          /* reserved */
    EEXIST,          /* file exists */
    EFAULT,          /* bad address */
    EFBIG,           /* file too large */
    EHOSTUNREACH,    /* host is unreachable */
    EIDRM,           /* identifier removed */
    EILSEQ,          /* illegal byte sequence */
    EINPROGRESS,     /* operation in progress */
    EINTR,           /* interrupted function */
    EINVAL,          /* invalid argument */
    EIO,             /* I/O error */
    EISCONN,         /* socket is connected */
    EISDIR,          /* is a directory */
    ELOOP,           /* too many levels of symbolic links */
    EMFILE,          /* file descriptor value too large */
    EMLINK,          /* too many links */
    EMSGSIZE,        /* message too large */
    EMULTIHOP,       /* reserved */
    ENAMETOOLONG,    /* filename too long */
    ENETDOWN,        /* network is down */
    ENETRESET,       /* connection aborted by network */
    ENETUNREACH,     /* network unreachable */
    ENFILE,          /* too many files open in system */
    ENOBUFS,         /* no buffer space available */
    ENODEV,          /* no such device */
    ENOENT,          /* no such file or directory */
    ENOEXEC,         /* executable file format error */
    ENOLCK,          /* no locks available */
    ENOLINK,         /* reserved */
    ENOMEM,          /* not enough space */
    ENOMSG,          /* no message of the desired type */
    ENOPROTOOPT,     /* protocol not available */
    ENOSPC,          /* no space left on device */
    ENOSYS,          /* function not supported */
    ENOTCONN,        /* the socket is not connected */
    ENOTDIR,         /* not a directory or a symbolic link to a directory */
    ENOTEMPTY,       /* directory not empty */
    ENOTRECOVERABLE, /* state not recoverable */
    ENOTSOCK,        /* not a socket */
    ENOTSUP,         /* not supported (maybe the same value as [EOPNOTSUPP]) */
    ENOTTY,          /* inappropriate I/O control operation */
    ENXIO,           /* no such device or address */
    EOPNOTSUPP,      /* operation not supported on socket (maybe the same value as ENOTSUP) */
    EOVERFLOW,       /* value too large to be stored in data type */
    EOWNERDEAD,      /* previous owner died */
    EPERM,           /* operation not permitted */
    EPIPE,           /* broken pipe */
    EPROTO,          /* protocol error */
    EPROTONOSUPPORT, /* protocol not supported */
    EPROTOTYPE,      /* protocol wrong type for socket */
    ERANGE,          /* result too large */
    EROFS,           /* read-only file system */
    ESPIPE,          /* invalid seek */
    ESRCH,           /* no such process */
    ESTALE,          /* reserved */
    ETIMEDOUT,       /* connection timed out */
    ETXTBSY,         /* text file busy */
    EWOULDBLOCK,     /* operation would block (maybe the same value as [EAGAIN]) */
    EXDEV,           /* cross-device link */
    ECUSTOM,         /* custom error, used only by LibTC/Error when only string is provided */
    EMAX             /* maximum error value */
};

/* Here the values are defined too because errno values must be available to the CPP too */

#define ENOERR          ENOERR
#define E2BIG           E2BIG
#define EACCES          EACCES
#define EADDRINUSE      EADDRINUSE
#define EADDRNOTAVAIL   EADDRNOTAVAIL
#define EAFNOSUPPORT    EAFNOSUPPORT
#define EAGAIN          EAGAIN
#define EALREADY        EALREADY
#define EBADF           EBADF
#define EBADMSG         EBADMSG
#define EBUSY           EBUSY
#define ECANCELED       ECANCELED
#define ECHILD          ECHILD
#define ECONNABORTED    ECONNABORTED
#define ECONNREFUSED    ECONNREFUSED
#define ECONNRESET      ECONNRESET
#define EDEADLK         EDEADLK
#define EDESTADDRREQ    EDESTADDRREQ
#define EDOM            EDOM
#define EDQUOT          EDQUOT
#define EEXIST          EEXIST
#define EFAULT          EFAULT
#define EFBIG           EFBIG
#define EHOSTUNREACH    EHOSTUNREACH
#define EIDRM           EIDRM
#define EILSEQ          EILSEQ
#define EINPROGRESS     EINPROGRESS
#define EINTR           EINTR
#define EINVAL          EINVAL
#define EIO             EIO
#define EISCONN         EISCONN
#define EISDIR          EISDIR
#define ELOOP           ELOOP
#define EMFILE          EMFILE
#define EMLINK          EMLINK
#define EMSGSIZE        EMSGSIZE
#define EMULTIHOP       EMULTIHOP
#define ENAMETOOLONG    ENAMETOOLONG
#define ENETDOWN        ENETDOWN
#define ENETRESET       ENETRESET
#define ENETUNREACH     ENETUNREACH
#define ENFILE          ENFILE
#define ENOBUFS         ENOBUFS
#define ENODEV          ENODEV
#define ENOENT          ENOENT
#define ENOEXEC         ENOEXEC
#define ENOLCK          ENOLCK
#define ENOLINK         ENOLINK
#define ENOMEM          ENOMEM
#define ENOMSG          ENOMSG
#define ENOPROTOOPT     ENOPROTOOPT
#define ENOSPC          ENOSPC
#define ENOSYS          ENOSYS
#define ENOTCONN        ENOTCONN
#define ENOTDIR         ENOTDIR
#define ENOTEMPTY       ENOTEMPTY
#define ENOTRECOVERABLE ENOTRECOVERABLE
#define ENOTSOCK        ENOTSOCK
#define ENOTSUP         ENOTSUP
#define ENOTTY          ENOTTY
#define ENXIO           ENXIO
#define EOPNOTSUPP      EOPNOTSUPP
#define EOVERFLOW       EOVERFLOW
#define EOWNERDEAD      EOWNERDEAD
#define EPERM           EPERM
#define EPIPE           EPIPE
#define EPROTO          EPROTO
#define EPROTONOSUPPORT EPROTONOSUPPORT
#define EPROTOTYPE      EPROTOTYPE
#define ERANGE          ERANGE
#define EROFS           EROFS
#define ESPIPE          ESPIPE
#define ESRCH           ESRCH
#define ESTALE          ESTALE
#define ETIMEDOUT       ETIMEDOUT
#define ETXTBSY         ETXTBSY
#define EWOULDBLOCK     EWOULDBLOCK
#define EXDEV           EXDEV
#define ECUSTOM         ECUSTOM
#define EMAX            EMAX

#ifdef errno
#    undef errno
#endif
#define errno *errno_location() /* use the errno location as errno symbol */

int* errno_location();

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
