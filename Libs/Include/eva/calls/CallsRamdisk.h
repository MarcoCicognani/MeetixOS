/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                       *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
 *         This software is derived from the Ghost operating system project,           *
 *         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
 *         https://ghostkernel.org/                                                    *
 *         https://github.com/maxdev1/ghost                                            *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or                       *
 * modify it under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2                      *
 * of the License, or (char *argumentat your option) any later version.                *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful,                     *
 * but WITHout ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 * GNU General Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License                   *
 * along with this program; if not, write to the Free Software                         *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#ifndef EVA_API_CALLS_RAMDISKCALLS
#define EVA_API_CALLS_RAMDISKCALLS

#include <eva/ramdisk.h>

__BEGIN_C

/**
 * @field path
 * 		the absolute path of the file to find
 *
 * @field nodeID
 * 		the resulting node id. if the file is
 * 		not found, this field is -1.
 *
 * @security-level APPLICATION
 */
typedef struct {
    char* path;

    int32_t nodeID;
} __attribute__((packed)) SyscallRamdiskFind;

/**
 * @field childName
 * 		the relative path of the file to find
 *
 * @field parentId
 * 		the id of the parent folder
 *
 * @field nodeID
 * 		the resulting node id. if the file does
 * 		not exist, this field is -1.
 */
typedef struct {
    int32_t parentID;
    char*   childName;

    int32_t nodeID;
} __attribute__((packed)) SyscallRamdiskFindChild;

/**
 * @field nodeID
 * 		the id of the file to read from
 *
 * @field offset
 * 		the offset within the file
 *
 * @field length
 * 		the maximum number of bytes to write to the buffer
 *
 * @field buffer
 * 		the target buffer
 *
 * @field readBytes
 * 		the resulting number of bytes that were actually read
 */
typedef struct {
    int32_t  nodeID;
    uint32_t offset;
    uint32_t length;
    char*    buffer;

    int32_t readBytes;
} __attribute__((packed)) SyscallRamdiskRead;

/**
 * @field nodeID
 * 		the id of the file to get information of
 *
 * @field type
 * 		the file type, or UNKNOWN/-1 if not existent
 *
 * @field length
 * 		the files length
 *
 * @field name
 * 		the name of the file
 */
typedef struct {
    int32_t nodeID;

    RamdiskEntryType type;
    uint32_t         length;
    char             name[RAMDISK_MAXIMUM_PATH_LENGTH];
} __attribute__((packed)) SyscallRamdiskInfo;

/**
 * @field nodeID
 * 		the id of the folder
 *
 * @field count
 * 		the resulting number of children
 */
typedef struct {
    int32_t nodeID;

    uint32_t count;
} __attribute__((packed)) SyscallRamdiskChildCount;

/**
 * @field nodeID
 * 		the id of the folder
 *
 * @field index
 * 		the index of the child entry
 *
 * @field childId
 * 		the resulting child id
 */
typedef struct {
    int32_t  nodeID;
    uint32_t index;

    int32_t childID;
} __attribute__((packed)) SyscallRamdiskChildAt;

__END_C

#endif
