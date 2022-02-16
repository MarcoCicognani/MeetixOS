/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <map>
#include <string>
#include <vector>

namespace Utils {

/* ------------------------------------------ C++ types ----------------------------------------- */

/**
 * @brief Parser for ini-like files
 */
class PropertyFileParser {
public:
    /**
     * @brief Constructors
     */
    PropertyFileParser(std::ifstream& stream);
    PropertyFileParser(const std::string& content);

    /**
     * @brief Returns the map of the parsed properties
     */
    std::map<std::string, std::string> properties();

private:
    void init(const std::string& content);
    bool property_entry(std::string& key, std::string& value);
    bool read_key(std::string& out);
    bool read_value(std::string& out);
    void step();

private:
    char        m_char{ -1 };
    usize       m_buffer_pos{ 0 };
    std::string m_buffer{};
};

} /* namespace Utils */