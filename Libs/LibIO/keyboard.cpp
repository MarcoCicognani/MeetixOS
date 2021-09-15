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
#include <fstream>
#include <io/files/futils.hpp>
#include <io/keyboard.hpp>
#include <io/ps2.hpp>
#include <map>
#include <stdio.h>
#include <string>
#include <utils/fparser.hpp>
#include <utils/utils.hpp>

Keyboard& Keyboard::instance() {
    static Keyboard* s_instance = nullptr;
    if ( !s_instance )
        s_instance = new Keyboard{};

    return *s_instance;
}

/**
 * load the scancode file of the layout path provided
 *
 * @param iso:		the path to the iso file
 * @return whether the operation success
 */
bool Keyboard::loadScancodeLayout(const std::string& iso) {
    // open the file
    std::ifstream in("/MeetiX/Configs/WM/KbLayouts/" + iso + ".layout");
    if ( !in.is_open() )
        return false;

    // get the property of the file
    std::map<std::string, std::string> properties;
    {
        m_scancode_layout.clear();
        PropertyFileParser props(in);
        properties = props.getProperties();
    }

    // register each property
    for ( std::pair<std::string, std::string> entry : properties ) {
        auto scancode  = 0;
        auto space_pos = entry.first.find(" ");
        if ( space_pos != std::string::npos ) {
            std::string part1 = entry.first.substr(0, space_pos);
            std::string part2 = entry.first.substr(space_pos + 1);

            uint32_t          part1val;
            std::stringstream conv1;
            conv1 << std::hex << part1;
            conv1 >> part1val;

            uint32_t          part2val;
            std::stringstream conv2;
            conv2 << std::hex << part2;
            conv2 >> part2val;

            scancode = (part1val << 8) | part2val;
        } else {
            std::stringstream conv;
            conv << std::hex << entry.first;
            conv >> scancode;
        }

        if ( entry.second.empty() ) {
            std::stringstream msg;
            msg << "could not map scancode " << (uint32_t)scancode << ", key name '" << entry.second
                << "' is not known";
            Utils::log(msg.str());
        } else {
            m_scancode_layout[scancode] = entry.second;
        }
    }

    return true;
}

/**
 * load the conversion file of the layout path provided
 *
 * @param iso:		the path to the iso file
 * @return whether the operation success
 */
bool Keyboard::loadConversionLayout(const std::string& iso) {
    // open the file
    std::ifstream in("/MeetiX/Configs/WM/KbLayouts/" + iso + ".conversion");
    if ( !in.good() )
        return false;

    // get the properties of the file
    std::map<std::string, std::string> properties;
    {
        m_conversion_layout.clear();
        PropertyFileParser props(in);
        properties = props.getProperties();
    }

    // register each property
    for ( std::pair<std::string, std::string> entry : properties ) {
        // create key info value
        Info info;

        // they shall be triggered on press
        info.pressed = true;

        // take the key and split if necessary
        std::string keyName  = entry.first;
        int32_t     spacePos = keyName.find(' ');

        if ( spacePos != std::string::npos ) {
            std::string flags = keyName.substr(spacePos + 1);
            keyName           = keyName.substr(0, spacePos);

            // Handle the flags
            for ( int i = 0; i < flags.length(); i++ ) {
                if ( flags[i] == 's' )
                    info.shift = true;
                else if ( flags[i] == 'c' )
                    info.ctrl = true;
                else if ( flags[i] == 'a' )
                    info.alt = true;
                else {
                    std::stringstream msg;
                    msg << "unknown flag in conversion mapping: " << flags[i];
                    Utils::log(msg.str());
                }
            }
        }

        // Set key
        info.key = keyName;

        // Push the mapping
        auto c     = -1;
        auto value = entry.second;
        if ( value.length() > 0 ) {
            c = value[0];

            // Escaped numeric values
            if ( c == '\\' ) {
                if ( value.length() > 1 ) {
                    std::stringstream conv;
                    conv << value.substr(1);
                    uint32_t num;
                    conv >> num;
                    c = num;
                } else {
                    Utils::log("skipping value '" + value + "' in key " + keyName
                               + ", illegal format");
                    continue;
                }
            }
        }
        m_conversion_layout[info] = static_cast<char>(c);
    }

    return true;
}

/**
 * read a key from shared ps2 driver area
 * and return an info instance with the code of the readed key
 *
 * @param breakCondition:	an additional flag to sincronize the reading
 * @return an info instance with the key code readed
 */
Keyboard::Info Keyboard::read(bool* breakCondition) {
    // if not registered do now
    if ( !ps2IsRegistered )
        if ( !Ps2::registerSelf() )
            return Info();

    // wait until incoming data is here (and the driver unsets the atom)
    AtomicBlockDual(&ps2Area->keyboard.atomNothingQueued, (uint8_t*)breakCondition);

    // take info from the shared memory
    uint8_t scancode = ps2Area->keyboard.scancode;

    // tell driver that we've handled it
    ps2Area->keyboard.atomNothingQueued = true;
    ps2Area->keyboard.atomUnhandled     = false;

    // read and convert data
    Info info;
    if ( keyForScancode(scancode, &info) )
        return info;
    return Info();
}

/**
 * convert a scancode key to a keyboard key name
 *
 * @param scancode:		the scancode to convert
 * @param out:			the info to fill to
 * @return whether the conversion success
 */
bool Keyboard::keyForScancode(uint8_t scancode, Info* out) {
    // Get "pressed" info from scancode
    out->pressed  = !(scancode & (1 << 7));
    out->scancode = scancode & ~(1 << 7); // remove 7th bit

    // Get key from layout map
    bool foundCompound = false;
    if ( m_have_last_unknown_key ) {
        int compoundScancode = m_last_unknown_key.scancode << 8 | out->scancode;

        // Try to find a compound key
        auto pos = m_scancode_layout.find(compoundScancode);
        if ( pos != m_scancode_layout.end() ) {
            out->key                = pos->second;
            foundCompound           = true;
            m_have_last_unknown_key = false;
        }
    }

    // When it is no compound
    if ( !foundCompound ) {
        // Try to find the normal key
        auto pos = m_scancode_layout.find(out->scancode);
        if ( pos == m_scancode_layout.end() ) {
            // If it's not found, this might be the start of a compound
            m_have_last_unknown_key = true;
            m_last_unknown_key      = *out;
            return false;
        }

        else
            out->key = pos->second;
    }

    // Handle special keys
    if ( out->key == "KEY_CTRL_L" || out->key == "KEY_CTRL_R" )
        m_status_ctrl = out->pressed;
    else if ( out->key == "KEY_SHIFT_L" || out->key == "KEY_SHIFT_R" )
        m_status_shift = out->pressed;
    else if ( out->key == "KEY_ALT_L" || out->key == "KEY_ALT_R" )
        m_status_alt = out->pressed;

    // Set control key info
    out->ctrl  = m_status_ctrl;
    out->shift = m_status_shift;
    out->alt   = m_status_alt;

    return true;
}

/**
 * convert the provided keyboard info to the corresponsing character
 *
 * @param info:		the keyboard key name
 * @return the converted character
 */
char Keyboard::charForKey(const Info& info) {
    auto pos = m_conversion_layout.find(info);
    if ( pos != m_conversion_layout.end() )
        return pos->second;

    return -1;
}

/**
 * creates a full keyboard info from a basic info
 *
 * @param basic:	the basic info to convert
 * @return the full key info
 */
Keyboard::Info Keyboard::fullKeyInfo(const InfoBasic& basic) {
    // create a full scancode object
    Info info(basic);

    // find the key code
    auto pos = m_scancode_layout.find(basic.scancode);
    if ( pos != m_scancode_layout.end() )
        info.key = pos->second;
    return info;
}

/**
 * @return the name of the current layout
 */
std::string Keyboard::getCurrentLayout() {
    return m_current_layout;
}

/**
 * load a new layout from filepath
 *
 * @param iso:		the path to the iso file
 * @return whether the operation success
 */
bool Keyboard::loadLayout(const std::string& iso) {
    if ( loadScancodeLayout(iso) && loadConversionLayout(iso) ) {
        m_current_layout = iso;
        return true;
    }
    return false;
}
