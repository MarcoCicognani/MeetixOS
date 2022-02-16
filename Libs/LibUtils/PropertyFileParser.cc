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

#include <fstream>
#include <LibUtils/PropertyFileParser.hh>
#include <LibUtils/Utils.hh>
#include <sstream>

namespace Utils {

PropertyFileParser::PropertyFileParser(std::ifstream& stream) {
    /* read at once the content of the given stream */
    std::stringstream ss;
    ss << stream.rdbuf();

    /* close the stream and initialize the content */
    stream.close();
    init(ss.str());
}

PropertyFileParser::PropertyFileParser(const std::string& content) {
    init(content);
}

std::map<std::string, std::string> PropertyFileParser::properties() {
    std::map<std::string, std::string> properties;

    /* parse the file */
    while ( true ) {
        std::string a, b;
        if ( property_entry(a, b) )
            properties.insert(std::make_pair(a, b));
        else
            break;
    }
    return properties;
}

void PropertyFileParser::init(const std::string& content) {
    m_buffer = content;
    step();
}

bool PropertyFileParser::property_entry(std::string& key, std::string& value) {
    /* read the current key */
    if ( !read_key(key) )
        return false;

    /* skip = or : */
    step();

    /* read the current value */
    if ( !read_value(value) )
        return false;

    return true;
}

bool PropertyFileParser::read_key(std::string& out) {
    out.clear();
    while ( m_char != -1 && m_char != '=' && m_char != ':' ) {
        out += m_char;
        step();
    }

    out = Utils::trim(out);
    return !out.empty();
}

bool PropertyFileParser::read_value(std::string& out) {
    out.clear();
    while ( m_char != -1 && m_char != '\n' ) {
        out += m_char;
        step();
    }

    out = Utils::trim(out);
    if ( out.length() > 1 && out[0] == '"' && out[out.length() - 1] == '"' )
        out = out.substr(1, out.length() - 2);
    return !out.empty();
}

void PropertyFileParser::step() {
    if ( m_buffer_pos >= m_buffer.length() ) {
        m_char = -1;
        return;
    }

    m_char = m_buffer[m_buffer_pos++];
}

} /* namespace Utils */