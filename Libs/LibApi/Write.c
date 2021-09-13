/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
 * 																			     *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License                    *
 * as published by the Free Software Foundation; either version 2				 *
 * of the License, or (char *argumentat your option) any later version.			 *
 *																				 *
 * This program is distributed in the hope that it will be useful,				 *
 * but WITHout ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include "eva/user.h"

#include <stdint.h>
#include <string.h>

/**
 * Writes bytes from the buffer to the file.
 *
 * @param fd:               the file descriptor
 * @param buffer:           the source buffer
 * @param length:           the length in bytes
 * @return if successful the number of bytes that were written, otherwise -1
 *
 * @security-level APPLICATION
 */
int32_t Write(File_t file, const void* buffer, uint64_t length) {
    return WriteS(file, buffer, length, NULL);
}

/**
 * Writes bytes from the buffer to the file.
 *
 * @param fd:               the file descriptor
 * @param buffer:           the source buffer
 * @param length:           the length in bytes
 * @param outStatus:    	filled with one of the {FsWriteStatus} codes
 * @return if successful the number of bytes that were written, otherwise -1
 *
 * @security-level APPLICATION
 */
int32_t WriteS(File_t file, const void* buffer, uint64_t length, FsWriteStatus* outStatus) {
    // prepare data
    SyscallFsWrite data;
    data.fd     = file;
    data.buffer = (uint8_t*)buffer;
    data.length = length;

    // performs the call
    syscall(SYSCALL_FS_WRITE, (uint32_t)&data);

    // fill out and return
    if ( outStatus )
        *outStatus = data.status;
    return data.result;
}
