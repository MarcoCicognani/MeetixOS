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

/**
 * global status for special keys
 */
static bool statusCtrl  = false;
static bool statusShift = false;
static bool statusAlt   = false;

/**
 * map to contains scancodes and conversions of the loaded layoutes
 */
static std::map<uint32_t, std::string> scancodeLayout;
static std::map<Keyboard::Info, char>  conversionLayout;

/**
 * last pressed key
 */
static std::string currentLayout;

/**
 * flag and relative descriptor of last unknow pressed key
 */
static bool           haveLastUnknownKey = false;
static Keyboard::Info lastUnknownKey;

/**
 * load the scancode file of the layout path provided
 *
 * @param iso:		the path to the iso file
 * @return whether the operation success
 */
bool Keyboard::loadScancodeLayout(const std::string& iso) {
    // open the file
    std::ifstream in("/sys/lib/kb/" + iso + ".layout");
    if ( !in.is_open() )
        return false;

    // get the property of the file
    std::map<std::string, std::string> properties;
    {
        scancodeLayout.clear();
        PropertyFileParser props(in);
        properties = props.getProperties();
    }

    // register each property
    for ( std::pair<std::string, std::string> entry : properties ) {
        uint32_t scancode = 0;
        int32_t  spacepos = entry.first.find(" ");
        if ( spacepos != std::string::npos ) {
            std::string part1 = entry.first.substr(0, spacepos);
            std::string part2 = entry.first.substr(spacepos + 1);

            uint32_t          part1val;
            std::stringstream conv1;
            conv1 << std::hex << part1;
            conv1 >> part1val;

            uint32_t          part2val;
            std::stringstream conv2;
            conv2 << std::hex << part2;
            conv2 >> part2val;

            scancode = (part1val << 8) | part2val;
        }

        else {
            std::stringstream conv;
            conv << std::hex << entry.first;
            conv >> scancode;
        }

        if ( entry.second.empty() ) {
            std::stringstream msg;
            msg << "could not map scancode " << (uint32_t)scancode << ", key name '" << entry.second
                << "' is not known";
            Utils::log(msg.str());
        }

        else
            scancodeLayout[scancode] = entry.second;
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
    std::ifstream in("/sys/lib/kb/" + iso + ".conversion");
    if ( !in.good() )
        return false;

    // get the properties of the file
    std::map<std::string, std::string> properties;
    {
        conversionLayout.clear();
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
        char        c     = -1;
        std::string value = entry.second;
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
                }

                else {
                    Utils::log("skipping value '" + value + "' in key " + keyName
                               + ", illegal format");
                    continue;
                }
            }
        }
        conversionLayout[info] = c;
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
    if ( haveLastUnknownKey ) {
        int compoundScancode = lastUnknownKey.scancode << 8 | out->scancode;

        // Try to find a compound key
        std::map<uint32_t, std::string>::iterator pos = scancodeLayout.find(compoundScancode);
        if ( pos != scancodeLayout.end() ) {
            out->key           = pos->second;
            foundCompound      = true;
            haveLastUnknownKey = false;
        }
    }

    // When it is no compound
    if ( !foundCompound ) {
        // Try to find the normal key
        std::map<uint32_t, std::string>::iterator pos = scancodeLayout.find(out->scancode);
        if ( pos == scancodeLayout.end() ) {
            // If it's not found, this might be the start of a compound
            haveLastUnknownKey = true;
            lastUnknownKey     = *out;
            return false;
        }

        else
            out->key = pos->second;
    }

    // Handle special keys
    if ( out->key == "KEY_CTRL_L" || out->key == "KEY_CTRL_R" )
        statusCtrl = out->pressed;
    else if ( out->key == "KEY_SHIFT_L" || out->key == "KEY_SHIFT_R" )
        statusShift = out->pressed;
    else if ( out->key == "KEY_ALT_L" || out->key == "KEY_ALT_R" )
        statusAlt = out->pressed;

    // Set control key info
    out->ctrl  = statusCtrl;
    out->shift = statusShift;
    out->alt   = statusAlt;

    return true;
}

/**
 * convert the provided keyboard info to the corresponsing character
 *
 * @param info:		the keyboard key name
 * @return the converted character
 */
char Keyboard::charForKey(const Info& info) {
    std::map<Info, char>::iterator pos = conversionLayout.find(info);
    if ( pos != conversionLayout.end() )
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
    std::map<uint32_t, std::string>::iterator pos = scancodeLayout.find(basic.scancode);
    if ( pos != scancodeLayout.end() )
        info.key = pos->second;
    return info;
}

/**
 * @return the name of the current layout
 */
std::string Keyboard::getCurrentLayout() {
    return currentLayout;
}

/**
 * load a new layout from filepath
 *
 * @param iso:		the path to the iso file
 * @return whether the operation success
 */
bool Keyboard::loadLayout(const std::string& iso) {
    if ( loadScancodeLayout(iso) && loadConversionLayout(iso) ) {
        currentLayout = iso;
        return true;
    }
    return false;
}
