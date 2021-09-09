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
#include <cstring>
#include <filesystem>
#include <iostream>

/**
 * @brief Creates a custom format ramdisk with the MeetiX kernel is able to read and write
 */
int main(int argc, char** argv) {
    if ( argc == 2 ) {
        if ( strcmp(argv[1], "--help") == 0 ) {
            std::cout << "Ramdisk Writer Tool\n\n";
            std::cout << "DESCRIPTION\n";
            std::cout << "\tGenerates a ramdisk image from a given source folder.\n\n";
            std::cout << "SYNTAX\n";
            std::cout << "\tRamdiskWriter Path/To/SourceDir Path/To/TargetFileName" << std::endl;
        } else {
            std::cerr << "error: unrecognized command line option '" << argv[1] << std::endl;
        }
        return 1;
    } else if ( argc == 3 ) {
        RamdiskWriter ramdisk_writer;
        ramdisk_writer.create(argv[1], argv[2]);
        return 0;
    } else {
        std::cerr << "usage: " << argv[0] << " path/to/source path/to/target" << std::endl;
        return 1;
    }
}

std::string trim(std::string& str) {
    if ( str.length() > 0 ) {
        auto first = str.find_first_not_of(" \r\n\t");
        auto last  = str.find_last_not_of(" \r\n\t");

        return str.substr(first, last - first + 1);
    }
    return str;
}

void RamdiskWriter::create(const std::string& source_path, const std::string& target_path) {
    /* check whether the directory contains a .read_ignore, then safe the content */
    auto read_ignore_path   = source_path + "/.read_ignore";
    auto read_ignore_stream = std::ifstream{ read_ignore_path };

    if ( read_ignore_stream.is_open() ) {
        /* put into m_ignores the path to the file itself */
        m_ignores.push_back(read_ignore_path);

        /* collect all the other ignores */
        std::string line;
        while ( getline(read_ignore_stream, line) )
            if ( line.length() > 0 ) {
                m_ignores.push_back(trim(line));
            }
    }

    /* try the writing recursive */
    try {
        m_out_file.open(target_path, std::ios::out | std::ios::binary | std::ios::trunc);

        if ( m_out_file.is_open() ) {
            std::cout << "Packing " << source_path << "/ to " << target_path << std::endl;

            /* write this directory recursively */
            auto cursor_pos = m_out_file.tellp();
            write_recursive(source_path, source_path, "", 0, 0, false);
            auto written_bytes = m_out_file.tellp() - cursor_pos;

            std::cout << target_path << " successfully created, written ";
            if ( written_bytes >= 1024 * 1024 ) {
                std::cout << written_bytes / 1024 / 1024 << "MiB";
            } else if ( written_bytes >= 1024 ) {
                std::cout << written_bytes / 1024 << "KiB";
            } else {
                std::cout << written_bytes << "Bytes";
            }
            std::cout << std::endl;
        } else {
            std::cerr << "Error: Unable to write to '" << target_path << "'" << std::endl;
        }
    } catch ( std::exception& e ) {
        std::cerr << "Error: Unable to write ramdisk image: Cause: " << e.what() << std::endl;
    }

    /* cleanup */
    if ( m_out_file.is_open() )
        m_out_file.close();
}

void RamdiskWriter::write_recursive(const std::string& base_path,
                                    const std::string& path,
                                    const std::string& name,
                                    uint32_t           content_length,
                                    uint32_t           parent_id,
                                    bool               is_file) {
    /* skip files to ignore */
    for ( const auto& ignore : m_ignores ) {
        /* check for pre-pended globbing */
        if ( !ignore.find('*') ) {
            auto part     = ignore.substr(1);
            auto find_res = path.find(part);

            if ( find_res != std::string::npos && find_res == path.length() - part.length() ) {
                std::cout << "  Skipping: " << path << std::endl;
                return;
            }
        }

        /* check for appended globbing */
        if ( ignore.find('*') == ignore.length() - 1 ) {
            auto part          = ignore.substr(0, ignore.length() - 1);
            auto abs_path_part = base_path + "/" + part;

            if ( !path.find(abs_path_part) ) {
                std::cout << "  Skipping: " << path << std::endl;
                return;
            }
        }

        /* full path ignore */
        auto abs_path = base_path + "/" + ignore;
        if ( abs_path == path ) {
            std::cout << "  Skipping: " << path << std::endl;
            return;
        }
    }

    const auto BUFFER_SIZE = 0x10000;
    auto       buffer_ptr  = new char[BUFFER_SIZE];
    auto       entry_id    = m_next_id++;

    std::cout << "Packing: " << (is_file ? "File" : "Dir") << ": '" << path
              << "' with ID: " << entry_id << std::endl;

    /* skip root entry */
    if ( entry_id > 0 ) {
        /* write entry-type 0 -> directory, 1 -> file */
        buffer_ptr[0] = is_file ? 1 : 0;
        m_out_file.write(buffer_ptr, 1);

        /* entry id */
        buffer_ptr[0] = static_cast<char>((entry_id >> 0) & 0xFF);
        buffer_ptr[1] = static_cast<char>((entry_id >> 8) & 0xFF);
        buffer_ptr[2] = static_cast<char>((entry_id >> 16) & 0xFF);
        buffer_ptr[3] = static_cast<char>((entry_id >> 24) & 0xFF);
        m_out_file.write(buffer_ptr, 4);

        /* parent id */
        buffer_ptr[0] = static_cast<char>((parent_id >> 0) & 0xFF);
        buffer_ptr[1] = static_cast<char>((parent_id >> 8) & 0xFF);
        buffer_ptr[2] = static_cast<char>((parent_id >> 16) & 0xFF);
        buffer_ptr[3] = static_cast<char>((parent_id >> 24) & 0xFF);
        m_out_file.write(buffer_ptr, 4);

        /* name len */
        auto name_len = name.length();
        buffer_ptr[0] = static_cast<char>((name_len >> 0) & 0xFF);
        buffer_ptr[1] = static_cast<char>((name_len >> 8) & 0xFF);
        buffer_ptr[2] = static_cast<char>((name_len >> 16) & 0xFF);
        buffer_ptr[3] = static_cast<char>((name_len >> 24) & 0xFF);
        m_out_file.write(buffer_ptr, 4);

        /* write the name */
        m_out_file.write(name.c_str(), static_cast<std::streamsize>(name_len));
    }

    /* write the file */
    if ( is_file ) {
        /* write the file len */
        buffer_ptr[0] = static_cast<char>((content_length >> 0) & 0xFF);
        buffer_ptr[1] = static_cast<char>((content_length >> 8) & 0xFF);
        buffer_ptr[2] = static_cast<char>((content_length >> 16) & 0xFF);
        buffer_ptr[3] = static_cast<char>((content_length >> 24) & 0xFF);
        m_out_file.write(buffer_ptr, 4);

        /* open the file-content */
        std::ifstream input_file;
        input_file.open(path, std::ios::in | std::ios::binary);

        /* read & write-out the file-content */
        while ( input_file.good() ) {
            input_file.read(buffer_ptr, BUFFER_SIZE);
            m_out_file.write(buffer_ptr, input_file.gcount());
        }

        /* close the input file */
        input_file.close();
    } else {
        /* recursively iterate the directory content */
        for ( auto& dir_entry : std::filesystem::directory_iterator{ path } ) {
            if ( dir_entry.is_regular_file() ) {
                write_recursive(base_path,
                                dir_entry.path().string(),
                                dir_entry.path().filename().string(),
                                dir_entry.file_size(),
                                entry_id,
                                true);
            } else if ( dir_entry.is_directory() ) {
                if ( dir_entry.path().filename().string() != "."
                     && dir_entry.path().filename().string() != ".." ) {
                    write_recursive(base_path,
                                    dir_entry.path().string(),
                                    dir_entry.path().filename().string(),
                                    0,
                                    entry_id,
                                    false);
                }
            }
        }
    }

    /* cleanup */
    m_out_file.flush();
    delete[] buffer_ptr;
}
