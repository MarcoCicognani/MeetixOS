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

#include <cstdint>
#include <sstream>
#include <string>

namespace IO {

/* ------------------------------------------ C++ types ----------------------------------------- */

/**
 * @brief Singleton interface to manage mouse client-side
 */
class Mouse {
public:
    /**
     * mouse info descriptor
     */
    struct Info {
    public:
        i16  m_x{ 0 };
        i16  m_y{ 0 };
        bool m_button1{ false };
        bool m_button2{ false };
        bool m_button3{ false };

        Info() = default;
        Info(i16 x, i16 y, bool button1, bool button2, bool button3)
            : m_x{ x }, m_y{ y }, m_button1{ button1 }, m_button2{ button2 }, m_button3{ button3 } {
        }
    };

    /**
     * @brief Returns the global instance of the Mouse manager
     */
    static Mouse& instance();

    /**
     * @brief Reads an action from the mouse device driver and returns the Info
     */
    Info read();
};

} /* namespace IO */