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

#ifndef MEETIX_LIBRARY_IO_KEYBOARD
#define MEETIX_LIBRARY_IO_KEYBOARD

#include <map>
#include <sstream>
#include <stdint.h>
#include <string>

/**
 * static class to manage keyboard
 */
class Keyboard {
public:
    /**
     * basic keyboard infos,
     * created with data readed on shared memory area
     */
    struct InfoBasic {
    public:
        /**
         * empty constructor
         */
        InfoBasic() : pressed(false), ctrl(false), alt(false), shift(false), scancode(0) {
        }

        /**
         * filled constructor
         *
         * @param pressed:		value to assign to pressed flag
         * @param ctrl:			value to assign to ctrl flag
         * @param alt:			value to assign to alt flag
         * @param shift:		value to assign to shift flag
         * @param scancode:		value to assign to scancode flag
         */
        InfoBasic(bool pressed, bool ctrl, bool alt, bool shift, uint8_t scancode)
            : pressed(pressed), ctrl(ctrl), alt(alt), shift(shift), scancode(scancode) {
        }

        /**
         * copy constructor
         */
        InfoBasic(const InfoBasic& bcp)
            : pressed(bcp.pressed), ctrl(bcp.ctrl), alt(bcp.alt), shift(bcp.shift),
              scancode(bcp.scancode) {
        }

        /**
         * internal data
         */
        bool    pressed;  // is key pressed flag
        bool    ctrl;     // control pressed flag
        bool    alt;      // alt pressed flag
        bool    shift;    // alt pressed flag
        uint8_t scancode; // scancode of the readed character
    } A_PACKED;

    /**
     * keyboard infos,
     * contains the name of the pressed key on the current layout
     */
    class Info : public InfoBasic {
    public:
        /**
         * empty constructor
         */
        Info() : key("KEY_NONE"), InfoBasic() {
        }

        /**
         * filled constructor
         *
         * @param key:		the key name to store
         */
        Info(const std::string& key) : key(key), InfoBasic() {
        }

        /**
         * filled constructor
         *
         * @param pressed:		value to assign to pressed flag
         * @param ctrl:			value to assign to ctrl flag
         * @param alt:			value to assign to alt flag
         * @param shift:		value to assign to shift flag
         * @param scancode:		value to assign to scancode flag
         */
        Info(bool pressed, bool ctrl, bool alt, bool shift, uint8_t scancode)
            : InfoBasic(pressed, ctrl, alt, shift, scancode) {
        }

        /**
         * filled constructor
         *
         * @param key:			the key name to store
         * @param pressed:		value to assign to pressed flag
         * @param ctrl:			value to assign to ctrl flag
         * @param alt:			value to assign to alt flag
         * @param shift:		value to assign to shift flag
         * @param scancode:		value to assign to scancode flag
         */
        Info(const std::string& key,
             bool               pressed,
             bool               ctrl,
             bool               alt,
             bool               shift,
             uint8_t            scancode)
            : key(key), InfoBasic(pressed, ctrl, alt, shift, scancode) {
        }

        /**
         * copy constructor
         */
        Info(const Info& cp)
            : key(cp.key), InfoBasic(cp.pressed, cp.ctrl, cp.alt, cp.shift, cp.scancode) {
        }

        /**
         * copy constructor
         */
        Info(const InfoBasic& bcp) : key("KEY_NONE"), InfoBasic(bcp) {
        }

        /**
         * the key name stored into the info
         */
        std::string key;

        /**
         * < operator overload
         */
        bool operator<(const Info& other) const {
            if ( key > other.key )
                return false;
            if ( key < other.key )
                return true;
            if ( pressed > other.pressed )
                return false;
            if ( pressed < other.pressed )
                return true;
            if ( ctrl > other.ctrl )
                return false;
            if ( ctrl < other.ctrl )
                return true;
            if ( alt > other.alt )
                return false;
            if ( alt < other.alt )
                return true;
            if ( shift > other.shift )
                return false;
            if ( shift < other.shift )
                return true;

            return false;
        }
    };

    static Keyboard& instance();

    /**
     * read a key from shared ps2 driver area
     * and return an info instance with the code of the readed key
     *
     * @param breakCondition:	an additional flag to sincronize the reading
     * @return an info instance with the key code readed
     */
    Info read(bool* breakCondition = nullptr);

    /**
     * convert a scancode key to a keyboard key name
     *
     * @param scancode:		the scancode to convert
     * @param out:			the info to fill to
     * @return whether the conversion success
     */
    bool keyForScancode(uint8_t scancode, Info* out);

    /**
     * convert the provided keyboard info to the corresponsing character
     *
     * @param info:		the keyboard key name
     * @return the converted character
     */
    char charForKey(const Info& info);

    /**
     * creates a full keyboard info from a basic info
     *
     * @param basic:	the basic info to convert
     * @return the full key info
     */
    Info fullKeyInfo(const InfoBasic& basic);

    /**
     * @return the name of the current layout
     */
    std::string getCurrentLayout();

    /**
     * load a new layout from filepath
     *
     * @param iso:		the path to the iso file
     * @return whether the operation success
     */
    bool loadLayout(const std::string& iso);

private:
    Keyboard() = default;

    /**
     * register the keyboard
     * to the shared memory area of the ps2 driver
     */
    void registerKeyboard();

    /**
     * load the scancode file of the layout path provided
     *
     * @param iso:		the path to the iso file
     * @return whether the operation success
     */
    bool loadScancodeLayout(const std::string& iso);

    /**
     * load the conversion file of the layout path provided
     *
     * @param iso:		the path to the iso file
     * @return whether the operation success
     */
    bool loadConversionLayout(const std::string& iso);

private:
    /**
     * global status for special keys
     */
    bool m_status_ctrl{ false };
    bool m_status_shift{ false };
    bool m_status_alt{ false };

    /**
     * map to contains scancodes and conversions of the loaded layoutes
     */
    std::map<uint32_t, std::string> m_scancode_layout{};
    std::map<Keyboard::Info, char>  m_conversion_layout{};

    /**
     * last pressed key
     */
    std::string m_current_layout{};

    /**
     * flag and relative descriptor of last unknow pressed key
     */
    bool m_have_last_unknown_key{ false };
    Info m_last_unknown_key{};
};

#endif
