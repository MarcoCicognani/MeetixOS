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
