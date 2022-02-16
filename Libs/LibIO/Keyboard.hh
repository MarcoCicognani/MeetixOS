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

#include <Api/Common.h>
#include <cstdint>
#include <map>
#include <sstream>
#include <string>

namespace IO {

/* ------------------------------------------ C++ types ----------------------------------------- */

/**
 * @brief Singleton interface to manage keyboard client-side
 */
class Keyboard {
public:
    /**
     * @brief Basic key-pressed information
     */
    struct InfoBasic {
    public:
        bool m_is_pressed{ false };
        bool m_ctrl{ false };
        bool m_alt{ false };
        bool m_shift{ false };
        u8   m_scancode{ 0 };

        /**
         * @brief Constructors
         */
        InfoBasic() = default;
        InfoBasic(bool pressed, bool ctrl, bool alt, bool shift, u8 scancode)
            : m_is_pressed{ pressed }
            , m_ctrl{ ctrl }
            , m_alt{ alt }
            , m_shift{ shift }
            , m_scancode{ scancode } {}
    } A_PACKED;

    /**
     * @brief Extended key-pressed information
     */
    struct Info : public InfoBasic {
    public:
        std::string m_key{ "KEY_NONE" };

        /**
         * @brief Constructors
         */
        Info() = default;
        Info(bool pressed, bool ctrl, bool alt, bool shift, u8 scancode)
            : InfoBasic(pressed, ctrl, alt, shift, scancode) {}
        Info(const std::string& key, bool pressed, bool ctrl, bool alt, bool shift, u8 scancode)
            : InfoBasic{ pressed, ctrl, alt, shift, scancode }
            , m_key{ key } {}
        Info(const InfoBasic& basic)
            : InfoBasic{ basic } {}

        /**
         * @brief operator less overload
         */
        bool operator<(const Info& other) const {
            if ( m_key > other.m_key )
                return false;
            if ( m_key < other.m_key )
                return true;
            if ( m_is_pressed > other.m_is_pressed )
                return false;
            if ( m_is_pressed < other.m_is_pressed )
                return true;
            if ( m_ctrl > other.m_ctrl )
                return false;
            if ( m_ctrl < other.m_ctrl )
                return true;
            if ( m_alt > other.m_alt )
                return false;
            if ( m_alt < other.m_alt )
                return true;
            if ( m_shift > other.m_shift )
                return false;
            if ( m_shift < other.m_shift )
                return true;

            return false;
        }
    };

    /**
     * @brief Returns the global instance of the Keyboard manager
     */
    static Keyboard& instance();

    /**
     * @brief Reads a key from the keyboard device driver and returns the Info
     */
    Info read(bool* breakCondition = nullptr);

    /**
     * @brief Converts a scancode key to a keyboard key name
     */
    bool key_for_scancode(u8 scancode, Info* out);

    /**
     * @brief Converts the provided keyboard info to the corresponding char
     */
    char char_for_key(const Info& info);

    /**
     * @brief Returns the full keyboard info from the given basic info
     */
    Info full_key_info(const InfoBasic& basic);

    /**
     * @brief Loads a new layout from the given path
     */
    bool load_layout(const std::string& iso);

    /**
     * @brief Returns the name of the current loaded layout
     */
    [[nodiscard]] const std::string& current_layout() const { return m_current_layout; }

private:
    Keyboard() = default;

    bool load_scancode_layout(const std::string& iso);
    bool load_conversion_layout(const std::string& iso);

    static std::map<std::string, std::string> read_properties(std::ifstream& stream);

private:
    bool                           m_status_ctrl{ false };
    bool                           m_status_shift{ false };
    bool                           m_status_alt{ false };
    std::map<u32, std::string>     m_scancode_layout{};
    std::map<Keyboard::Info, char> m_conversion_layout{};
    std::string                    m_current_layout{};
    bool                           m_have_last_unknown_key{ false };
    Info                           m_last_unknown_key{};
};

} /* namespace IO */