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

namespace TC::Tag {

enum class Copy : bool { Value };

/**
 * @brief The object must make a copy of the memory
 */
constexpr auto C_COPY = Copy::Value;

} /* namespace TC::Tag */