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

#include <Api.h>
#include <fstream>
#include <LibIO/Input.hh>
#include <LibIO/Keyboard.hh>
#include <LibUtils/PropertyFileParser.hh>
#include <LibUtils/Utils.hh>
#include <map>
#include <string>

namespace IO {

Keyboard& Keyboard::instance() {
    static Keyboard* s_instance = nullptr;
    if ( !s_instance )
        s_instance = new Keyboard{};

    return *s_instance;
}

Keyboard::Info Keyboard::read(bool* breakCondition) {
    /* register to the Input driver */
    if ( !Input::instance().is_registered() ) {
        if ( !Input::instance().register_self() )
            return {};
    }

    auto& ps2_shared_area = Input::instance().shared_area();

    /* wait until incoming data is here (and the driver unsets the atom) */
    s_atomic_block_dual(&ps2_shared_area.m_keyboard.m_atom_nothing_queued, breakCondition);

    /* take info from the shared memory */
    auto scancode = ps2_shared_area.m_keyboard.m_scancode;

    ps2_shared_area.m_keyboard.m_atom_nothing_queued = true;
    ps2_shared_area.m_keyboard.m_atom_unhandled      = false;

    /* read and convert data */
    Info info;
    if ( key_for_scancode(scancode, &info) )
        return info;
    else
        return {};
}

bool Keyboard::key_for_scancode(u8 scancode, Info* out) {
    /* get "pressed" info from scancode */
    out->m_is_pressed = !(scancode & (1 << 7));
    out->m_scancode   = scancode & ~(1 << 7); /* remove 7th bit */

    /* get key from layout map */
    bool found_compound = false;
    if ( m_have_last_unknown_key ) {
        auto compound_scancode = m_last_unknown_key.m_scancode << 8 | out->m_scancode;

        /* try to find a compound key */
        auto pos = m_scancode_layout.find(compound_scancode);
        if ( pos != m_scancode_layout.end() ) {
            out->m_key              = pos->second;
            found_compound          = true;
            m_have_last_unknown_key = false;
        }
    }

    /* when it is no compound */
    if ( !found_compound ) {
        /* try to find the normal key */
        auto pos = m_scancode_layout.find(out->m_scancode);
        if ( pos == m_scancode_layout.end() ) {
            /* if it's not found, this might be the start of a compound */
            m_have_last_unknown_key = true;
            m_last_unknown_key      = *out;
            return false;
        } else
            out->m_key = pos->second;
    }

    /* handle special keys */
    if ( out->m_key == "KEY_CTRL_L" || out->m_key == "KEY_CTRL_R" )
        m_status_ctrl = out->m_is_pressed;
    else if ( out->m_key == "KEY_SHIFT_L" || out->m_key == "KEY_SHIFT_R" )
        m_status_shift = out->m_is_pressed;
    else if ( out->m_key == "KEY_ALT_L" || out->m_key == "KEY_ALT_R" )
        m_status_alt = out->m_is_pressed;

    /* set control key info */
    out->m_ctrl  = m_status_ctrl;
    out->m_shift = m_status_shift;
    out->m_alt   = m_status_alt;
    return true;
}

char Keyboard::char_for_key(const Info& info) {
    auto pos = m_conversion_layout.find(info);
    if ( pos != m_conversion_layout.end() )
        return pos->second;
    else
        return -1;
}

Keyboard::Info Keyboard::full_key_info(const InfoBasic& basic) {
    Info info{ basic };
    auto pos = m_scancode_layout.find(basic.m_scancode);
    if ( pos != m_scancode_layout.end() )
        info.m_key = pos->second;

    return info;
}

bool Keyboard::load_layout(const std::string& iso) {
    if ( load_scancode_layout(iso) && load_conversion_layout(iso) ) {
        m_current_layout = iso;
        return true;
    }
    return false;
}

bool Keyboard::load_scancode_layout(const std::string& iso) {
    /* open the scancode layout file */
    std::ifstream in("/MeetiX/Configs/WM/KbLayouts/" + iso + ".layout");
    if ( !in.is_open() )
        return false;

    /* read all */
    m_scancode_layout.clear();
    auto properties = read_properties(in);

    /* register each property */
    for ( auto& entry : properties ) {
        auto scancode  = u32{ 0 };
        auto space_pos = entry.first.find(" ");
        if ( space_pos != std::string::npos ) {
            auto key   = entry.first.substr(0, space_pos);
            auto value = entry.first.substr(space_pos + 1);

            u32               key_value;
            std::stringstream ss;
            ss << std::hex << key;
            ss >> key_value;

            u32               value_value;
            std::stringstream ss2;
            ss2 << std::hex << value;
            ss2 >> value_value;

            scancode = (key_value << 8) | value_value;
        } else {
            std::stringstream ss;
            ss << std::hex << entry.first;
            ss >> scancode;
        }

        if ( entry.second.empty() )
            Utils::log("Could not map scancode %i, key name '%s' is not known", scancode, entry.second.c_str());
        else
            m_scancode_layout[scancode] = entry.second;
    }

    return true;
}

bool Keyboard::load_conversion_layout(const std::string& iso) {
    /* open the scancode conversion file */
    std::ifstream in("/MeetiX/Configs/WM/KbLayouts/" + iso + ".conversion");
    if ( !in.good() )
        return false;

    /* read all */
    m_conversion_layout.clear();
    auto properties = read_properties(in);

    /* register each property */
    for ( auto& entry : properties ) {
        /* create key info value */
        Info info;
        info.m_is_pressed = true;

        // take the key and split if necessary
        auto key_name  = entry.first;
        auto space_pos = key_name.find(' ');

        if ( space_pos != std::string::npos ) {
            auto flags = key_name.substr(space_pos + 1);
            key_name   = key_name.substr(0, space_pos);

            /* handle the flags */
            for ( char flag : flags ) {
                if ( flag == 's' )
                    info.m_shift = true;
                else if ( flag == 'c' )
                    info.m_ctrl = true;
                else if ( flag == 'a' )
                    info.m_alt = true;
                else
                    Utils::log("Unknown flag in conversion mapping: %c", flag);
            }
        }

        /* set key name */
        info.m_key = key_name;

        /* push the mapping */
        auto c     = char{ -1 };
        auto value = entry.second;
        if ( value.length() > 0 ) {
            c = value[0];

            /* escaped numeric values */
            if ( c == '\\' ) {
                if ( value.length() > 1 ) {
                    u32               num;
                    std::stringstream ss;
                    ss << value.substr(1);
                    ss >> num;

                    c = static_cast<char>(num);
                } else {
                    Utils::log("skipping value '%s' in key %s, illegal format", value.c_str(), key_name.c_str());
                    continue;
                }
            }
        }
        m_conversion_layout[info] = static_cast<char>(c);
    }

    return true;
}
std::map<std::string, std::string> Keyboard::read_properties(std::ifstream& stream) {
    Utils::PropertyFileParser props{ stream };
    return props.properties();
}

} /* namespace IO */