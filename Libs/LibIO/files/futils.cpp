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

#include <eva.h>
#include <io/files/futils.hpp>
#include <stdio.h>

/**
 * read a string from the provided file and store to the out object
 *
 * @param file:		the file to read
 * @param out:		the object where store the readed bytes
 * @return whether the operation success
 */
bool FileUtils::readString(File_t file, std::string& out) {
    // create buffer to stoire readed bytes
    uint8_t           c;
    std::stringstream s;
    while ( Read(file, &c, 1) > 0 ) {
        // stop on null-terminator
        if ( !c ) {
            out = s.str();
            return true;
        }

        s << (char)c;
    }

    // must have a null-terminator
    return false;
}

/**
 * read a len bytes from the provided file and store the content to the buffer
 *
 * @param file:		the file to read
 * @param buffer:	the object where store the readed bytes
 * @param len:		the quantity of bytes to be readed
 * @return whether the operation success
 */
bool FileUtils::readBytes(File_t file, uint8_t* buffer, size_t len) {
    ssize_t remain = len;

    FsReadStatus status;
    while ( remain ) {
        ssize_t read = ReadS(file, &buffer[len - remain], remain, &status);
        if ( status != FS_READ_SUCCESSFUL || read <= 0 )
            return false;
        remain -= read;
    }

    return true;
}

/**
 * read a len bytes from the provided file and store the content to the buffer
 *
 * @param file:		the file to read
 * @param offset:	the offset of the file
 * @param buffer:	the object where store the readed bytes
 * @param len:		the quantity of bytes to be readed
 * @return whether the operation success
 */
bool FileUtils::readBytes(File_t file, size_t offset, uint8_t* buffer, size_t len) {
    // performs a seek before read
    FsSeekStatus s;
    SeekS(file, offset, FS_SEEK_SET, &s);
    if ( s != FS_SEEK_SUCCESSFUL )
        return false;

    return readBytes(file, buffer, len);
}

/**
 * try to read a len bytes from the provided file and store the content to the buffer
 *
 * @param file:		the file to read
 * @param offset:	the offset of the file
 * @param buffer:	the object where store the readed bytes
 * @param len:		the quantity of bytes to be readed
 * @return whether the operation success
 */
bool FileUtils::tryReadBytes(FILE* file, uint32_t offset, uint8_t* buffer, uint32_t len) {
    uint32_t remain = len;
    fseek(file, offset, SEEK_SET);

    while ( remain ) {
        size_t read = fread(&buffer[len - remain], 1, remain, file);
        if ( !read )
            return false;
        remain -= read;
    }

    return true;
}
