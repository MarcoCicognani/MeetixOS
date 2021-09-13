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

#ifndef MEETIX_LIBRARY_IO_FILEUTILS
#define MEETIX_LIBRARY_IO_FILEUTILS

#include <sstream>
#include <stdint.h>

/**
 * static call that provide methods to read files
 */
class FileUtils {
private:
    /**
     * remove constructor
     */
    FileUtils() = delete;

public:
    /**
     * read a string from the provided file and store to the out object
     *
     * @param file:		the file to read
     * @param out:		the object where store the readed bytes
     * @return whether the operation success
     */
    static bool readString(File_t file, std::string& out);

    /**
     * read a len bytes from the provided file and store the content to the buffer
     *
     * @param file:		the file to read
     * @param buffer:	the object where store the readed bytes
     * @param len:		the quantity of bytes to be readed
     * @return whether the operation success
     */
    static bool readBytes(File_t file, uint8_t* buffer, size_t len);

    /**
     * read a len bytes from the provided file and store the content to the buffer
     *
     * @param file:		the file to read
     * @param offset:	the offset of the file
     * @param buffer:	the object where store the readed bytes
     * @param len:		the quantity of bytes to be readed
     * @return whether the operation success
     */
    static bool readBytes(File_t file, size_t offset, uint8_t* buffer, size_t len);

    /**
     * try to read a len bytes from the provided file and store the content to the buffer
     *
     * @param file:		the file to read
     * @param offset:	the offset of the file
     * @param buffer:	the object where store the readed bytes
     * @param len:		the quantity of bytes to be readed
     * @return whether the operation success
     */
    static bool tryReadBytes(FILE* file, uint32_t offset, uint8_t* buffer, uint32_t len);
};

#endif
