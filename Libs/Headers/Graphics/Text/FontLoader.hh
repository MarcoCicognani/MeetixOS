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

#include <Graphics/Text/Font.hh>
#include <string>

namespace Graphics::Text::FontLoader {

/* ----------------------------------- C++ function prototypes ---------------------------------- */

/**
 * @brief Searches for the font by-name, if it doesn't exists into the loaded list, loads a new one
 * from the filesystem
 */
Font* get(const std::string& name);

/**
 * @brief Returns the default font
 */
Font* get_default();

}; /* namespace Graphics::Text::FontLoader */
