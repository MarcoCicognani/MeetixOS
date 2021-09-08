/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *  Ghost, a micro-kernel based operating system for the x86 architecture    *
 *  Copyright (C) 2015, Max Schlüssel <lokoxe@gmail.com>                     *
 *                                                                           *
 *  This program is free software: you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation, either version 3 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "RamdiskWriter.hpp"

#include <algorithm>
#include <dirent.h>
#include <iostream>
#include <list>
#include <sstream>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // create ramdisk object
    RamdiskWriter ramdisk_writer;
    if ( argc == 2 ) {
        if ( strcmp(argv[1], "--help") == 0 ) {
            std::cout << '\n';
            std::cout << "NAME\n";
            std::cout << "\tMeetiX OS Ramdisk Writer, by  Max Schlüssel\n\n";
            std::cout << "DESCRIPTION\n";
            std::cout << "\tGenerates a ramdisk image from a given source folder.\n\n";
            std::cout << "SYNTAX\n";
            std::cout << "\tPath/To/SourceDir Path/To/TargetFileName" << std::endl;
        } else {
            std::cerr << "error: unrecognized command line option '" << argv[1] << std::endl;
        }
        return 1;
    } else if ( argc == 3 ) {
        ramdisk_writer.create(argv[1], argv[2]);
        return 0;
    } else {
        std::cerr << "usage: " << argv[0] << " path/to/source path/to/target" << std::endl;
        return 1;
    }
}

/**
 * trim the provided string
 *
 * @param str:		the string to trim
 * @return:			the trimmed string or the same string
 */
std::string trim(std::string& str) {
    // the string have to valid
    if ( str.length() > 0 ) {
        size_t first = str.find_first_not_of(" \r\n\t");
        size_t last  = str.find_last_not_of(" \r\n\t");
        return str.substr(first, last - first + 1);
    }
    return str;
}

/**
 * create a ramdisk file from a source path
 *
 * @param sourcePath:		the path where the tool take entryes to create
 * the output file
 * @param targetPath:		the path where the tool write the output file
 */
void RamdiskWriter::create(const std::string& sourcePath, const std::string& targetPath) {
    // read .rdignore if it exists
    std::ifstream rdignore(sourcePath + "/.rdignore");
    if ( rdignore.is_open() ) {
        std::string line;
        while ( getline(rdignore, line) )
            if ( line.length() > 0 )
                ignores.push_back(trim(line));
    }

    // try the writing recursive
    try {
        // open the targetPath file
        out.open(targetPath, std::ios::out | std::ios::binary);

        // check open success
        if ( out.good() ) {
            // show the current status
            std::cout << "status: packing folder \"" << sourcePath << "\" to file \"" << targetPath
                      << "\":" << std::endl;

            int64_t pos = out.tellp();
            writeRecursive(sourcePath, sourcePath, "", 0, 0, false);
            int64_t written = out.tellp() - pos;

            std::cout << "status: \"" << targetPath << "\" successfully created, wrote " << written
                      << " bytes" << std::endl;
        }

        // an error was occoured
        else
            std::cerr << "error: could not write to file '" << targetPath << "'" << std::endl;

    } catch ( std::exception& e ) {
        std::cerr << "error: an error occured while creating the ramdisk: " << e.what()
                  << std::endl;
    }

    // close file if is opened
    if ( out.is_open() )
        out.close();
}

/**
 * write each file on a output file
 *
 * @param basePath:		 the base path of a file
 * @param path:			 the absolute path of the file
 * @param name:			 the filename
 * @param contentLength: the content length in bytes of the file
 * @param parentId:		 the id of the parent
 * @param isFile:		 flag to check if current entry is a file or a
 * directory
 */
void RamdiskWriter::writeRecursive(const std::string& basePath,
                                   const std::string& path,
                                   const std::string& name,
                                   uint32_t           contentLength,
                                   uint32_t           parentId,
                                   bool               isFile) {
    // check whether to skip the file
    std::string basePathStr(basePath);
    std::string pathStr(path);
    for ( std::string ign : ignores ) {
        // starts with star?
        if ( !ign.find("*") ) {
            std::string part = ign.substr(1);
            if ( pathStr.find(part) == pathStr.length() - part.length() ) {
                std::cout << "  skipping: " << path << std::endl;
                return;
            }
        }

        // ends with star?
        if ( ign.find("*") == ign.length() - 1 ) {
            std::string part             = ign.substr(0, ign.length() - 1);
            std::string absolutePartPath = basePathStr + "/" + part;

            if ( pathStr.find(absolutePartPath) == 0 ) {
                std::cout << "  skipping: " << path << std::endl;
                return;
            }
        }

        // full paths
        std::string absolutePath = basePathStr + "/" + ign;
        if ( absolutePath == pathStr ) {
            std::cout << "  skipping: " << path << std::endl;
            return;
        }
    }

    uint32_t bufferSize = 0x10000;
    char*    buffer     = new char[bufferSize];
    uint32_t entryId    = idCounter++;

    std::stringstream msg;
    msg << "Packing: " << entryId << ": " << path << (isFile ? "" : "/");
    std::cout << msg.str() << std::endl;

    // Root must not be written
    if ( entryId > 0 ) {
        // file or folder
        buffer[0] = isFile ? 1 : 0;
        out.write(buffer, 1);

        // id
        buffer[0] = ((entryId >> 0) & 0xFF);
        buffer[1] = ((entryId >> 8) & 0xFF);
        buffer[2] = ((entryId >> 16) & 0xFF);
        buffer[3] = ((entryId >> 24) & 0xFF);
        out.write(buffer, 4);

        // parent id
        buffer[0] = ((parentId >> 0) & 0xFF);
        buffer[1] = ((parentId >> 8) & 0xFF);
        buffer[2] = ((parentId >> 16) & 0xFF);
        buffer[3] = ((parentId >> 24) & 0xFF);
        out.write(buffer, 4);

        // name length
        int32_t namelen = name.length();
        buffer[0]       = ((namelen >> 0) & 0xFF);
        buffer[1]       = ((namelen >> 8) & 0xFF);
        buffer[2]       = ((namelen >> 16) & 0xFF);
        buffer[3]       = ((namelen >> 24) & 0xFF);
        out.write(buffer, 4);

        // name
        memcpy(buffer, name.c_str(), namelen);
        out.write(buffer, namelen);
    }

    // write a file
    if ( isFile ) {
        // if (strcmp(basename(path.c_str()), ".directory") != 0)
        {
            // file length
            buffer[0] = ((contentLength >> 0) & 0xFF);
            buffer[1] = ((contentLength >> 8) & 0xFF);
            buffer[2] = ((contentLength >> 16) & 0xFF);
            buffer[3] = ((contentLength >> 24) & 0xFF);
            out.write(buffer, 4);

            // file content
            std::ifstream fileInput;
            fileInput.open(path, std::ios::in | std::ios::binary);

            int32_t length;
            while ( fileInput.good() ) {
                fileInput.read(buffer, bufferSize);
                length = fileInput.gcount();
                out.write(buffer, length);
            }

            fileInput.close();
        }
    } else {
        DIR*    directory;
        dirent* entry;

        if ( (directory = opendir(path.c_str())) != NULL ) {
            while ( (entry = readdir(directory)) != NULL ) {
                char entryPath[260];

                std::stringstream str;
                str << path << '/' << entry->d_name;

                str >> entryPath;

                struct stat s;
                int32_t     statr = stat(entryPath, &s);
                if ( !statr ) {
                    if ( s.st_mode & S_IFREG )
                        writeRecursive(basePath,
                                       entryPath,
                                       entry->d_name,
                                       s.st_size,
                                       entryId,
                                       true);

                    else if ( s.st_mode & S_IFDIR )
                        if ( !(strcmp(entry->d_name, ".") == 0
                               || strcmp(entry->d_name, "..") == 0) )
                            writeRecursive(basePath, entryPath, entry->d_name, 0, entryId, false);
                }

                else {
                    std::cerr << "error: could not read directory: '" << path << "'";
                    break;
                }
            }

            closedir(directory);
        }

        else
            std::cerr << "error: could not open directory: '" << path << "'";
    }

    // entry done
    out.flush();

    delete[] buffer;
}
