/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
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

#ifndef EVA_FILESYSTEM_FILEDESCRIPTORS
#define EVA_FILESYSTEM_FILEDESCRIPTORS

#include "eva/fs.h"
#include "filesystem/pipes.hpp"

#include <tasking/process.hpp>
#include <utils/HashMap.hpp>

/**
 *
 */
struct FileDescriptorContent {
    File_t   id;
    int64_t  offset;
    FsVirtID nodeID;
    int32_t  openFlags;

    void cloneInto(FileDescriptorContent* other) {
        other->offset    = offset;
        other->nodeID    = nodeID;
        other->openFlags = openFlags;
    }
};

/**
 *
 */
struct FileDescriptorTable {
    File_t                                  nextFileDescriptor = 3; // skips stdin/stdout/stderr
    HashMap<File_t, FileDescriptorContent*> descriptors;
};

/**
 *
 */
class FileDescriptors {
private:
    static FileDescriptorContent* createDescriptor(FileDescriptorTable* table,
                                                   File_t               overrideFile_t = -1);

public:
    static void initialize();

    static File_t map(Pid pid, FsVirtID nodeID, File_t fd, int32_t openFlags);
    static void   unmap(Pid pid, File_t fd);
    static void   unmapAll(Pid pid);

    static FileDescriptorContent* get(Pid pid, File_t fd);
    static FileDescriptorTable*   getProcessTable(Pid pid);
};

#endif
