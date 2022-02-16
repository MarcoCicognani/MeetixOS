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

#include <Api/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Little endian conversion of 4-byte value <value>
 */
#define BW_PUT_LE_4(array, value)                                                                                      \
    {                                                                                                                  \
        array[0] = (value >> 24) & 0xFF;                                                                               \
        array[1] = (value >> 16) & 0xFF;                                                                               \
        array[2] = (value >> 8) & 0xFF;                                                                                \
        array[3] = value & 0xFF;                                                                                       \
    };

/**
 * @brief Little endian conversion of 4-byte value in array <arr>
 */
#define BW_GET_LE_4(array, value)                                                                                      \
    {                                                                                                                  \
        value = 0;                                                                                                     \
        value |= array[0] << 24;                                                                                       \
        value |= array[1] << 16;                                                                                       \
        value |= array[2] << 8;                                                                                        \
        value |= array[3];                                                                                             \
    };

#ifdef __cplusplus
}
#endif
