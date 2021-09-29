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

#include <string>

namespace Utils::Environment {

/* ----------------------------------- C++ function prototypes ---------------------------------- */

/**
 * @brief Returns the value of the environment variable which corresponds to <key>
 */
std::string get(const std::string& key);
std::string get(const char* key);

/**
 * @brief Sets the <key>=<value> pair into the environment
 */
void set(const std::string& key, const std::string& value);

/**
 * @brief Returns the currently logged username
 */
std::string getLoggedUser();

/**
 * @brief Returns the hostname
 */
std::string getHostname();

/**
 * @brief Returns the current GUI theme
 */
std::string getTheme();

} /* namespace Utils::Environment */