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

#include <string>
#include <vector>

namespace Utils::Arguments {

/* ----------------------------------- C++ function prototypes ---------------------------------- */

/**
 * @brief Returns all the arguments which doesn't start with '-'
 */
std::vector<std::string> non_filtered(int argc, const char** argv);

/**
 * @brief Returns all the arguments which start with '-'
 */
std::vector<std::string> filtered(int argc, const char** argv);

/**
 * @brief Splits the given <str> for each <delim> and stores the result into <result>
 */
template<typename T>
void split(const std::string& str, char delim, T result);

/**
 * @brief Splits the given <str> for each <delim> and returns an <std::vector>
 */
std::vector<std::string> split(const std::string& str, char delim);

} /* namespace Utils::Arguments */