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

#ifndef __MEETIX_LIBC_DIRENT__
#define __MEETIX_LIBC_DIRENT__

#include <dir.h>
#include <Api/Common.h>

__BEGIN_C

/**
 * Directory entry types
 */
#define DT_UNKNOWN 0
#define DT_REG     1
#define DT_DIR     2
#define DT_FIFO    3
#define DT_SOCK    4
#define DT_CHR     5
#define DT_BLK     6
#define DT_LNK     7

// typedef direct struct as type
typedef struct dirent dirent;

/**
 * Represents a directory entry
 */
struct dirent {
    ino_t         d_fileno;             // inode of the entry
    size_t        d_reclen;             // size of the entry
    size_t        d_namlen;             // name length of the entry
    dev_t         d_dev;                // device identifier
    unsigned char d_type;               // node type (file/directory)
    char          d_name[FILENAME_MAX]; // entry name
};

/**
 * close a directory from directory entry iterator
 *
 * @param dir:		the directory iterator pointer
 * @return 0 if success, 1 otherwise
 */
int closedir(DIR* dir);

/**
 * open a directory from path
 *
 * @param path:		the string path of the directory to be opened
 * @return the directory iterator instance if success, NULL otherwise
 */
DIR* opendir(const char* path);

/**
 * read the next dir entry from directory iterator
 *
 * @param dir:		the directory iterator
 * @return the dirent buffer if success, NULL otherwise
 */
struct dirent* readdir(DIR* dir);

/**
 * reset the directory iterator
 *
 * @param dir:		the iterator to be resetted
 */
void rewinddir(DIR* dir);

__END_C

#endif
