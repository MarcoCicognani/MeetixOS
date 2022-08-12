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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------ C defines ----------------------------------------- */

#if defined(__GNUC__) && defined(__BYTE_ORDER__)
#    define __LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
#    define __BIG_ENDIAN    __ORDER_BIG_ENDIAN__
#    define __PDP_ENDIAN    __ORDER_PDP_ENDIAN__
#else
#    define __LITTLE_ENDIAN 0x01020304
#    define __BIG_ENDIAN    0x04030201
#    define __PDP_ENDIAN    0x03040102
#endif

#if defined(__GNUC__) && defined(__BYTE_ORDER__)
#    define __BYTE_ORDER __BYTE_ORDER__
#elif __x86_64__
#    define __BYTE_ORDER __LITTLE_ENDIAN
#elif __i386__
#    define __BYTE_ORDER __LITTLE_ENDIAN
#else
#    error "unable to declare endianness for this platform"
#endif

#define BIG_ENDIAN    __BIG_ENDIAN
#define LITTLE_ENDIAN __LITTLE_ENDIAN
#define PDP_ENDIAN    __PDP_ENDIAN
#define BYTE_ORDER    __BYTE_ORDER

static __inline u16 __bswap16(u16 x) {
    return x << 8 | x >> 8;
}

static __inline u32 __bswap32(u32 x) {
    return x >> 24 | (x >> 8 & 0xff00) | (x << 8 & 0xff0000) | x << 24;
}

static __inline u64 __bswap64(u64 x) {
    return __bswap32(x) + 0ULL << 32 | __bswap32(x >> 32);
}

#if __BYTE_ORDER == __LITTLE_ENDIAN
#    define htobe16(x) __bswap16(x)
#    define be16toh(x) __bswap16(x)
#    define betoh16(x) __bswap16(x)
#    define htobe32(x) __bswap32(x)
#    define be32toh(x) __bswap32(x)
#    define betoh32(x) __bswap32(x)
#    define htobe64(x) __bswap64(x)
#    define be64toh(x) __bswap64(x)
#    define betoh64(x) __bswap64(x)
#    define htole16(x) (u16)(x)
#    define le16toh(x) (u16)(x)
#    define letoh16(x) (u16)(x)
#    define htole32(x) (u32)(x)
#    define le32toh(x) (u32)(x)
#    define letoh32(x) (u32)(x)
#    define htole64(x) (u64)(x)
#    define le64toh(x) (u64)(x)
#    define letoh64(x) (u64)(x)
#else
#    define htobe16(x) (u16)(x)
#    define be16toh(x) (u16)(x)
#    define betoh16(x) (u16)(x)
#    define htobe32(x) (u32)(x)
#    define be32toh(x) (u32)(x)
#    define betoh32(x) (u32)(x)
#    define htobe64(x) (u64)(x)
#    define be64toh(x) (u64)(x)
#    define betoh64(x) (u64)(x)
#    define htole16(x) __bswap16(x)
#    define le16toh(x) __bswap16(x)
#    define letoh16(x) __bswap16(x)
#    define htole32(x) __bswap32(x)
#    define le32toh(x) __bswap32(x)
#    define letoh32(x) __bswap32(x)
#    define htole64(x) __bswap64(x)
#    define le64toh(x) __bswap64(x)
#    define letoh64(x) __bswap64(x)
#endif

#ifdef __cplusplus
}
#endif
