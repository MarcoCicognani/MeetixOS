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

#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

/**
 * @brief Creates a custom format ramdisk with the MeetiX kernel is able to read and write
 */
class RamdiskWriter {
public:
    RamdiskWriter() = default;

    /**
     * @brief Recursively creates the ramdisk image packing the files and the directories found into
     * source_path into a file referenced by target_path
     * @param source_path The source directory to pack
     * @param target_path The target file where the ramdisk is generated
     */
    void create(const std::string& source_path, const std::string& target_path);

private:
    void write_recursive(const std::string& base_path,
                         const std::string& path,
                         const std::string& name,
                         uint32_t           content_length,
                         uint32_t           parent_id,
                         bool               is_file);

private:
    int                      m_next_id{ 0 };
    std::ofstream            m_out_file{};
    std::vector<std::string> m_ignores{ std::string{ "*.keep" } };
};
